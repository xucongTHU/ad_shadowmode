/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "ad_base/std/optional.h"

#include "ad_diag/manifest/schema/comm_manifest.h"

namespace YAML {
class Node;
}

namespace senseAD {
namespace diag {
namespace schema {

constexpr char kDiagCfgExtension[] = ".diagcfg.yaml";

// Expect the dependency grep will not have cycle
struct SupervisionDependency {
    // name of the upstream supervision
    std::vector<std::string> depend_on;
};

struct RecoveryCondition {
    // Expect in period [now - check_time_period_sec , now]
    // exist and only exist healthy result
    double check_time_period_sec;
};

struct SupervisionItem {
    // unique name
    std::string name;
    // supervision check type
    std::string cpp_type;

    // map to the main class in the product design
    base::optional<std::string> type_l1;
    base::optional<std::string> description;

    // custom parameter. differrent type may have different param
    std::shared_ptr<YAML::Node> param;
    base::optional<SupervisionDependency> dependency;
    // supervision group
    base::optional<std::string> group;
    // important level
    base::optional<std::string> level;

    base::optional<RecoveryCondition> recovery_condition;
};

struct ProcessManifest {
    std::string process_name = {};
    std::string platform = {};
    std::string identifier{};
    std::vector<SupervisionItem> swc_supervisions{};
    std::vector<CommSupervisionManifest> comm_supervisions{};

    static ProcessManifest LoadFromYamlNode(const YAML::Node &node);
};

}  // namespace schema
}  // namespace diag
}  // namespace senseAD
