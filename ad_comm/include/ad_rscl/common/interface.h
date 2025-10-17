/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

namespace senseAD {
namespace rscl {
namespace common {

class Suspendable {
 public:
    virtual ~Suspendable() = default;
    virtual bool Suspend() = 0;
    virtual bool Resume() = 0;
};

}  // namespace common
}  // namespace rscl
}  // namespace senseAD
