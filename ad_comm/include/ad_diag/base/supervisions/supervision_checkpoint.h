/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include "ad_diag/base/supervisions/supervision.h"

namespace senseAD {
namespace diag {

class CheckPoint {
 public:
    explicit CheckPoint(std::function<void()> cb);

    void operator()() const;

 private:
    std::function<void()> cb_;
};

/***************************************************
 * !Check Point That Use Checkpoint                *
 ***************************************************/

class LogicalSupervision : public SupervisionBase {};

class AliveSupervision : public SupervisionBase {};

class DeadlineSupervision : public SupervisionBase {};

class HealthChannelSupervision : public SupervisionBase {};

}  // namespace diag
}  // namespace senseAD
