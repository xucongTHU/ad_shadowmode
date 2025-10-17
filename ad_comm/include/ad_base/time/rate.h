/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>

#include "ad_base/time/clock.h"

namespace senseAD {
namespace base {
namespace time {

/**
 * @brief Class to ensure the logic will be executable with target interval
 *
 */
class Rate {
 public:
    // interval ms = 1000 / frequency
    explicit Rate(double frequency);
    // interval ms = nanoseconds * 1e-6
    explicit Rate(uint64_t nanoseconds);
    // Sleep until the next expect timestamp
    bool Sleep();
    void Reset();

    // for abnormal sleep error log
    void SetTag(const std::string& tag) { rate_tag_ = tag; }

 private:
    int64_t start_ns_;
    int64_t expected_cycle_time_ns_;

    std::string rate_tag_;
};
}  // namespace time
}  // namespace base
}  // namespace senseAD
