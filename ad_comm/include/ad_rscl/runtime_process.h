/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ad_rscl/runtime.h"
#include "ad_rscl/idl/process_state.capnp.h"

namespace senseAD {
namespace rscl {

namespace comm {
class Node;
class PublisherBase;
}  // namespace comm
using senseAD::rscl::comm::Node;
using senseAD::rscl::comm::PublisherBase;

namespace runtime {

class ProcessManagement {
 public:
    ProcessManagement();

    void EnableRtStateBoardcast(Node* rt_node);
    void DisableRtStateBoardcast();
    /**
     * @brief Boardcast Runtime State
     * !IMPORTANT: STATE_UNINITIALIZED and STATE_SHUTDOWN
     * will not be handle due to implementation detail
     */
    void HandleStateUpdate(const State& state);

 private:
    void PubState(const senseAD::rscl::idl::ProcessStateInfo::State& state);

    Node* rt_node_;
    std::shared_ptr<PublisherBase> rt_state_pub_;

    uint32_t pid_;
    std::string hostname_;
    std::unordered_map<int, std::string> state_str_map_;
};

}  // namespace runtime
}  // namespace rscl
}  // namespace senseAD
