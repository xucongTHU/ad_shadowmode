/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ad_mw/comm.h"

#include "ad_msg_bridge/manager/bridge_manager.h"
#include "ad_msg_bridge/external_msg_comm/external_comm_base.h"
#include "ad_msg_bridge/manager/rscl_base.h"

namespace senseAD {
namespace ad_msg_bridge {

class ExternalCommRegsiter {
 public:
    // ! Since typename depend in typeid(T).name.
    // ! Not support using different compiler
    template <typename MessageT>
    std::shared_ptr<ExternalPublisherBase> CreateExternalPublisher(
        const std::string &topic_key);
    template <typename RsclType, typename ExternalType>
    std::shared_ptr<ExternalPublisherBase> CreateExternalDirectPublisher(
        const std::string &topic_key);
    template <typename MessageT>
    std::shared_ptr<ExternalSubscriberBase> CreateExternalSubscriber(
        const std::string &topic_key,
        const CallbackFunc<MessageT> &cb = nullptr);
    template <typename RsclType, typename ExternalType>
    std::shared_ptr<ExternalSubscriberBase> CreateExternalDirectSubscriber(
        const std::string &topic_key,
        const ExternalCallbackFunc<ExternalType> &external_cb = nullptr);

 private:
    BridgeCore *bridge_core_;
    std::vector<std::shared_ptr<senseAD::mw::comm::SubscriberBase>>
        external_subscribers_;

    DECLARE_AD_SINGLETON(ExternalCommRegsiter)
};

template <typename MessageT>
std::shared_ptr<ExternalPublisherBase>
ExternalCommRegsiter::CreateExternalPublisher(const std::string &topic_key) {
    return bridge_core_->CreateExternalPublisher<MessageT>(topic_key);
}

template <typename RsclType, typename ExternalType>
std::shared_ptr<ExternalPublisherBase>
ExternalCommRegsiter::CreateExternalDirectPublisher(
    const std::string &topic_key) {
    return bridge_core_->CreateExternalDirectPublisher<RsclType, ExternalType>(
        topic_key);
}

template <typename MessageT>
std::shared_ptr<ExternalSubscriberBase>
ExternalCommRegsiter::CreateExternalSubscriber(
    const std::string &topic_key, const CallbackFunc<MessageT> &cb) {
    return bridge_core_->CreateExternalSubscriber<MessageT>(topic_key, cb);
}

template <typename RsclType, typename ExternalType>
std::shared_ptr<ExternalSubscriberBase>
ExternalCommRegsiter::CreateExternalDirectSubscriber(
    const std::string &topic_key,
    const ExternalCallbackFunc<ExternalType> &external_cb) {
    return bridge_core_->CreateExternalDirectSubscriber<RsclType, ExternalType>(
        topic_key, external_cb);
}

}  // namespace ad_msg_bridge
}  // namespace senseAD
