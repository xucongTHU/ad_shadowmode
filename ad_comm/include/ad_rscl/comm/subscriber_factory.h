/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "ad_rscl/exception.h"
#include "ad_rscl/comm/subscriber_base.h"
#include "ad_rscl/comm/internal_impl/internal_subscriber.h"
#include "ad_rscl/comm/external_impl/external_subscriber.h"
#include "ad_rscl/comm/external_impl/external_direct_subscriber.h"
#include "ad_msg_bridge/manager/bridge_manager.h"
#include "ad_serde/ad_serde.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename RsclType, typename ExternalType>
using SubscriberGateway = ExternalDirectSubscriber<RsclType, ExternalType>;

/**
 * @brief Create an subscriber instance according to the conf or global setting
 *
 */
class SubscriberFactory {
 public:
    template <typename MessageT>
    static std::shared_ptr<Subscriber<MessageT>> Create(
        NodeOwnerBase* const node_owner,
        const CallbackFunc<MessageT>& cb,
        const SubscriberConf& conf);
    /**
     * @brief Create a subscriber according to config.
     *
     * @tparam MessageT
     * @param node_owner
     * @param cb
     * @param external_cb
     * @param conf
     * @return std::shared_ptr<Subscriber<MessageT>>
     */
    template <typename RsclType, typename ExternalType>
    static std::shared_ptr<SubscriberGateway<RsclType, ExternalType>> CreateGW(
        NodeOwnerBase* const node_owner,
        const ExternalCallbackFunc<ExternalType>& external_cb,
        const CallbackFunc<RsclType>& cb,
        const SubscriberConf& conf);
};

/**
 * @brief Create an subscriber instance according to the subscriber conf.
 *        If topic key is already configured in the conf, it will load the
 *        external topic config and create an external subscriber.
 *        Otherwise, it will create an internal subscriber.
 *
 * @tparam MessageT
 * @param node_owner The node owner of the subscriber
 * @param cb callback function when message reach
 * @param conf The subscriber conf
 * @return std::shared_ptr<subscriber<MessageT>> The subscriber instance
 */
template <typename MessageT>
std::shared_ptr<Subscriber<MessageT>> SubscriberFactory::Create(
    NodeOwnerBase* const node_owner,
    const CallbackFunc<MessageT>& cb,
    const SubscriberConf& conf) {
    if (conf.channel_name().empty()) {
        AD_LERROR(SubscriberFactory)
            << "Empty subscriber channel name. Cannot create subscriber";
        return nullptr;
    }
    AD_LINFO(SubscriberFactory)
        << "Trying to create subscriber topic:" << conf.channel_name()
        << " with msg type:"
        << senseAD::serde::MessageTypeMeta<MessageT>::GetMessageTypename();
    // create external subscriber
    if (senseAD::ad_msg_bridge::BridgeCore::Instance()->HasMsgBridge(
            conf.channel_name())) {
        return std::make_shared<ExternalSubscriber<MessageT>>(node_owner, cb,
                                                              conf);
    }
    // create internal subscriber
    return std::make_shared<InternalSubscriber<MessageT>>(node_owner, cb, conf);
}

template <typename RsclType, typename ExternalType>
std::shared_ptr<SubscriberGateway<RsclType, ExternalType>>
SubscriberFactory::CreateGW(
    NodeOwnerBase* const node_owner,
    const ExternalCallbackFunc<ExternalType>& external_cb,
    const CallbackFunc<RsclType>& cb,
    const SubscriberConf& conf) {
    if (conf.channel_name().empty()) {
        AD_LERROR(SubscriberFactory)
            << "Empty subscriber channel name. Cannot create subscriber";
        return nullptr;
    }

    // create external subscriber
    bool bridge_config_exist =
        senseAD::ad_msg_bridge::BridgeCore::Instance()->HasMsgBridge(
            conf.channel_name());

    if (!bridge_config_exist) {
        if (!cb || !conf.internal_compat()) {
            std::stringstream ss;
            ss << "[" << conf.channel_name()
               << "] has NO msg bridge config while not allow rscl "
               << "type callback. IT MUST BE A PROGRAMMING ERROR.";
            std::string err = ss.str();
            AD_LFATAL(SubscriberFactory) << err;
            throw MsgBridgeConfigMissingException(err);
        } else {
            AD_LWARN(SubscriberFactory) << "[" << conf.channel_name()
                                        << "] has NO msg bridge config. will "
                                           "only enable the rscl type callback";
            return std::make_shared<
                ExternalDirectSubscriber<RsclType, ExternalType>>(
                node_owner, nullptr, cb, conf);
        }
    }

    return std::make_shared<ExternalDirectSubscriber<RsclType, ExternalType>>(
        node_owner, external_cb, cb, conf);
}
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
