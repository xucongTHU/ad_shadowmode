/*
 * Copyright (C) 2020-2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cstring>
#include <memory>
#include <vector>

#include "ad_msg_idl/std_msgs/typed_data_list.capnp.h"

namespace senseAD {
namespace ad_serde {

// Convert capnp float list to std::vector<float>
void ToIdlFloatVector(const std::vector<float>& in,
                      senseAD::msg::std_msgs::Float32List::Builder out,
                      const size_t size = 0);
// Convert std::vector<float> to capnp float list
void ToStdFloatVector(const senseAD::msg::std_msgs::Float32List::Reader in,
                      std::vector<float>* out,
                      const size_t size = 0);

// Convert capnp double list to std::vector<double>
void ToIdlDoubleVector(const std::vector<double>& in,
                       senseAD::msg::std_msgs::Float64List::Builder out,
                       const size_t size = 0);
// Convert std::vector<double> to capnp double list
void ToStdDoubleVector(const senseAD::msg::std_msgs::Float64List::Reader in,
                       std::vector<double>* out,
                       const size_t size = 0);

}  // namespace ad_serde
}  // namespace senseAD

// Backward Compatibility
namespace senseAD {
namespace ad_msg_bridge {
const auto& ToIdlFloatVector = senseAD::ad_serde::ToIdlFloatVector;
const auto& ToStdFloatVector = senseAD::ad_serde::ToStdFloatVector;
const auto& ToIdlDoubleVector = senseAD::ad_serde::ToIdlDoubleVector;
const auto& ToStdDoubleVector = senseAD::ad_serde::ToStdDoubleVector;
}  // namespace ad_msg_bridge
}  // namespace senseAD
