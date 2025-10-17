/*
 * Copyright (C) 2020-2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include "ad_buf/ad_buf.h"

namespace senseAD {
namespace ad_buf {

/**
 * @brief Assume the user have construct an array of FixedSizeArenaBlock to form
 * a arena
 *
 * FixedSizeArenaBlock is a block to store information
 */
struct FixedSizeArenaBlock {
    static constexpr uint8_t kStateEmpty = 0;
    static constexpr uint8_t kStateAllocated = 1;
    static constexpr uint8_t kStateAllocatable = 2;

    uint32_t offset = 0;
    uint32_t size = 0;

    // Three possible state
    uint8_t state = kStateEmpty;

    AdBuf::TokenT token;
    uint32_t lock_index;

    // Let the block to be reuse, but still take space in arena
    inline void Release() {
        state = kStateAllocatable;
        token = 0;
        lock_index = 0;
    }

    // Release + no longer take space in arena
    inline void Clear() {
        offset = 0;
        size = 0;
        state = kStateEmpty;
        token = 0;
        lock_index = 0;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    FixedSizeArenaBlock const& m) {
        return os << "(offset: " << m.offset << ", size: " << m.size
                  << ", state: " << static_cast<int>(m.state)
                  << ", token: " << m.token << ", lock_index: " << m.lock_index
                  << ")";
    }
};

void DefragmentFixedSizeArena(FixedSizeArenaBlock* arena_head,
                              uint32_t* allocated_block_count,
                              uint32_t* total_block_count);

FixedSizeArenaBlock* AllocatedNewBlockFromArena(
    FixedSizeArenaBlock* arena_head,
    uint32_t* total_block_count,
    const size_t required_block_size,
    const size_t max_block_limit);

int32_t GetBlockIndex(const FixedSizeArenaBlock* arena_head,
                      const uint32_t total_block_count,
                      const AdBuf::TokenT token);

base::optional<uint32_t> FindFreeLockIndex(
    const FixedSizeArenaBlock* arena_head,
    const uint32_t total_block_count,
    const size_t max_block_limit);

}  // namespace ad_buf
}  // namespace senseAD
