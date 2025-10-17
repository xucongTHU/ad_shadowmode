/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */
#ifndef AD_MSG_BRIDGE_MANAGER_BRIDGE_MANAGER_H_
#define AD_MSG_BRIDGE_MANAGER_BRIDGE_MANAGER_H_

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <map>

#include <ad_log/ad_log.hpp>

#include "ad_base/class_loader/class_loader_manager.h"
#include "ad_base/common/macros.h"

#include "ad_msg_bridge/manager/bridge_config.h"
#include "ad_msg_bridge/manager/rscl_base.h"

namespace senseAD {
namespace ad_msg_bridge {

class BridgeCore {
 public:
    void Shutdown();
    // SetupCore must be called before any other functions.
    void SetupCore(MsgBridgeConfig conf,
                   const std::string external_topic_prefix = "");
    void SetupCore(const std::string &conf_path,
                   const std::string external_topic_prefix = "");
    ~BridgeCore();

    // CreateExternalPublisher
    // topic_key: topic key is the index in map, not the topic name.
    // use topic_key to find the topic name and msg type.
    // then load the class from dynamic library.
    // call dynamic loaded function create the publisher.
    template <typename MessageT>
    std::shared_ptr<ExternalPublisherBase> CreateExternalPublisher(
        const std::string &topic_key);

    template <typename MessageT, typename ExternalType>
    std::shared_ptr<ExternalPublisherBase> CreateExternalDirectPublisher(
        const std::string &topic_key);

    template <typename MessageT>
    std::shared_ptr<ExternalSubscriberBase> CreateExternalSubscriber(
        const std::string &topic_key, const CallbackFunc<MessageT> &cb);

    template <typename MessageT, typename ExternalType>
    std::shared_ptr<ExternalSubscriberBase> CreateExternalDirectSubscriber(
        const std::string &topic_key,
        const ExternalCallbackFunc<ExternalType> &external_cb);

    bool IsExternalPublisherImplemented(const std::string &topic_key);
    bool IsExternalSubscriberImplemented(const std::string &topic_key);

    /**
     * @brief Check if the topic was set up.
     * @param topic_key: topic key is the index in map, not the topic name.
     *     you can find the topic name and msg type by topic_key.
     */
    inline bool HasMsgBridge(const std::string &topic_key) const {
        return conf_.HasMsgBridge(topic_key);
    }

    void ShutDown();
    std::map<std::string, Topic> GetExternalChannels() const;
    const Topic GetExternalChannel(const std::string &topic_key) const;

    friend std::ostream &operator<<(std::ostream &os, BridgeCore *core);

 private:
    senseAD::base::class_loader::ClassLoaderManager class_loader_manager_;
    MsgBridgeConfig conf_;
    std::string external_topic_prefix_;

    void LoadDynamicLibrary(const std::string &file_path);
    void LoadDynamicLibrarys();

    std::shared_ptr<senseAD::ad_msg_bridge::ExternalCommunicatorBase>
    CreateExternalCommunicatorObjForTopic(const std::string &topic_key);

    DECLARE_AD_SINGLETON(BridgeCore)
};

template <typename MessageT>
std::shared_ptr<ExternalPublisherBase> BridgeCore::CreateExternalPublisher(
    const std::string &topic_key) {
    auto obj = CreateExternalCommunicatorObjForTopic(topic_key);
    if (!obj) {
        return nullptr;
    }
    auto topic = conf_.topics().at(topic_key);
    AD_LDEBUG(BridgeCore) << "CreateExternalPublisher for " +
                                 topic.DebugString();

    const auto &rscl_type = typeid(MessageT);
    return obj->CreateExternalPublisher(topic, &rscl_type);
}

template <>
inline std::shared_ptr<ExternalPublisherBase>
BridgeCore::CreateExternalPublisher<senseAD::mw::BorrowedRawMessage>(
    const std::string &topic_key) {
    auto obj = CreateExternalCommunicatorObjForTopic(topic_key);
    if (!obj) {
        return nullptr;
    }

    auto topic = conf_.topics().at(topic_key);
    AD_LDEBUG(BridgeCore) << "CreateExternalPublisher for " +
                                 topic.DebugString();

    return obj->CreateExternalPublisher(topic);
}

template <typename MessageT, typename ExternalType>
std::shared_ptr<ExternalPublisherBase>
BridgeCore::CreateExternalDirectPublisher(const std::string &topic_key) {
    auto obj = CreateExternalCommunicatorObjForTopic(topic_key);
    if (!obj) {
        return nullptr;
    }
    auto topic = conf_.topics().at(topic_key);

    AD_LDEBUG(BridgeCore) << "CreateExternalPublisher for " +
                                 topic.DebugString();
    const auto &rscl_type = typeid(MessageT);
    const auto &external_type = typeid(ExternalType);
    return obj->CreateExternalPublisher(topic, &rscl_type, &external_type);
}

template <typename MessageT>
std::shared_ptr<ExternalSubscriberBase> BridgeCore::CreateExternalSubscriber(
    const std::string &topic_key, const CallbackFunc<MessageT> &cb) {
    auto obj = CreateExternalCommunicatorObjForTopic(topic_key);
    if (!obj) {
        return nullptr;
    }
    auto topic = conf_.topics().at(topic_key);

    AD_LDEBUG(BridgeCore) << "CreateExternalSubscriber for " +
                                 topic.DebugString();
    const auto &rscl_type = typeid(MessageT);
    auto sub = obj->CreateExternalSubscriber(topic, &rscl_type);
    if (sub) {
        if (!sub->SubscribeRsclType<MessageT>(cb)) {
            AD_LERROR(BridgeCore)
                << "SubscribeRsclType failed for " << topic.DebugString();
            return nullptr;
        }
    }
    return sub;
}

// provide rsclbag record external message
template <>
inline std::shared_ptr<ExternalSubscriberBase>
BridgeCore::CreateExternalSubscriber<senseAD::mw::RawMessage>(
    const std::string &topic_key,
    const CallbackFunc<senseAD::mw::RawMessage> &cb) {
    auto obj = CreateExternalCommunicatorObjForTopic(topic_key);
    if (!obj) {
        return nullptr;
    }
    auto topic = conf_.topics().at(topic_key);

    AD_LDEBUG(BridgeCore) << "CreateExternalSubscriber for " +
                                 topic.DebugString();
    auto sub = obj->CreateExternalSubscriber(topic);
    if (sub) {
        if (!sub->SubscribeRawMessage(cb)) {
            AD_LERROR(BridgeCore)
                << "SubscribeRawMessage failed for " << topic.DebugString();
            return nullptr;
        }
    }

    return sub;
}

template <typename MessageT, typename ExternalType>
std::shared_ptr<ExternalSubscriberBase>
BridgeCore::CreateExternalDirectSubscriber(
    const std::string &topic_key,
    const ExternalCallbackFunc<ExternalType> &external_cb) {
    auto obj = CreateExternalCommunicatorObjForTopic(topic_key);
    if (!obj) {
        return nullptr;
    }
    auto topic = conf_.topics().at(topic_key);

    AD_LDEBUG(BridgeCore) << "CreateExternalSubscriber for " +
                                 topic.DebugString();

    const auto &rscl_type = typeid(MessageT);
    const auto &external_type = typeid(ExternalType);
    auto sub = obj->CreateExternalSubscriber(topic, &rscl_type, &external_type);

    if (sub) {
        if (!sub->SubscribeExternalType<ExternalType>(external_cb)) {
            AD_LERROR(BridgeCore)
                << "SubscribeExternalType failed for " << topic.DebugString();
            return nullptr;
        }
    }

    return sub;
}

}  // namespace ad_msg_bridge
}  // namespace senseAD

#endif /* AD_MSG_BRIDGE_MANAGER_BRIDGE_MANAGER_H_ */
