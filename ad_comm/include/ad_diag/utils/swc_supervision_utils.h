/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include "ad_diag/base/ad_diag_base.h"

namespace senseAD {
namespace diag {

template <typename BaseSupervision, SupervisionUniqueId uid>
class SwcSupervision : public BaseSupervision {
 public:
    SwcSupervision() : BaseSupervision(uid) {}
};

}  // namespace diag
}  // namespace senseAD
