/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 */

#pragma once

#include <string>
#include <map>

#include "ad_rscl/runtime_param/runtime_param_manager.h"

namespace senseAD {
namespace rscl {
namespace param {
using RuntimeParamSetCallBack = std::function<bool(
    const std::string &, const std::string &, const std::string &)>;
using RuntimeParamGetCallBack =
    std::function<std::string(const std::string &, const std::string &)>;

using senseAD::rscl::param::Param;
/**
 * @brief  Register the params of MW itself
 *
 * @param node_name Name of component
 */
void RegisterRsclBuiltInParam(const std::string &node_name);

}  // namespace param
}  // namespace rscl
}  // namespace senseAD
