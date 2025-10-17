/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>

#include "ad_diag/base/supervisions/common.h"
#include "ad_diag/base/supervisions/supervision.h"

namespace senseAD {
namespace diag {

struct FunctionCost {
    bool is_key_function = false;
    std::string function_name;
    float cost;
    DEFINE_SAVE_LOAD_FN(is_key_function, function_name, cost)
};

struct ComponentState {
    std::string platform;
    std::string name;
    int status;
    int pid;
    std::vector<FunctionCost> function_cost;

    DEFINE_SAVE_LOAD_FN(platform, name, status, pid, function_cost)
};

struct ComponentReport {
    SupervisionReport base;
    ComponentState state;

    DEFINE_SAVE_LOAD_FN(state)
};

class ComponentSupervision : public SupervisionBase {
 public:
    ComponentSupervision();
    ~ComponentSupervision() override;

    void Report(const ComponentState& state);

    constexpr static SupervisionCppType Type = SupervisionCppType::COMPONENT;

 private:
};

typedef SupervisionEndpoint<ComponentReport, ComponentSupervision::Type>
    ComponentSupervisionEndpoint;

}  // namespace diag
}  // namespace senseAD
