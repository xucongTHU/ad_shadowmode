/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <unordered_map>

#include "ad_base/common/shutdownable.h"
#include "ad_diag/base/supervisions/supervision_component.h"
#include "ad_rscl/idl/runtime_state.capnp.h"
#include "ad_rscl/comm/node.h"
#include "ad_rscl/common/feature_manager.h"
#include "ad_rscl/task/task.h"

namespace senseAD {
namespace rscl {
namespace component {

using senseAD::rscl::idl::ComponentStateInfo;
using senseAD::rscl::idl::NodeStateCtrlRequest;
using senseAD::rscl::task::TimerTask;

class ComponentBase;

/**
 * @brief Implement the status upload and communicate with the monitor
 *
 */
class RuntimeStateCliImpl : public ::Shutdownable {
 public:
    explicit RuntimeStateCliImpl(ComponentBase *c);

    ~RuntimeStateCliImpl() override;

    /**
     * @brief Call after the node created
     *
     */
    void InitializeOnce();

    /**
     * @brief Call before destruction
     *
     */
    void ShutdownImpl() override;

    /**
     * @brief Record the time cost for specific function
     *
     * @param func_name Name of the function
     * @param time_cost_ns Time cost in nanosecond
     */
    void PushFunctionCost(const std::string &func_name,
                          const uint64_t &time_cost_ns);
    /**
     * @brief Record the estimated time cost for the component
     *
     * @param time_cost_ns
     */
    inline void UpdateComponentCost(const uint64_t &time_cost_ns) {
        node_cost_ns_ = time_cost_ns;
    }

    /**
     * @brief Upload status to monitor
     *
     */
    void SendComponentStatus();

 private:
    /**
     * @brief Handle the monitor request
     *
     */
    void OnNodeStateCtrlRequest(NodeStateCtrlRequest::Reader);

    void SendComponentSupervisionMsg(
        const std::vector<diag::FunctionCost> &cost);

    ComponentBase *c_;

    std::mutex function_cost_mutex_;
    std::unordered_map<std::string, float> function_cost_;
    senseAD::rscl::comm::PublisherBase::Ptr state_pub_;
    senseAD::rscl::comm::SubscriberBase::Ptr monitor_control_sub_;
    std::unique_ptr<TimerTask> status_timer_;

    std::string node_name_;
    uint64_t pid_ = 0;
    uint64_t node_cost_ns_ = 0;
    senseAD::diag::ComponentSupervision supervision_;
};

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
