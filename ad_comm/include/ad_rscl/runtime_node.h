/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <atomic>
#include <unordered_set>

#include "ad_rscl/runtime.h"

namespace senseAD {

namespace rscl {
namespace comm {
class Node;
class NodeImpl;
}  // namespace comm
using senseAD::rscl::comm::Node;
using senseAD::rscl::comm::NodeImpl;
using senseAD::rscl::common::Suspendable;

namespace runtime {

class NodeManagement {
 public:
    std::unique_ptr<Node> CreateNode(const std::string &node_name,
                                     const std::string &node_namespace = "");

    void RegisterGlobalSuspendable(Suspendable *instance);
    void UnRegisterGlobalSuspendable(Suspendable *instance);
    void RegisterComponent(Suspendable *instance);
    void UnRegisterComponent(Suspendable *instance);

    bool Suspend();
    bool Resume();

 private:
    // Node Management Related
    std::vector<std::shared_ptr<NodeImpl>> GetAliveNodes();

    std::atomic<bool> suspended_{false};

    std::mutex suspendable_mtx_;
    std::unordered_set<Suspendable *> suspendable_logics_;
    std::unordered_set<Suspendable *> suspendable_components_;

    std::mutex node_mgn_mtx_;
    std::list<std::weak_ptr<NodeImpl>> node_impl_wptrs_;
};

}  // namespace runtime
}  // namespace rscl
}  // namespace senseAD
