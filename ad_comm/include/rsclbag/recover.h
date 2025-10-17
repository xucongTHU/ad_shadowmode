/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once
#include <string>
#include <deque>
#include <queue>
#include <memory>
#include <fstream>
#include <chrono>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <cmath>

#include "ad_log/ad_log.hpp"
#include "ad_bag/bag_writer.h"
#include "ad_bag/io/bag_section_visitor.h"
#include "ad_bag/schema/schema_parse.h"
#include "ad_log/profiler.hpp"

const char tmp_file_prefix[] = "sorted_tmpblock_";

// This struct represents an item used for merging messages
struct MergeItem {
    std::string filename;
    uint64_t timestamp;
    uint64_t channel_uid;
    // Serialized data of the message
    std::vector<char> message_buffer;

    std::shared_ptr<std::ifstream> fptr;  // The file containing the message
    /**
     * Overload the greater than operator for comparing MergeItems
     * @param other - The other MergeItem to compare with
     * @return True if this MergeItem's message timestamp is greater than the
     * other MergeItem's message timestamp, false otherwise
     */
    bool operator>(const MergeItem &other) const {
        return timestamp > other.timestamp;
    }

    bool operator<(const MergeItem &other) const {
        return timestamp < other.timestamp;
    }

    void Clear() {
        filename.clear();
        timestamp = 0;
        channel_uid = 0;
        message_buffer.clear();
        fptr = nullptr;
    }
};

using MinMsgHeap = std::
    priority_queue<MergeItem, std::vector<MergeItem>, std::greater<MergeItem>>;

class RsclBagRecover {
 public:
    RsclBagRecover(const std::string &in_bag_path,
                   const std::string &out_bag_path)
        : in_bag_path_(in_bag_path), out_bag_path_(out_bag_path) {}
    ~RsclBagRecover() { CleanTempFile(); }
    int Recover();

 private:
    void CleanTempFile() {
        for (int i = 0; i <= file_index_; i++) {
            std::string file_name = tmp_file_prefix + std::to_string(i);
            std::remove(file_name.c_str());
        }
    }
    bool ReadFromFileStream(std::shared_ptr<std::ifstream> input,
                            MergeItem *msg);
    void SortAndWriteToFile(std::vector<MergeItem> &block,  // NOLINT
                            MinMsgHeap &heap);              // NOLINT
    void MergeSort(MinMsgHeap &heap);                       // NOLINT
    void GetDisorderItems();
    void RecoverHeader(senseAD::bag::BagHeader &header);  // NOLINT
    void MergeDisorderItems();
    void SplitOriginalBagAndMerge();
    void RecoverWithNoDisorderItem();

    std::string in_bag_path_;
    std::string out_bag_path_;
    std::unordered_map<uint64_t, senseAD::bag::ChannelInfo> channel_infos_;
    std::deque<senseAD::bag::MessageHeader> message_headers_;
    std::deque<senseAD::bag::ChunkHeader> chunk_headers_;
    senseAD::bag::BagWriteExtraInfo extra_bag_info_;
    std::unordered_set<std::string> disorder_msgs_;
    uint32_t total_disorder_block_bytes_{0};
    std::vector<MergeItem> disorder_block_;
    senseAD::bag::BagWriterAttribute attr_;
    std::shared_ptr<senseAD::bag::BagWriter> bag_writer_;
    int file_index_{0};
};
