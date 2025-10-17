/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Zhang Yi <zhangyi15@senseauto.com>
 */

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "yaml-cpp/yaml.h"

#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace rscl {
namespace component {

class ExtParamParser {
 public:
    ExtParamParser();
    ~ExtParamParser() = default;

    bool LoadExtYamlConfig(YAML::Node* node);

 private:
    std::unordered_map<std::string,
                       std::function<bool(const YAML::Node&, YAML::Node)>>
        parse_map_;

    std::unordered_set<std::string> special_keys_;
};

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
