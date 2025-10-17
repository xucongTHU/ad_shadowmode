/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "ad_rscl/comm/raw_impl/raw_comm.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace raw {

class RawPublisherFactory {
 public:
    static std::unique_ptr<RawPublisherBase> CreatePublisher(
        const std::string& name, CustomConfig config = CustomConfig());
};

}  // namespace raw
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
