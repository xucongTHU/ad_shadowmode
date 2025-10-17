/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhangyi <zhangyi15@senseauto.com>
 */

#pragma once

#include <memory>

#include "ad_msg_bridge/manager/bridge_manager.h"
#include "ad_rscl/comm/external_to_internal_impl/external_subscriber_to_internal_publisher.h"

namespace senseAD {
namespace rscl {
namespace comm {

class ExternalSubscriberToInternalPublisherFactory {
 public:
    template <typename MessageT>
    static std::shared_ptr<ExternalSubscriberToInternalPublisher<MessageT>>
    Create(NodeOwnerBase* const node_owner,
           const CallbackFunc<MessageT>& cb,
           const SubscriberConf& sub_conf,
           const PublisherConf& pub_conf);
};

template <typename MessageT>
std::shared_ptr<ExternalSubscriberToInternalPublisher<MessageT>>
ExternalSubscriberToInternalPublisherFactory::Create(
    NodeOwnerBase* const node_owner,
    const CallbackFunc<MessageT>& cb,
    const SubscriberConf& sub_conf,
    const PublisherConf& pub_conf) {
    if (pub_conf.channel_name().empty()) {
        AD_LERROR(ExternalSubscriberToInternalPublisherFactory)
            << "empty publish channel name";
    }
    if (sub_conf.channel_name().empty()) {
        AD_LERROR(ExternalSubscriberToInternalPublisherFactory)
            << "empty subscribe channel name";
    }
    if (!senseAD::ad_msg_bridge::BridgeCore::Instance()->HasMsgBridge(
            sub_conf.channel_name())) {
        AD_LERROR(ExternalSubscriberToInternalPublisherFactory)
            << "Failed to create ExternalSubscriberToInternalPublisher, no msg "
               "bridge for: "
            << sub_conf.channel_name();
        return nullptr;
    }
    return std::make_shared<ExternalSubscriberToInternalPublisher<MessageT>>(
        node_owner, cb, sub_conf, pub_conf);
}
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
