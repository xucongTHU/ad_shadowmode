/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>

#include "ad_base/std/optional.h"

namespace YAML {
class Node;
}

namespace senseAD {
namespace diag {
namespace schema {

struct CommSupervisionManifest {
    // Default as: process name + '#' + topic name
    // std::string name;
    base::optional<std::string> description;

    // supervision group
    base::optional<std::string> group;
    // important level
    base::optional<std::string> level;

    std::string topic_name;

    base::optional<float> min_hz;
    base::optional<float> max_hz;
    base::optional<float> hints_hz;
    base::optional<float> warning_hz;

    base::optional<double> max_avg_size_kb;

    base::optional<bool> is_key_frame;

    base::optional<float> min_gap_ns;
    base::optional<float> max_gap_ns;
    base::optional<float> hints_gap_ns;
    base::optional<float> warning_gap_ns;

    static CommSupervisionManifest LoadFromYamlNode(const YAML::Node &node);
};

}  // namespace schema
}  // namespace diag
}  // namespace senseAD
