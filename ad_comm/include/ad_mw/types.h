/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <memory>
#include <utility>
#include <functional>

#include "ad_serde/wrapped_type/base.h"
#include "ad_mw/proto/comm_conf.pb.h"
// From protobuf
#include "ad_mw/proto/qos_profile.pb.h"

namespace senseAD {
namespace mw {

class NullType {};

/**
 * @brief An pointer storing a buffer allocated from mw
 * Designed to used for sending data
 *
 * TODO(kit) Let all mw Publisher impl provide the allocate api
 */
using BorrowedRawMessageAllocateePtr =
    std::unique_ptr<BorrowedRawMessage,
                    std::function<void(BorrowedRawMessage*)>>;

}  // namespace mw
}  // namespace senseAD
