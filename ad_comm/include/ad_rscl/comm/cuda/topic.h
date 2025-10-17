/*
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * Mu Chunpeng <muchunpeng@sensetime.com>
 */

#pragma once

#include <atomic>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

#include "ad_rscl/comm/cuda/memory.h"

namespace senseAD {
namespace rscl {
namespace comm {

enum TopicFlags {
    // Only need to read
    READ_ONLY = 1 << 0,
    // publisher and receiver can communication without ipc
    SAME_PROCESS = 1 << 1,
};

inline TopicFlags operator|(TopicFlags a, TopicFlags b) {
    return static_cast<TopicFlags>(static_cast<uint64_t>(a) |
                                   static_cast<uint64_t>(b));
}
inline MemoryPrivateConfig_t ToMemoryPrivateConfig(TopicFlags flag) {
    MemoryPrivateConfig_t cfg;
    cfg.read_only = flag & READ_ONLY;
    cfg.ipc_handle_enable = !(flag & SAME_PROCESS);
    return cfg;
}

/**
 * @brief An read/write interface for the underlying "Memory"
 * TODO Totally rewrite and redesign it. And implementation this in mw
 *      implementation level
 */
class Topic {
 public:
    explicit Topic(const MemorySharedConfig_t config)
        : memory_(config, MemoryPrivateConfig_t()) {}
    Topic(const MemorySharedConfig_t config, TopicFlags flag)
        : memory_(config, ToMemoryPrivateConfig(flag)) {}

    ~Topic() = default;

    bool get_shm_status() { return memory_.get_shm_status(); }
    const MemorySharedConfig_t &get_memory_config() const {
        return memory_.get_memory_config();
    }

    bool write(const Memblock &memblock, uint64_t *seq);

    bool read(const uint64_t &seq, Memblock *memblock);

    bool prealloc_memblock(const uint64_t &block_byte, Memblock *memblock);
    void free_prealloced_memblock(const Memblock &memblock);

    inline __attribute__((always_inline)) uint32_t inc_counter() {
        return memory_.shared_queue_->counter.fetch_add(1);
    }

    inline __attribute__((always_inline)) uint32_t counter() const {
        return memory_.shared_queue_->counter.load();
    }

    inline size_t queue_size() const { return memory_.queue_size(); }

    static inline std::string mock_channel_name() {
        return "/_internal_faked/cuda_mock/0000000";
    }

 private:
    Memory<LockableElement> memory_;
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
