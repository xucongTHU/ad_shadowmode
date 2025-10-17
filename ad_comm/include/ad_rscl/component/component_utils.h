/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>

#include "ad_rscl/component/component.h"
#include "ad_rscl/time/time.h"

namespace senseAD {
namespace rscl {
namespace component {

/**
 * @brief Report the function time cost to monitor for the caller function
 *
 */
#define PUSH_THIS_FUNCTION_COST(cost) \
    PushFunctionCost(std::string(__func__), cost);

class FunctionCostRaii {
 public:
    FunctionCostRaii(ComponentBase *c, const std::string &name)
        : c_(c), name_(name) {
        start_ns_ = senseAD::rscl::time::Time::SteadyNow().ToNanosecond();
    }
    ~FunctionCostRaii() {
        c_->PushFunctionCost(
            name_,
            senseAD::rscl::time::Time::SteadyNow().ToNanosecond() - start_ns_);
    }

 private:
    ComponentBase *c_;
    std::string name_;
    uint64_t start_ns_;
};

class ComponentCostRaii {
 public:
    explicit ComponentCostRaii(ComponentBase *c) : c_(c) {
        start_ns_ = senseAD::rscl::time::Time::SteadyNow().ToNanosecond();
    }
    ~ComponentCostRaii() {
        c_->UpdateComponentCost(
            senseAD::rscl::time::Time::SteadyNow().ToNanosecond() - start_ns_);
    }

 private:
    ComponentBase *c_;
    uint64_t start_ns_;
};

/**
 * @brief Report the function time cost with auto time cost measure to monitor
 * for the caller function
 *
 */
#define PUSH_THIS_FUNCTION_COST_AUTO()                               \
    senseAD::rscl::component::FunctionCostRaii fcost_raii##__LINE__( \
        this, std::string(__func__));

// Since we cannot sure the onProc is the core function. Need to let user to
// determine
#define PUSH_COMPONENT_COST_AUTO() \
    senseAD::rscl::component::ComponentCostRaii ccost_raii##__LINE__(this);

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
