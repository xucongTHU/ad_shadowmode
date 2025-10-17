/*
 * Copyright (C) 2020-2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <pthread.h>

#include <type_traits>
#include <iostream>
#include <string>
#include <memory>

namespace senseAD {
namespace ad_buf {

struct AdBufManagerMmapInfo;
class BufCfg {
 public:
    virtual ~BufCfg() = default;
    inline std::string buf_type() const { return buf_type_; }

    inline std::string key() const { return key_; }
    //! IMPORTANT: Not suppose to be call by user
    inline BufCfg& key(const std::string& key) {
        key_ = key;
        return *this;
    }

    inline uint32_t max_buf_count() const { return max_buf_count_; }
    inline BufCfg& max_buf_count(const uint32_t max_buf_count) {
        max_buf_count_ = max_buf_count;
        return *this;
    }

    inline uint32_t specific_info_size() const { return specific_info_size_; }

    inline uint32_t max_bufhandle_size() const { return max_bufhandle_size_; }

    friend std::ostream& operator<<(std::ostream& os, BufCfg const& m) {
        return os << "(buf_type: " << m.buf_type() << ", key: " << m.key()
                  << " , max_buf_count: " << m.max_buf_count()
                  << " , specific_info_size: " << m.specific_info_size()
                  << " , max_bufhandle_size: " << m.max_bufhandle_size() << ")";
    }

 protected:
    inline BufCfg& buf_type(const std::string& buf_type) {
        buf_type_ = buf_type;
        return *this;
    }
    inline BufCfg& specific_info_size(const uint32_t specific_info_size) {
        specific_info_size_ = specific_info_size;
        return *this;
    }
    inline BufCfg& max_bufhandle_size(const uint32_t max_bufhandle_size) {
        max_bufhandle_size_ = max_bufhandle_size;
        return *this;
    }

 private:
    std::string buf_type_ = "malloc";
    std::string key_;

    uint32_t max_buf_count_ = 4;
    uint32_t specific_info_size_ = 0;
    uint32_t max_bufhandle_size_ = 1024;

    friend struct AdBufManagerMmapInfo;
};

class LockGuard {
 public:
    explicit LockGuard(pthread_mutex_t& mtx) : mtx_(mtx) {
        pthread_mutex_lock(&mtx_);
    }
    ~LockGuard() { pthread_mutex_unlock(&mtx_); }

 private:
    pthread_mutex_t& mtx_;
};

struct AdBufManagerMmapInfo {
    char buf_type[256];
    char key[256];
    uint32_t max_buf_count;

    pthread_mutexattr_t global_lock_attr;
    pthread_mutex_t global_lock;

    char gen_id[64];
    uint64_t master_pid;
    char process_create_time[64];

    uint32_t specific_info_size;
    uint32_t max_bufhandle_size;

    bool init_done;
    uint64_t g_revision = 0;

    inline BufCfg ToBufCfg() const {
        BufCfg meta;
        meta.buf_type(buf_type)
            .key(key)
            .max_buf_count(max_buf_count)
            .specific_info_size(specific_info_size)
            .max_bufhandle_size(max_bufhandle_size);
        return meta;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    AdBufManagerMmapInfo const& m) {
        return os << "(buf_type: " << m.buf_type << ", key: " << m.key
                  << ", max_buf_count: " << m.max_buf_count
                  << ", gen_id: " << m.gen_id
                  << ", master_pid: " << m.master_pid
                  << ",process_create_time:" << m.process_create_time
                  << ", specific_info_size: " << m.specific_info_size
                  << ", max_bufhandle_size: " << m.max_bufhandle_size
                  << ", init_done: " << m.init_done << ")";
    }
};
static_assert(std::is_trivially_copyable<AdBufManagerMmapInfo>::value,
              "AdBufManagerMmapInfo must be a POD type.");

struct ImgMeta {
    uint32_t img_width = 0;
    uint32_t img_height = 0;
    uint32_t img_stride = 0;
    uint32_t img_scanlines = 0;

    enum class ImgChannelFormat { NONE, BGR, RGB, YUV420, NV12 };
    ImgChannelFormat channel = ImgChannelFormat::NONE;
    uint64_t img_capture_timestamp = 0;

    inline void Clear() {
        img_width = 0;
        img_height = 0;
        img_stride = 0;
        img_scanlines = 0;
        channel = ImgChannelFormat::NONE;
        img_capture_timestamp = 0;
    }
};

struct BufMeta {
    uint64_t revision = 0;

    // Only For the image case
    ImgMeta img_meta;

    inline void Clear() {
        revision = 0;
        img_meta.Clear();
    }
};
static_assert(std::is_trivially_copyable<BufMeta>::value,
              "AdBufManagerMmapInfo must be a POD type.");

}  // namespace ad_buf
}  // namespace senseAD
