/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <mutex>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <list>
#include <unordered_set>
#include <unordered_map>

#include "ad_base/common/macros.h"
#include "ad_rscl/exception.h"
#include "ad_service_discovery/service_discovery/service_discovery.h"
#include "ad_serde/msgmeta.h"
#include "ad_rscl/comm/node_impl.h"
#include "ad_serde/ad_serde.h"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::service_discovery::SrvDiscEvent;
using SrvDiscEventAction = senseAD::service_discovery::SrvDiscEvent::Action;

class NodeImpl;
class SubscriberBase;
class MsgTypeVerifierManager : public ::Shutdownable {
 public:
    ~MsgTypeVerifierManager();
    void ShutdownImpl() override;
    void AddObserveNode(NodeImpl*);
    void RemoveNode(NodeImpl*);
    DECLARE_AD_SINGLETON(MsgTypeVerifierManager)

 private:
    std::mutex node_mutex_;
    std::unordered_set<NodeImpl*> observe_nodes_;
    // key: sub pointer, value: if descriptor check pass
    std::unordered_map<uint64_t, bool> sub_checkpass_;
    // key: channel name, value: meta
    std::unordered_map<std::string, senseAD::serde::MsgMeta> topic_metas_;
    std::list<std::shared_ptr<SrvDiscEvent>> change_msg_list_;
    std::mutex change_mutex_;
    std::once_flag reg_listener_flag_;

    bool is_running_;
    std::thread verifier_thread_;

    // @brief check the msg type between pub and sub.
    void StartVerifier();

    bool CheckSubSptrValid(const std::shared_ptr<SubscriberBase>&);
    bool CheckSubscriberDescriptor(const std::shared_ptr<SubscriberBase>&);
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
