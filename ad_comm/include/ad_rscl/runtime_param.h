/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 */

#pragma once

#include <string>
#include <map>

#include "ad_rscl/runtime_param/runtime_param_manager.h"

namespace senseAD {
namespace rscl {
namespace runtime {
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
void RegisterInnerParam(const std::string &node_name);
/**
 * @brief Register param for node
 *
 * @param node_name Name of node
 * @param param_name Name of added param
 * @param param_value value of added param
 * @param set_cb set param callback function, called if param value changed
 * @param get_cb get param callback function, called if recv get param cmd
 * not
 * @return True if success to add param, or false
 */
bool AddParam(const std::string &node_name,
              const std::string &param_name,
              const std::string &param_value,
              const RuntimeParamSetCallBack &set_cb,
              const RuntimeParamGetCallBack &get_cb);
/**
 * @brief Add params for node
 *
 * @param node_name Name of node
 * @param param_cbs map struct is: map<param_name, Param>
 * not
 * @return True if success to add all params, or false
 */
bool AddParams(const std::string &node_name,
               const std::map<std::string, Param> &param_cbs);
}  // namespace runtime
}  // namespace rscl
}  // namespace senseAD
