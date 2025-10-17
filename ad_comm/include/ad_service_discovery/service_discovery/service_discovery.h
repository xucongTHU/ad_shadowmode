/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "ad_base/common/macros.h"
#include "ad_serde/msgmeta.h"
#include "ad_service_discovery/service_discovery/types.h"
#include "ad_msg_bridge/manager/bridge_manager.h"

#include "ad_mw/types.h"
#include "ad_service_discovery/polyfill_service_discovery/service_discovery_polyfill_impl.h"

using senseAD::mw::RawMessage;

namespace senseAD {
namespace comm {
class NodeImpl;
}  // namespace comm
namespace service_discovery {

using SrvDiscEventAction = SrvDiscEvent::Action;

class ServiceDiscovery {
 public:
    using ListenerToken = uint32_t;

    /**
     * @brief key is the channel names.
     * value is the list of participant of its channel
     *
     */
    using RoleAttrPerTopic =
        std::unordered_map<std::string, std::vector<SrvDiscEvent>>;

    ~ServiceDiscovery();

    void Shutdown();

    /**
     * @brief Get the list of participant of all existing channel
     *
     * @return RoleAttrPerTopic
     */
    inline RoleAttrPerTopic GetTopicInfo(
        const SrvDiscEvent& eventFilter = SrvDiscEvent()) const {
        RoleAttrPerTopic ret = impl_->GetTopicInfo(eventFilter);
        return ret;
    }

    bool isInited() const;

    /**
     * @brief Get message meta information for target channel
     * Those information can be used for reflection
     *
     * @param topic_name channel name
     * @param val message meta information
     * @return whether the call success
     */
    bool GetMsgMetabyTopic(const std::string& topic_name,
                           senseAD::serde::MsgMeta* val) const;

    /**
     * @brief Get the message provider node name for target channel
     *
     * @param topic_name
     * @return std::vector<std::string>
     */
    inline std::vector<std::string> GetUpstreamNodebyTopic(
        const std::string& topic_name) const {
        return impl_->GetUpstreamNodebyTopic(topic_name);
    }

    /**
     * @brief Get the message receiver node name for target channel
     *
     * @param topic_name
     * @return std::vector<std::string>
     */
    inline std::vector<std::string> GetDownstreamNodebyTopic(
        const std::string& topic_name) const {
        return impl_->GetDownstreamNodebyTopic(topic_name);
    }

    inline std::vector<std::string> GetNodes() const {
        return impl_->GetNodes();
    }

    inline std::string GetNodeAttr(const std::string& node_name) const {
        return impl_->GetNodeAttr(node_name);
    }

    inline std::vector<std::string> GetSubsofNode(
        const std::string& node_name) const {
        return impl_->GetSubsofNode(node_name);
    }

    inline std::vector<std::string> GetPubsofNode(
        const std::string& node_name) const {
        return impl_->GetPubsofNode(node_name);
    }

    inline std::vector<std::string> GetServices() const {
        return impl_->GetServices();
    }

    inline std::string GetServiceAttr(const std::string& service_name) const {
        return impl_->GetServiceAttr(service_name);
    }

    inline ListenerToken addNodeActionListener(
        const SrvDiscListenerCallback& cb, SrvDiscEventAction action) {
        return impl_->addNodeActionListener(cb, action);
    }

    inline ListenerToken addSubActionListener(const SrvDiscListenerCallback& cb,
                                              SrvDiscEventAction action) {
        return impl_->addSubActionListener(cb, action);
    }

    inline ListenerToken addPubActionListener(const SrvDiscListenerCallback& cb,
                                              SrvDiscEventAction action) {
        return impl_->addPubActionListener(cb, action);
    }

    inline ListenerToken addSrvActionListener(const SrvDiscListenerCallback& cb,
                                              SrvDiscEventAction action) {
        return impl_->addSrvActionListener(cb, action);
    }

    inline void RemoveListener(const ListenerToken& token) {
        impl_->RemoveListener(token);
    }

    inline void RemoveAllListener() { impl_->RemoveAllListener(); }

 private:
    std::unique_ptr<senseAD::service_discovery::ServiceDiscoveryPolyfillImpl>
        impl_;
    std::shared_ptr<senseAD::ad_msg_bridge::BridgeCore> msg_bridge_core_ =
        nullptr;

    DECLARE_AD_SINGLETON(ServiceDiscovery)
};

}  // namespace service_discovery
}  // namespace senseAD
