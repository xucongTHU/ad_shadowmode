/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>

#include "ad_diag/base/supervisions/supervision.h"
#include "ad_diag/base/supervisions/supervision_simple.h"
#include "ad_diag/base/supervisions/supervision_checkpoint.h"
#include "ad_diag/base/supervisions/supervision_system.h"
#include "ad_diag/base/supervisions/supervision_comm.h"
#include "ad_diag/base/supervisions/supervision_component.h"
#include "ad_diag/base/supervisions/sensor_supervision.h"
#include "ad_diag/base/supervisions/supervision_workshop.h"

namespace senseAD {
namespace diag {

void Init(const std::string &process_name);

}  // namespace diag
}  // namespace senseAD
