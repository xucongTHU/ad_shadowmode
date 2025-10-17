/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Wang Kun <wangkun2@senseauto.com>
 */

#pragma once

#include "ad_rscl/idl/sm_conf.pb.h"

namespace senseAD {
namespace ad_sm {

using rscl::idl::SMStatus;

class SMClient {
 public:
    using StateChangeFunc = std::function<void(SMStatus)>;

    virtual SMStatus GetSMStatus() = 0;
    virtual void RegisterStateChangeFunc(StateChangeFunc func) = 0;

 protected:
    SMStatus sm_status_;
};

}  // namespace ad_sm
}  // namespace senseAD
