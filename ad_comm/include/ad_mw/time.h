/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cstdint>

#include "ad_mw/visibility_control.h"

namespace senseAD {
namespace mw {
namespace time {

// Below are temporory solution
enum class ClockMode { SYSTEM_TIME, MOCK_TIME };
ADMW_PUBLIC void SetMode(const ClockMode mode);
ADMW_PUBLIC void SetMockNow(const uint64_t now_ns);
ADMW_PUBLIC uint64_t NowInNanoseconds();
}  // namespace time
}  // namespace mw
}  // namespace senseAD
