/*
 * Copyright (C) 2022  y SenseTime Group Limited. All rights reserved.
 * Mu Chunpeng <muchunpeng@sensetime.com>
 * Cai Wenkang <caiwenkang@sensetime.com>
 */

#pragma once

#include <cassert>
#include <map>
#include <mutex>
#include <string>
#include <utility>

#include "ad_base/common/macros.h"
#include "concurrency/robust_lock.h"

namespace senseAD {
namespace rscl {
namespace comm {
using senseAD::ad_shm::concurrent::RobustLock;

constexpr int kMaxQueueSize = 256;

#define SHMALIGN(s, a) (((s - 1) | (a - 1)) + 1)

inline uint8_t *align_address(void *ptr, size_t alignment) {
    auto int_ptr = reinterpret_cast<uintptr_t>(ptr);
    auto aligned_int_ptr = SHMALIGN(int_ptr, alignment);
    return reinterpret_cast<uint8_t *>(aligned_int_ptr);
}

class BufferPtrManager {
 public:
    uint8_t *get_memory_ptr(char *name) {
        std::unique_lock<std::mutex> lock(mutex_);
        return ptr_map_.at(std::string(name));
    }
    uint8_t *get_memory_ptr(std::string name) {
        std::unique_lock<std::mutex> lock(mutex_);
        return ptr_map_.at(name);
    }
    void set_memory_ptr(std::string name, uint8_t *p) {
        std::unique_lock<std::mutex> lock(mutex_);
        ptr_map_.insert(make_pair(name, p));
    }

 private:
    std::mutex mutex_;
    std::map<std::string, uint8_t *> ptr_map_;

    DECLARE_AD_SINGLETON(BufferPtrManager)
};

struct MemNode {
    uint32_t alloc_index_;
    bool free_;
    size_t size_;
    uint32_t reference_count_;
    MemNode() : alloc_index_(0), free_(true), size_(0), reference_count_(0) {}
    MemNode(uint32_t alloc_index, bool free, size_t size)
        : alloc_index_(alloc_index),
          free_(free),
          size_(size),
          reference_count_(0) {}
};

class Allocator {
 public:
    using handle = uint64_t;

    Allocator(std::string name, size_t size);
    uint8_t *Alloc(uint32_t bytes);
    uint8_t *Alloc(uint32_t bytes, size_t alignment);
    bool Free(const uint8_t *ptr);
    bool Lock(const uint8_t *ptr);
    void Reset();

    inline handle PtrToHandle(uint8_t *p) {
        return p - reinterpret_cast<uint8_t *>(Heap_());
    }
    inline uint8_t *HandleToPtr(handle h) {
        return reinterpret_cast<uint8_t *>(Heap_()) + h;
    }

    size_t GetTotalSize() { return size_; }
    uint32_t GetPayloadArrayIndex(const handle alloc_index) const;

    uint32_t IncCounter(const handle alloc_index);
    uint32_t SubCounter(const handle alloc_index);
    uint32_t Counter(const handle alloc_index);

    inline std::array<MemNode, kMaxQueueSize> GetPayloadArray() const {
        return payload_array_;
    }
    RobustLock lock_;

 private:
    void ValidateIndex(uint32_t index) const;
    void *SuggestAddr(uint32_t payload_size);
    void InitPayloadArray(uint32_t payload_size);
    uint32_t *__attribute__((always_inline)) Heap_() {
        return reinterpret_cast<uint32_t *>(
            BufferPtrManager::Instance()->get_memory_ptr(name_));
    }

    size_t size_;
    uint32_t node_index_;
    uint32_t node_count_;
    bool is_inited_;
    std::array<MemNode, kMaxQueueSize> payload_array_;
    char name_[64];
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
