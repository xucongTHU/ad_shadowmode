/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include "ad_mw/comm.h"
#include "ad_mw/runtime.h"
#include "ad_mw/task.h"

/**
 * ad_mw header
 */
#include "ad_mw/time.h"

#include "ad_mw_cyber/task.h"
#include "ad_mw_cyber/feature_manager.h"

namespace senseAD {
namespace mw {
constexpr const char* GetAdMwImplementation() { return "ad_mw_cyber"; }
}  // namespace mw
}  // namespace senseAD
