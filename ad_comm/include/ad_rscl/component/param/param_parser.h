/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <unordered_map>

#include "ad_rscl/common/file.h"
#include "ad_rscl/component/param/param_utils.h"
#include "ad_log/ad_log.hpp"
#include "google/protobuf/any.h"
#include "google/protobuf/any.pb.h"

namespace senseAD {
namespace rscl {
namespace component {

class GeneralParamParser {
 public:
    GeneralParamParser() = default;
    GeneralParamParser(const GeneralParamParser&) = delete;
    GeneralParamParser& operator=(const GeneralParamParser&) = delete;

    void InitParser(
        const std::unordered_map<std::string, std::string>& custom_param) {
        custom_param_ptr_ = &custom_param;
    }

    bool ParseAllConf(YAML::Node* all_config_node, const bool allow_ext = true);

    bool LoadOfflineConfig(YAML::Node* all_config_node,
                           const std::string& offline_config_path = "");

 private:
    bool LoadArgAndEnv(YAML::Node* all_config_node);

    bool CustomOverwrite(YAML::Node* all_config_node);

    const std::unordered_map<std::string, std::string>* custom_param_ptr_ =
        nullptr;
};

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
