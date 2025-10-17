/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>

#include "ad_base/time/time.h"
#include "ad_base/time/clock.h"
#include "ad_base/time/rate.h"

namespace senseAD {
namespace rscl {

namespace time = senseAD::base::time;

using senseAD::base::common::ListenerHandle;

void InitTimeSystem();

void ShutdownTimeSystem();

}  // namespace rscl
}  // namespace senseAD
