/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <chrono>

namespace senseAD {
namespace base {
namespace common {

inline uint64_t GetCurrentStampNs() {
    std::chrono::system_clock::time_point now =
        std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               now.time_since_epoch())
        .count();
}

}  // namespace common
}  // namespace base
}  // namespace senseAD
