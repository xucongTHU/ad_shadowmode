/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <regex>
#include <string>
#include <unordered_map>

#include "ad_log/ad_log.hpp"
#include "ad_base/common/env.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/message.h"
#include "yaml-cpp/yaml.h"

namespace senseAD {
namespace rscl {
namespace component {

/**
 * @brief Convert proto config into yaml node
 * @param src_proto The proto as input to convert.
 * @param dst_node The yaml node as output from src_proto.
 * @param custom_param custom defined param as input to replace according
 * key/value.
 * @return If the action is successful.
 */
bool ProtoToYaml(
    const google::protobuf::Message& src_proto,
    YAML::Node* dst_node,
    const std::unordered_map<std::string, std::string>& custom_param = {});

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
