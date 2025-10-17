/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <unistd.h>

#include <thread>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <unordered_map>

#include "ad_base/time/time.h"
#include "ad_base/common/macros.h"
#include "ad_base/common/rpc.h"
#include "ad_service_discovery/polyfill_service_discovery/common.h"
#include "ad_service_discovery/polyfill_transport/transport_impl.h"
#include "ad_service_discovery/polyfill_service_discovery/request_response.h"
#include "ad_service_discovery/service_discovery/types.h"

namespace senseAD {
namespace service_discovery {

// Since service discovery depend in polyfill core, try to start polyfill core
constexpr char kExpectedPolyfillCorePidPath[]{"/tmp/rscl_polyfill_core.pid"};
void TryAutoForkPolyfillCore();
void TryKillPolyfillCore();

class SDServiceClient {
 public:
    using Token = std::string;
    using ListenerToken = uint32_t;

    ~SDServiceClient();
    bool NotifyEvent(const SrvDiscEvent& event);
    bool GetDiscoveryMeta(const GetDiscoveryMetaRequest& req,
                          std::vector<SrvDiscEvent>* metas);

    /* node manager */
    inline bool AddNode(const std::string& node_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_JOIN)
                .SetIdentity(SrvDiscRoleAttr::Identity::NODE)
                .SetNodeName(node_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }
    inline bool LeaveNode(const std::string& node_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_EXIT)
                .SetIdentity(SrvDiscRoleAttr::Identity::NODE)
                .SetNodeName(node_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    /* channel manager */
    inline bool AddWriter(const std::string& node_name,
                          const std::string& channel_name,
                          const std::string& msg_type,
                          const std::string& msg_desc) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_JOIN)
                .SetIdentity(SrvDiscRoleAttr::Identity::WRITER)
                .SetNodeName(node_name)
                .SetChannelName(channel_name)
                .SetMsgType(msg_type)
                .SetMsgDesc(msg_desc)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    inline bool LeaveWriter(const std::string& node_name,
                            const std::string& channel_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_EXIT)
                .SetIdentity(SrvDiscRoleAttr::Identity::WRITER)
                .SetNodeName(node_name)
                .SetChannelName(channel_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    inline bool AddReader(const std::string& node_name,
                          const std::string& channel_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_JOIN)
                .SetIdentity(SrvDiscRoleAttr::Identity::READER)
                .SetNodeName(node_name)
                .SetChannelName(channel_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    inline bool LeaveReader(const std::string& node_name,
                            const std::string& channel_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_EXIT)
                .SetIdentity(SrvDiscRoleAttr::Identity::READER)
                .SetNodeName(node_name)
                .SetChannelName(channel_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    /* service manager */
    inline bool AddService(const std::string& node_name,
                           const std::string& service_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_JOIN)
                .SetIdentity(SrvDiscRoleAttr::Identity::SERVER)
                .SetNodeName(node_name)
                .SetServiceName(service_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    inline bool LeaveService(const std::string& node_name,
                             const std::string& service_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_EXIT)
                .SetIdentity(SrvDiscRoleAttr::Identity::SERVER)
                .SetNodeName(node_name)
                .SetServiceName(service_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    inline bool AddClient(const std::string& node_name,
                          const std::string& service_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_JOIN)
                .SetIdentity(SrvDiscRoleAttr::Identity::CLIENT)
                .SetNodeName(node_name)
                .SetServiceName(service_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    inline bool LeaveClient(const std::string& node_name,
                            const std::string& service_name) {
        return NotifyEvent(
            SrvDiscEvent()
                .SetAction(SrvDiscEvent::Action::EVENT_EXIT)
                .SetIdentity(SrvDiscRoleAttr::Identity::CLIENT)
                .SetNodeName(node_name)
                .SetServiceName(service_name)
                .SetTimestamp(static_cast<uint64_t>(
                    senseAD::base::time::Time::Now().ToSecond())));
    }

    /* notifier handler */

    Token RegNotifierHandler(const std::string& node_name,
                             std::function<void()> handler);
    void UnRegNotifierHandler(const Token& token);

    ListenerToken AddListener(const SrvDiscListenerCallback& cb,
                              SrvDiscEvent::Action target_action,
                              SrvDiscRoleAttr::Identity identity);
    void RemoveListener(const ListenerToken& token);
    void RemoveAllListener();
    void ShutDown();

 private:
    void InitOnce();

    // bool NotifyPolyfillCore(const NotifyToSubscribeRequest& req);
    // bool NotifyToSubscribe(const std::string& service_name);
    // bool NotifyToLeaveSubscribe(const std::string& service_name);

    std::chrono::milliseconds GetPunishTimeoutMs(uint32_t timeout_ms);
    bool WrapRpcIO(const transport::RequestType& input,
                   ResponseWrapper* output,
                   RequestCommonType type);

    bool inited_ = false;
    std::once_flag inited_flag_;
    std::unique_ptr<senseAD::base::common::RpcClient> sd_client_;

    std::mutex query_mutux_;
    std::mutex reg_mutux_;
    std::mutex event_mtx_;
    // std::unordered_map<Token, std::function<void()>> notifier_handlers_;

    std::unordered_map<std::string, SrvDiscEvent> at_exit_events_;

    std::mutex listener_mutux_;
    std::string pre_notify_msg_;
    ListenerToken listen_token_ = 0;
    std::unordered_map<ListenerToken,
                       std::shared_ptr<senseAD::transport::Reporter>>
        listeners_;
    uint32_t punish_decrease_ms_ = 0;

    // for polyfill_core reconnect.
    std::unordered_map<std::string, SrvDiscEvent> history_events_;
    std::shared_ptr<transport::Subscriber> notify_sub_;
    bool server_reconnecting_ = false;
    int last_ping_tsec_ = 0;
    static const int kLostConnectionTimeoutSec = 3;

    std::shared_ptr<void> log_mgr_ref_;
    std::shared_ptr<void> time_mgr_ref_;

    DECLARE_AD_SINGLETON(SDServiceClient)
};

}  // namespace service_discovery
}  // namespace senseAD
