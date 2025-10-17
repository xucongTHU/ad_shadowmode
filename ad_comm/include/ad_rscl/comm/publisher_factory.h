/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <vector>
#include <string>

#include "ad_rscl/comm/publisher_base.h"
#include "ad_rscl/comm/external_impl/external_publisher.h"
#include "ad_rscl/comm/external_impl/external_direct_publisher.h"
#include "ad_rscl/comm/internal_impl/internal_publisher.h"
#include "ad_msg_bridge/manager/bridge_manager.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename RsclType, typename ExternalType>
using PublisherGateway = ExternalDirectPublisher<RsclType, ExternalType>;

/**
 * @brief Create an publisher instance according to the conf or global setting
 *
 */
class PublisherFactory {
 public:
    template <typename MessageT>
    static std::shared_ptr<Publisher<MessageT>> Create(
        NodeOwnerBase* const node_owner, const PublisherConf& conf);

    template <typename RsclType, typename ExternalType>
    static std::shared_ptr<PublisherGateway<RsclType, ExternalType>> CreateGW(
        NodeOwnerBase* const node_owner, const PublisherConf& conf);
};

/**
 * @brief Create an publisher instance according to the publisher conf.
 *        If topic key is already configured in the conf, it will load the
 *        external topic config and create an external publisher.
 *        Otherwise, it will create an internal publisher.
 *
 * @tparam MessageT
 * @param node_owner The node owner of the publisher
 * @param conf The publisher conf
 * @return std::shared_ptr<Publisher<MessageT>> The publisher instance
 */
template <typename MessageT>
std::shared_ptr<Publisher<MessageT>> PublisherFactory::Create(
    NodeOwnerBase* const node_owner, const PublisherConf& conf) {
    if (conf.channel_name().empty()) {
        AD_LERROR(PublisherFactory)
            << "Empty publisher channel name. Cannot create publisher";
        return nullptr;
    }
    AD_LINFO(PublisherFactory)
        << "Trying to create publisher topic:" << conf.channel_name()
        << " with msg type:" << conf.msg_type();
    // create external publisher
    if (senseAD::ad_msg_bridge::BridgeCore::Instance()->HasMsgBridge(
            conf.channel_name())) {
        return std::make_shared<ExternalPublisher<MessageT>>(node_owner, conf);
    }
    // create internal publisher
    return std::make_shared<InternalPublisher<MessageT>>(node_owner, conf);
}

template <typename RsclType, typename ExternalType>
std::shared_ptr<PublisherGateway<RsclType, ExternalType>>
PublisherFactory::CreateGW(NodeOwnerBase* const node_owner,
                           const PublisherConf& conf) {
    if (conf.channel_name().empty()) {
        AD_LERROR(PublisherFactory)
            << "Empty publisher channel name. Cannot create publisher";
        return nullptr;
    }
    // create external publisher
    bool bridge_config_exist =
        senseAD::ad_msg_bridge::BridgeCore::Instance()->HasMsgBridge(
            conf.channel_name());

    if (!bridge_config_exist && !conf.internal_compat()) {
        std::stringstream ss;
        ss << "[" << conf.channel_name() << "] has NO msg bridge config while "
           << "internal_compat = false. IT MUST BE A PROGRAMMING ERROR.";
        std::string err = ss.str();
        AD_LFATAL(PublisherFactory) << err;
        throw MsgBridgeConfigMissingException(err);
    }

    return std::make_shared<ExternalDirectPublisher<RsclType, ExternalType>>(
        node_owner, conf);
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
