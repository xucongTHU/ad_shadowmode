/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <unordered_map>
#include <atomic>

#include "ad_base/common/shutdownable.h"
#include "ad_service_discovery/service_discovery/types.h"

namespace senseAD {
namespace service_discovery {

/**
 * @brief Service Discovery API
 *
 */
class ServiceDiscoveryPolyfillImpl : public ::Shutdownable {
 public:
    using ListenerToken = uint32_t;

    ServiceDiscoveryPolyfillImpl();
    ~ServiceDiscoveryPolyfillImpl();
    void ShutdownImpl() override;

    std::unordered_map<std::string, std::vector<SrvDiscEvent>> GetTopicInfo(
        const SrvDiscEvent& event_filter = SrvDiscEvent());

    std::vector<std::string> GetUpstreamNodebyTopic(
        const std::string& topic_name);

    std::vector<std::string> GetDownstreamNodebyTopic(
        const std::string& topic_name);

    std::vector<std::string> GetNodes();

    std::string GetNodeAttr(const std::string& node_name);

    std::vector<std::string> GetSubsofNode(const std::string& node_name);

    std::vector<std::string> GetPubsofNode(const std::string& node_name);

    std::vector<std::string> GetServices();

    std::string GetServiceAttr(const std::string& service_name);

    ListenerToken addNodeActionListener(
        const SrvDiscListenerCallback& cb,
        SrvDiscEvent::Action target_action = SrvDiscEvent::Action::NONE);
    ListenerToken addSubActionListener(
        const SrvDiscListenerCallback& cb,
        SrvDiscEvent::Action target_action = SrvDiscEvent::Action::NONE);
    ListenerToken addPubActionListener(
        const SrvDiscListenerCallback& cb,
        SrvDiscEvent::Action target_action = SrvDiscEvent::Action::NONE);
    ListenerToken addSrvActionListener(
        const SrvDiscListenerCallback& cb,
        SrvDiscEvent::Action target_action = SrvDiscEvent::Action::NONE);

    void RemoveListener(const ListenerToken& token);

    void RemoveAllListener();

 private:
    std::shared_ptr<void> impl_;
};

}  // namespace service_discovery
}  // namespace senseAD
