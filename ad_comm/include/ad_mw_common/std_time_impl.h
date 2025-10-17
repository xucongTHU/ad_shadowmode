/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * kitfung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <chrono>

#include "ad_mw/time.h"

namespace senseAD {
namespace mw_common {
namespace time {

using senseAD::mw::time::ClockMode;

void SetMode(const ClockMode mode);
void SetMockNow(const uint64_t now_ns);
uint64_t NowInNanoseconds();

}  // namespace time
}  // namespace mw_common
}  // namespace senseAD
