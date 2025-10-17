/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "yaml-cpp/yaml.h"

#include "ad_rscl/idl/component_conf.pb.h"

namespace senseAD {
namespace rscl {
namespace component {

using senseAD::rscl::idl::SyncedComponentConfig;
using senseAD::rscl::idl::TimerComponentConfig;

struct ComponentConfig {
    // config parameter for individual module
    YAML::Node config_param;

    // rscl_param.yaml/ros_param.yaml in replay data
    std::string offline_config_path;

    SyncedComponentConfig synced_conf;
    TimerComponentConfig timer_conf;
};

std::ostream &operator<<(std::ostream &out, ComponentConfig const &self);

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
