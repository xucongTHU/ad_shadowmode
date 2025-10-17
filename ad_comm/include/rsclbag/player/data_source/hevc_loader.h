/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * chenzhanghao <chenzhanghao@senseauto.com>
 */
#pragma once

#include <sys/mman.h>  // for mmap
#include <fcntl.h>     // for open
#include <unistd.h>    // for close

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <sstream>
#include <memory>

#include "yaml-cpp/yaml.h"
#include "ad_rscl/ad_rscl.h"

struct HevcFrameData {
    std::vector<uint8_t> data;
    uint8_t nalu_type;
    uint64_t timestamp;
    std::string camera_name_;
};

class HevcLoader {
 private:
    HevcLoader(const HevcLoader &) = delete;
    HevcLoader &operator=(const HevcLoader &) = delete;

 public:
    HevcLoader() = default;

    ~HevcLoader() {
        if (mapped_ != nullptr) {
            munmap(mapped_, file_size_);
        }
    }

    int Init(std::string hevc_filename,
             std::string timestamp_filename,
             uint64_t begin_time_ns = 0,
             uint64_t end_time_ns = std::numeric_limits<uint64_t>::max()) {
        std::ifstream file(timestamp_filename.c_str());
        if (!file.is_open()) {
            AD_LERROR(REC_REPLAY)
                << "can not open timestamp tile: " << timestamp_filename
                << std::endl;
            return -1;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int frameNumber;
            int64_t timestamp;
            char comma;
            if (iss >> frameNumber >> comma >> timestamp) {
                timestamps_.push_back(timestamp);
            }
        }
        file.close();
        AD_LINFO(REC_REPLAY)
            << "timestamp count: " << timestamps_.size() << std::endl;

        if (timestamps_.empty()) {
            AD_LERROR(REC_REPLAY)
                << "No valid timestamps found in file: " << timestamp_filename
                << std::endl;
            return -1;
        }

        hevc_file_.open(hevc_filename, std::ios::in | std::ios::binary);
        if (!hevc_file_.is_open()) {
            AD_LERROR(REC_REPLAY)
                << "can not open hevc file: " << hevc_filename << std::endl;
            return -1;
        }

        // 获取文件大小
        hevc_file_.seekg(0, std::ios::end);
        file_size_ = hevc_file_.tellg();
        hevc_file_.seekg(0, std::ios::beg);

        // 使用mmap读取文件
        int fd = open(hevc_filename.c_str(), O_RDONLY);
        if (fd == -1) {
            AD_LERROR(REC_REPLAY)
                << "can not open HEVC file: " << hevc_filename << std::endl;
            return -1;
        }

        mapped_ = reinterpret_cast<uint8_t *>(
            mmap(0, file_size_, PROT_READ, MAP_SHARED, fd, 0));
        if (mapped_ == MAP_FAILED) {
            AD_LERROR(REC_REPLAY) << "mmap failed" << std::endl;
            close(fd);
            return -1;
        }
        close(fd);
        AD_LINFO(REC_REPLAY) << "hevc file size: " << file_size_ << std::endl;

        SetRange(begin_time_ns, end_time_ns);

        // 获取相机名，去除文件后缀名(长度不固定)和路径
        auto pos = hevc_filename.find_last_of('/');
        if (pos == std::string::npos) {
            pos = 0;
        } else {
            pos++;
        }
        auto dot_pos = hevc_filename.find_last_of('.');
        if (dot_pos == std::string::npos) {
            dot_pos = hevc_filename.size();
        }
        camera_name_ = hevc_filename.substr(pos, dot_pos - pos);
        return 0;
    }

    inline uint64_t FrameCount() const { return timestamps_.size(); }

    inline uint64_t GetFirstFrameTimeNs() const { return timestamps_[0]; }

    inline uint64_t GetLastFrameTimeNs() const {
        return timestamps_[timestamps_.size() - 1];
    }

    void SetRange(uint64_t begin_time_ns, uint64_t end_time_ns) {
        AD_LINFO(REC_REPLAY)
            << "Set HEVC range: begin_time_ns: " << begin_time_ns
            << ", end_time_ns: " << end_time_ns << std::endl;
        offset_ = 0;
        timestamp_offset_ = 0;
        this->end_time_ns_ = end_time_ns;
        while (true) {
            uint64_t timestamp = PeekTimestamp();
            if (timestamp == 0) {
                AD_LERROR(REC_REPLAY)
                    << "No valid timestamp found in HEVC file: "
                    << camera_name_;
                break;
            }
            if (timestamp >= begin_time_ns) {
                AD_LINFO(REC_REPLAY)
                    << "Found first timestamp: " << timestamp
                    << " greater than or equal to begin_time_ns: ";
                break;
            }
            if (timestamp >= end_time_ns) {
                AD_LERROR(REC_REPLAY)
                    << "now timestamp: " << timestamp
                    << " is greater than end_time_ns: " << end_time_ns;
            }
            PopHevcFrame();
        }
        begin_offset_ = offset_;
        begin_timestamp_offset_ = timestamp_offset_;
    }

    void Reset() {
        offset_ = begin_offset_;
        timestamp_offset_ = begin_timestamp_offset_;
    }

    uint64_t PeekTimestamp() {
        if (timestamp_offset_ >= timestamps_.size()) {
            return 0;
        }
        if (timestamps_[timestamp_offset_] > end_time_ns_) {
            return 0;
        }
        return timestamps_[timestamp_offset_];
    }

    std::shared_ptr<HevcFrameData> PopHevcFrame() {
        if (PeekTimestamp() == 0) {
            return nullptr;
        }

        auto create_msg = [this](uint64_t start, uint64_t end) {
            auto ret = std::make_shared<HevcFrameData>();
            ret->data.resize(end - start);
            std::copy(this->mapped_ + start, this->mapped_ + end,
                      ret->data.begin());
            ret->timestamp = this->timestamps_[this->timestamp_offset_++];
            ret->camera_name_ = this->camera_name_;
            ret->nalu_type = (this->mapped_[start + 4] >> 1) & 0x3f;
            if (ret->nalu_type != 1) {
                ret->nalu_type = 19;
            }
            return ret;
        };

        auto start_offset = offset_;

        while (true) {
            offset_++;  // 先+1是为了避免起始就是0x00 0x00 0x00 0x01的情况

            if (offset_ + 4 >= file_size_) {
                break;
            }

            if ((mapped_[offset_] == 0x00 && mapped_[offset_ + 1] == 0x00 &&
                 mapped_[offset_ + 2] == 0x00 &&
                 mapped_[offset_ + 3] == 0x01) &&
                (mapped_[offset_ + 4] == 0x40 ||
                 mapped_[offset_ + 4] == 0x02)) {
                return create_msg(start_offset, offset_);
            }
        }

        auto ret = create_msg(start_offset, file_size_);
        offset_ = file_size_;
        return ret;
    }

 public:
    std::string camera_name_;
    std::vector<uint64_t> timestamps_;
    uint64_t timestamp_offset_ = 0;
    uint64_t begin_timestamp_offset_ = 0;
    std::ifstream hevc_file_;
    uint8_t *mapped_ = nullptr;
    uint64_t file_size_ = 0;
    uint64_t offset_ = 0;
    uint64_t begin_offset_ = 0;
    uint64_t end_time_ns_ = 0;
};

class MultiHevcLoader {
 public:
    MultiHevcLoader(const MultiHevcLoader &) = delete;
    MultiHevcLoader &operator=(const MultiHevcLoader &) = delete;

 public:
    MultiHevcLoader() {}
    ~MultiHevcLoader() {}

    int Init(std::vector<std::string> hevc_filenames,
             std::vector<std::string> timestamp_filenames,
             uint64_t begin_time_ns = 0,
             uint64_t end_time_ns = std::numeric_limits<uint64_t>::max()) {
        if (hevc_filenames.size() != timestamp_filenames.size()) {
            AD_LERROR(REC_REPLAY)
                << "HEVC file count not equal timestamp file count"
                << std::endl;
            return -1;
        }

        for (size_t i = 0; i < hevc_filenames.size(); i++) {
            auto loader = std::make_shared<HevcLoader>();
            if (loader->Init(hevc_filenames[i], timestamp_filenames[i],
                             begin_time_ns, end_time_ns) != 0) {
                return -1;
            }
            loaders.push_back(loader);
        }
        return 0;
    }

    void Reset() {
        for (auto &loader : loaders) {
            loader->Reset();
        }
    }

    inline uint64_t FrameCount() const {
        uint64_t count = 0;
        for (auto &loader : loaders) {
            count += loader->FrameCount();
        }
        return count;
    }

    inline uint64_t GetFirstFrameTimeNs() const {
        uint64_t min_timestamp = std::numeric_limits<uint64_t>::max();
        for (auto &loader : loaders) {
            if (loader->FrameCount() == 0) {
                continue;
            }
            auto timestamp = loader->GetFirstFrameTimeNs();
            if (timestamp < min_timestamp) {
                min_timestamp = timestamp;
            }
        }
        return min_timestamp;
    }

    inline uint64_t GetLastFrameTimeNs() const {
        uint64_t max_timestamp = 0;
        for (auto &loader : loaders) {
            if (loader->FrameCount() == 0) {
                continue;
            }
            auto timestamp = loader->GetLastFrameTimeNs();
            if (timestamp > max_timestamp) {
                max_timestamp = timestamp;
            }
        }
        return max_timestamp;
    }

    uint64_t PeekTimestamp() {
        if (loaders.empty()) {
            return 0;
        }
        get_min_loader();
        return min_loader->PeekTimestamp();
    }

    std::shared_ptr<HevcFrameData> PopHevcFrame() {
        if (loaders.empty()) {
            return nullptr;
        }
        get_min_loader();
        auto ret = min_loader->PopHevcFrame();
        min_loader = nullptr;
        return ret;
    }

    static void ReadInputJson(std::vector<std::string> *hevc_filenames,
                              std::vector<std::string> *timestamp_filenames) {
        std::string launcher_replay;
        senseAD::base::common::GetEnvironmentVariable("launcher_replay",
                                                      &launcher_replay);
        if (launcher_replay != "True" && launcher_replay != "true" &&
            launcher_replay != "1" && launcher_replay != "TRUE") {
            return;
        }

        std::string json_path = "/tmp/input.json";

        YAML::Node root;
        if (!rscl::common::GetYamlFromASCIIFile(json_path, &root)) {
            AD_LERROR(REC_REPLAY) << "The input.json is invalid: " << json_path;
            return;
        }
        if (!root["input_config"] || !root["input_config"]["base_path"]) {
            AD_LERROR(REC_REPLAY)
                << "The input.json is invalid. Missing input_config/base_path: "
                << json_path << std::endl;
            return;
        }

        auto base_path = root["input_config"]["base_path"].as<std::string>();

        for (const auto &input : root["input_config"]["inputs"]) {
            if (input["type"].as<std::string>() == "VIDEO") {
                hevc_filenames->push_back(base_path + "/" +
                                          input["filepath"].as<std::string>());
                timestamp_filenames->push_back(
                    base_path + "/" + input["timestamp"].as<std::string>());
            }
        }
    }

 public:
    void get_min_loader() {
        if (min_loader != nullptr) {
            return;
        }

        if (loaders.empty()) {
            return;
        }

        min_loader = loaders[0];
        uint64_t min_timestamp = std::numeric_limits<uint64_t>::max();
        for (auto &loader : loaders) {
            auto timestamp = loader->PeekTimestamp();
            if (timestamp == 0) {
                continue;
            }
            if (timestamp < min_timestamp) {
                min_timestamp = loader->PeekTimestamp();
                min_loader = loader;
            }
        }
    }
    std::vector<std::shared_ptr<HevcLoader>> loaders;
    std::shared_ptr<HevcLoader> min_loader = nullptr;
};
