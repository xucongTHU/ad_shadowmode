/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>

 * @file synced_component.h
 * @brief Provide a message driven component wrapper class.
 * Their Process function will be call when provided message is received
 *
 */
#pragma once

#ifndef AD_MW_SYNC_TASK_UNSUPPORTED

#include <memory>
#include <utility>
#include <vector>

#include "ad_mw/types.h"
#include "ad_rscl/component/component.h"
#include "ad_rscl/exception.h"

// reference: cyber/component/component.h

namespace senseAD {
namespace rscl {
namespace component {
using senseAD::mw::BorrowedRawMessage;
using senseAD::mw::NullType;

// TODO(kit) Currently only support for case
//* - Message is from internal middleware, internal message type
//
//* Need support 2 more case:
//*  -Message is from external middleware, external message type
//*  -Message is from internal middleware, external message type
//
//! Possible solution: No longer use mw layer for message sync
template <typename M0 = NullType,
          typename M1 = NullType,
          typename M2 = NullType,
          typename M3 = NullType>
class Component : public ComponentBase {
 public:
    [[deprecated("Will be remove in future.")]] Component() {}
    ~Component() override {}

    bool Initialize(const ComponentConfig& config) override;
    bool Initialize() override;
    bool Process(const std::shared_ptr<ReceivedMsg<M0>>& msg0,
                 const std::shared_ptr<ReceivedMsg<M1>>& msg1,
                 const std::shared_ptr<ReceivedMsg<M2>>& msg2,
                 const std::shared_ptr<ReceivedMsg<M3>>& msg3);

 private:
    virtual bool OnProc(const std::shared_ptr<ReceivedMsg<M0>>& msg0,
                        const std::shared_ptr<ReceivedMsg<M1>>& msg1,
                        const std::shared_ptr<ReceivedMsg<M2>>& msg2,
                        const std::shared_ptr<ReceivedMsg<M3>>& msg3) = 0;

    std::vector<std::shared_ptr<senseAD::mw::comm::SubscriberBase>>
        subscribers_;
    std::unique_ptr<
        senseAD::mw::task::MessageSyncedCallbackTask<BorrowedRawMessage,
                                                     BorrowedRawMessage,
                                                     BorrowedRawMessage,
                                                     BorrowedRawMessage>>
        inner_task_;
};

template <typename M0>
class Component<M0, NullType, NullType, NullType> : public ComponentBase {
 public:
    [[deprecated("Will be remove in future.")]] Component() {}
    ~Component() override {}
    bool Initialize(const ComponentConfig& config) override;
    bool Initialize() override;
    bool Process(const std::shared_ptr<ReceivedMsg<M0>>& msg);

 private:
    virtual bool OnProc(const std::shared_ptr<ReceivedMsg<M0>>& msg) = 0;
    std::vector<std::shared_ptr<senseAD::mw::comm::SubscriberBase>>
        subscribers_;
    std::unique_ptr<
        senseAD::mw::task::MessageSyncedCallbackTask<BorrowedRawMessage>>
        inner_task_;
};

template <typename M0, typename M1>
class Component<M0, M1, NullType, NullType> : public ComponentBase {
 public:
    [[deprecated("Will be remove in future.")]] Component() {}
    ~Component() override {}
    bool Initialize(const ComponentConfig& config) override;
    bool Initialize() override;
    bool Process(const std::shared_ptr<ReceivedMsg<M0>>& msg0,
                 const std::shared_ptr<ReceivedMsg<M1>>& msg1);

 private:
    virtual bool OnProc(const std::shared_ptr<ReceivedMsg<M0>>& msg0,
                        const std::shared_ptr<ReceivedMsg<M1>>& msg1) = 0;
    std::vector<std::shared_ptr<senseAD::mw::comm::SubscriberBase>>
        subscribers_;
    std::unique_ptr<
        senseAD::mw::task::MessageSyncedCallbackTask<BorrowedRawMessage,
                                                     BorrowedRawMessage>>
        inner_task_;
};

template <typename M0, typename M1, typename M2>
class Component<M0, M1, M2, NullType> : public ComponentBase {
 public:
    [[deprecated("Will be remove in future.")]] Component() {}
    ~Component() override {}
    bool Initialize(const ComponentConfig& config) override;
    bool Initialize() override;
    bool Process(const std::shared_ptr<ReceivedMsg<M0>>& msg0,
                 const std::shared_ptr<ReceivedMsg<M1>>& msg1,
                 const std::shared_ptr<ReceivedMsg<M2>>& msg2);

 private:
    virtual bool OnProc(const std::shared_ptr<ReceivedMsg<M0>>& msg0,
                        const std::shared_ptr<ReceivedMsg<M1>>& msg1,
                        const std::shared_ptr<ReceivedMsg<M2>>& msg2) = 0;
    std::vector<std::shared_ptr<senseAD::mw::comm::SubscriberBase>>
        subscribers_;
    std::unique_ptr<
        senseAD::mw::task::MessageSyncedCallbackTask<BorrowedRawMessage,
                                                     BorrowedRawMessage,
                                                     BorrowedRawMessage>>
        inner_task_;
};

#define PROCESS_CHECK()    \
    if (IsShutdown()) {    \
        return true;       \
    }                      \
    OnProcGuard g(this);   \
    if (!g.AbleToProc()) { \
        return true;       \
    }

template <typename M0>
bool Component<M0, NullType, NullType, NullType>::Process(
    const std::shared_ptr<ReceivedMsg<M0>>& msg) {
    PROCESS_CHECK()

    return OnProc(msg);
}

#define PUSH_INNER_SUBSCRIBER(readers)                        \
    for (const auto& reader : readers) {                      \
        senseAD::mw::comm::SubscriberConf subscriber_conf;    \
        subscriber_conf.set_channel_name(reader.channel());   \
        /** subscriber_conf.set_qos_profile(); **/            \
        auto subscriber = senseAD::mw::comm::Subscriber::New( \
            GetMwNode(), nullptr, subscriber_conf);           \
        subscribers_.emplace_back(std::move(subscriber));     \
    }

template <typename M0>
bool Component<M0, NullType, NullType, NullType>::Initialize(
    const ComponentConfig& config) {
    bool ret = LoadConfig(config);
    ret &= Initialize();
    return ret;
}

template <typename M0>
bool Component<M0, NullType, NullType, NullType>::Initialize() {
    const auto& name = config_->synced_conf.name();
    const auto& readers = config_->synced_conf.readers();
    for (const auto& param : config_->synced_conf.custom_param()) {
        custom_param_[param.first] = param.second;
    }

    if (readers.size() != 1) {
        throw new InvalidComponentConfigException(
            "Number of sync topic is not equal to 1");
    }

    InnerNodeCreate(name);
    if (!(EmitEvent(senseAD::Event::INIT_DONE) &&
          EmitEvent(senseAD::Event::START_ALLOWED))) {
        return false;
    }

    PUSH_INNER_SUBSCRIBER(readers)

    std::vector<senseAD::mw::comm::SubscriberBase*> subscribers_ptr;
    for (auto subscriber : subscribers_) {
        subscribers_ptr.emplace_back(subscriber.get());
    }
    inner_task_ =
        senseAD::mw::task::MessageSyncedCallbackTask<BorrowedRawMessage>::New(
            name, subscribers_ptr,
            [this](const std::shared_ptr<BorrowedRawMessage>& msg) {
                auto typed_msg = std::make_shared<ReceivedMsg<M0>>(msg);
                this->Process(typed_msg);
            });
    inner_task_->Start();
    return true;
}

template <typename M0, typename M1>
bool Component<M0, M1, NullType, NullType>::Process(
    const std::shared_ptr<ReceivedMsg<M0>>& msg0,
    const std::shared_ptr<ReceivedMsg<M1>>& msg1) {
    PROCESS_CHECK()

    return OnProc(msg0, msg1);
}

template <typename M0, typename M1>
bool Component<M0, M1, NullType, NullType>::Initialize(
    const ComponentConfig& config) {
    bool ret = LoadConfig(config);
    ret &= Initialize();
    return ret;
}

template <typename M0, typename M1>
bool Component<M0, M1, NullType, NullType>::Initialize() {
    const auto& name = config_->synced_conf.name();
    const auto& readers = config_->synced_conf.readers();
    for (const auto& param : config_->synced_conf.custom_param()) {
        custom_param_[param.first] = param.second;
    }

    if (readers.size() != 2) {
        throw new InvalidComponentConfigException(
            "Number of sync topic is not equal to 2");
    }

    InnerNodeCreate(name);
    if (!(EmitEvent(senseAD::Event::INIT_DONE) &&
          EmitEvent(senseAD::Event::START_ALLOWED))) {
        return false;
    }

    PUSH_INNER_SUBSCRIBER(readers)

    std::vector<senseAD::mw::comm::SubscriberBase*> subscribers_ptr;
    for (auto subscriber : subscribers_) {
        subscribers_ptr.emplace_back(subscriber.get());
    }
    inner_task_ = senseAD::mw::task::
        MessageSyncedCallbackTask<BorrowedRawMessage, BorrowedRawMessage>::New(
            name, subscribers_ptr,
            [this](const std::shared_ptr<BorrowedRawMessage>& msg0,
                   const std::shared_ptr<BorrowedRawMessage>& msg1) {
                auto typed_msg0 = std::make_shared<ReceivedMsg<M0>>(msg0);
                auto typed_msg1 = std::make_shared<ReceivedMsg<M1>>(msg1);
                this->Process(typed_msg0, typed_msg1);
            });
    inner_task_->Start();
    return true;
}

template <typename M0, typename M1, typename M2>
bool Component<M0, M1, M2, NullType>::Process(
    const std::shared_ptr<ReceivedMsg<M0>>& msg0,
    const std::shared_ptr<ReceivedMsg<M1>>& msg1,
    const std::shared_ptr<ReceivedMsg<M2>>& msg2) {
    PROCESS_CHECK()

    return OnProc(msg0, msg1, msg2);
}

template <typename M0, typename M1, typename M2>
bool Component<M0, M1, M2, NullType>::Initialize(
    const ComponentConfig& config) {
    bool ret = LoadConfig(config);
    ret &= Initialize();
    return ret;
}

template <typename M0, typename M1, typename M2>
bool Component<M0, M1, M2, NullType>::Initialize() {
    const auto& name = config_->synced_conf.name();
    const auto& readers = config_->synced_conf.readers();
    for (const auto& param : config_->synced_conf.custom_param()) {
        custom_param_[param.first] = param.second;
    }

    if (readers.size() != 3) {
        throw new InvalidComponentConfigException(
            "Number of sync topic is not equal to 1");
    }

    InnerNodeCreate(name);
    if (!(EmitEvent(senseAD::Event::INIT_DONE) &&
          EmitEvent(senseAD::Event::START_ALLOWED))) {
        return false;
    }

    PUSH_INNER_SUBSCRIBER(readers)

    std::vector<senseAD::mw::comm::SubscriberBase*> subscribers_ptr;
    for (auto subscriber : subscribers_) {
        subscribers_ptr.emplace_back(subscriber.get());
    }
    inner_task_ = senseAD::mw::task::MessageSyncedCallbackTask<
        BorrowedRawMessage, BorrowedRawMessage, BorrowedRawMessage>::
        New(name, subscribers_ptr,
            [this](const std::shared_ptr<BorrowedRawMessage>& msg0,
                   const std::shared_ptr<BorrowedRawMessage>& msg1,
                   const std::shared_ptr<BorrowedRawMessage>& msg2) {
                auto typed_msg0 = std::make_shared<ReceivedMsg<M0>>(msg0);
                auto typed_msg1 = std::make_shared<ReceivedMsg<M1>>(msg1);
                auto typed_msg2 = std::make_shared<ReceivedMsg<M2>>(msg2);
                this->Process(typed_msg0, typed_msg1, typed_msg2);
            });
    inner_task_->Start();
    return true;
}

template <typename M0, typename M1, typename M2, typename M3>
bool Component<M0, M1, M2, M3>::Process(
    const std::shared_ptr<ReceivedMsg<M0>>& msg0,
    const std::shared_ptr<ReceivedMsg<M1>>& msg1,
    const std::shared_ptr<ReceivedMsg<M2>>& msg2,
    const std::shared_ptr<ReceivedMsg<M3>>& msg3) {
    PROCESS_CHECK()

    return OnProc(msg0, msg1, msg2, msg3);
}

template <typename M0, typename M1, typename M2, typename M3>
bool Component<M0, M1, M2, M3>::Initialize(const ComponentConfig& config) {
    bool ret = LoadConfig(config);
    ret &= Initialize();
    return ret;
}

template <typename M0, typename M1, typename M2, typename M3>
bool Component<M0, M1, M2, M3>::Initialize() {
    const auto& name = config_->synced_conf.name();
    const auto& readers = config_->synced_conf.readers();
    for (const auto& param : config_->synced_conf.custom_param()) {
        custom_param_[param.first] = param.second;
    }

    if (readers.size() != 4) {
        throw new InvalidComponentConfigException(
            "Number of sync topic is not equal to 1");
    }

    InnerNodeCreate(name);
    if (!(EmitEvent(senseAD::Event::INIT_DONE) &&
          EmitEvent(senseAD::Event::START_ALLOWED))) {
        return false;
    }

    PUSH_INNER_SUBSCRIBER(readers)

    std::vector<senseAD::mw::comm::SubscriberBase*> subscribers_ptr;
    for (auto subscriber : subscribers_) {
        subscribers_ptr.emplace_back(subscriber.get());
    }
    inner_task_ = senseAD::mw::task::MessageSyncedCallbackTask<
        BorrowedRawMessage, BorrowedRawMessage, BorrowedRawMessage,
        BorrowedRawMessage>::New(name, subscribers_ptr,
                                 [this](const std::shared_ptr<M0>& msg0,
                                        const std::shared_ptr<M1>& msg1,
                                        const std::shared_ptr<M2>& msg2,
                                        const std::shared_ptr<M3>& msg3) {
                                     auto typed_msg0 =
                                         std::make_shared<ReceivedMsg<M0>>(
                                             msg0);
                                     auto typed_msg1 =
                                         std::make_shared<ReceivedMsg<M1>>(
                                             msg1);
                                     auto typed_msg2 =
                                         std::make_shared<ReceivedMsg<M2>>(
                                             msg2);
                                     auto typed_msg3 =
                                         std::make_shared<ReceivedMsg<M3>>(
                                             msg3);
                                     this->Process(typed_msg0, typed_msg1,
                                                   typed_msg2, typed_msg3);
                                 });
    inner_task_->Start();
    return true;
}
}  // namespace component
}  // namespace rscl
}  // namespace senseAD

#endif
