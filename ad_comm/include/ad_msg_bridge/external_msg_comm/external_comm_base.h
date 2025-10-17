/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <functional>
#include <cstring>
#include <typeinfo>

#include "ad_log/ad_log.hpp"
#include "ad_base/common/shutdownable.h"
#include "ad_mw/proto/comm_conf.pb.h"
#include "ad_mw/comm.h"
#include "ad_serde/ad_serde.h"
#include "ad_serde/msgmeta.h"
#include "ad_msg_bridge/manager/exception.h"
#include "ad_msg_bridge/manager/bridge_config.h"

template <typename M0>
using CallbackFunc =
    std::function<void(const std::shared_ptr<ReceivedMsg<M0>>&)>;
template <typename M0>
using ExternalCallbackFunc = std::function<void(const M0&)>;

namespace senseAD {
namespace ad_msg_bridge {

using PublisherConf = senseAD::mw::proto::PublisherConf;
using SubscriberConf = senseAD::mw::proto::SubscriberConf;
using ServiceConf = senseAD::mw::proto::ServiceConf;
using ClientConf = senseAD::mw::proto::ClientConf;
using SharedVoidPtr = std::shared_ptr<void>;

class ExternalPublisherBase : public senseAD::mw::comm::PublisherBase,
                              public ::Shutdownable {
 public:
    virtual ~ExternalPublisherBase() { Shutdown(); }

    void ShutdownImpl() override {}

    virtual const std::type_info& ExternalTypeInfo() const = 0;
    virtual const std::type_info& RsclTypeInfo() const = 0;
    virtual senseAD::serde::MsgMeta MsgMeta() const = 0;

    virtual bool PublishWithConvert(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg) = 0;
    inline bool PublishWithConvert(
        const SendMsg<senseAD::mw::BorrowedRawMessage>& msg) {
        return PublishWithConvert(msg.Finalize().BorrowRaw());
    }
    template <typename RsclType>
    bool PublishWithConvert(const SendMsg<RsclType>& msg) {
        if (strcmp(typeid(RsclType).name(), RsclTypeInfo().name()) != 0) {
            std::stringstream ss;
            ss << "Called ExternalPublisherBase::PublishWithConvert using "
                  "invalid type ["
               << typeid(RsclType).name() << "] while expect ["
               << RsclTypeInfo().name() << "]";
            throw senseAD::ad_msg_bridge::InvalidRsclTypeException(ss.str());
        }
        return PublishWithConvertImpl(&msg);
    }
    template <typename ExternalType>
    bool PublishWithoutConvert(const ExternalType& msg) {
        if (strcmp(typeid(ExternalType).name(), ExternalTypeInfo().name()) !=
            0) {
            std::stringstream ss;
            ss << "Called ExternalPublisherBase::PublishWithoutConvert using "
                  "invalid type ["
               << typeid(ExternalType).name() << "] while expect ["
               << ExternalTypeInfo().name() << "]";
            throw senseAD::ad_msg_bridge::InvalidExternalTypeException(
                ss.str());
        }
        return PublishWithoutConvertImpl(&msg);
    }

 protected:
    explicit ExternalPublisherBase(const PublisherConf& conf)
        : PublisherBase(conf, nullptr),
          ::Shutdownable("ExternalPublisherBase", conf.channel_name()) {}

    virtual bool PublishWithConvertImpl(const void* rscl_msg_ptr) = 0;
    virtual bool PublishWithoutConvertImpl(const void* external_msg_ptr) = 0;
};

/**
 * @brief Act as a middle layer between the user view (non template class with
 * template function) and the actual impl view (template class with non template
 * function)
 * Allow the bridge so export a non-templated interface class while able to
 * interact to the actual message type in runtime
 *
 * @tparam RsclType
 * @tparam ExternalType
 */
template <typename RsclType, typename ExternalType>
class ExternalPublisher : public ExternalPublisherBase {
 public:
    virtual ~ExternalPublisher() {}
    const std::type_info& RsclTypeInfo() const final {
        return typeid(RsclType);
    }
    const std::type_info& ExternalTypeInfo() const final {
        return typeid(ExternalType);
    }
    senseAD::serde::MsgMeta MsgMeta() const final {
        return senseAD::serde::MsgMeta::From<RsclType>();
    }

 protected:
    explicit ExternalPublisher(const senseAD::ad_msg_bridge::Topic& topic)
        : ExternalPublisherBase(topic.ToPublisherConf()) {}

    bool PublishWithConvert(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg) final {
        auto rscl_msg_ptr = BorrowedRawMessageToRsclMsg(msg);
        if (!rscl_msg_ptr) {
            // Received a wrong type message
            AD_LWARN(ExternalPublisher)
                << "Received message with different msg type for topic: "
                << PublisherBase::GetPublisherConf().channel_name();
            return false;
        }
        return PublishWithConvertImpl(rscl_msg_ptr.get());
    }
    bool PublishWithConvertImpl(const void* rscl_msg_ptr) final {
        auto msg_ptr = reinterpret_cast<const SendMsg<RsclType>*>(rscl_msg_ptr);
        return PublishWithConvertImplTyped(*msg_ptr);
    }
    bool PublishWithoutConvertImpl(const void* external_msg_ptr) final {
        auto msg_ptr = reinterpret_cast<const ExternalType*>(external_msg_ptr);
        return PublishWithoutConvertImplTyped(*msg_ptr);
    }

    virtual bool PublishWithConvertImplTyped(
        const SendMsg<RsclType>& rscl_msg) = 0;
    virtual bool PublishWithoutConvertImplTyped(
        const ExternalType& external_msg) = 0;

    std::shared_ptr<void> BorrowedRawMessageToRsclMsg(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg) {
        auto rmsg = ReceivedMsg<RsclType>(msg);
        auto smsg = std::make_shared<SendMsg<RsclType>>(rmsg.Msg());
        return smsg;
    }
};

class ExternalSubscriberBase : public senseAD::mw::comm::SubscriberBase,
                               public ::Shutdownable {
 public:
    virtual ~ExternalSubscriberBase() { Shutdown(); }
    void ShutdownImpl() override {}

    virtual const std::type_info& ExternalTypeInfo() const = 0;
    virtual const std::type_info& RsclTypeInfo() const = 0;
    virtual senseAD::serde::MsgMeta MsgMeta() const = 0;

    template <typename RsclType>
    bool SubscribeRsclType(CallbackFunc<RsclType> cb) {
        if (strcmp(typeid(RsclType).name(), RsclTypeInfo().name()) != 0) {
            std::stringstream ss;
            ss << "Called ExternalSubscriberBase::SubscribeRsclType using "
                  "invalid type ["
               << typeid(RsclType).name() << "] while expect ["
               << RsclTypeInfo().name() << "]";
            throw senseAD::ad_msg_bridge::InvalidRsclTypeException(ss.str());
        }
        return SubscribeRsclTypeImpl([cb](const void* ptr) {
            auto val =
                reinterpret_cast<const std::shared_ptr<ReceivedMsg<RsclType>>*>(
                    ptr);
            cb(*val);
        });
    }
    template <typename ExternalType>
    bool SubscribeExternalType(ExternalCallbackFunc<ExternalType> external_cb) {
        if (strcmp(typeid(ExternalType).name(), ExternalTypeInfo().name()) !=
            0) {
            std::stringstream ss;
            ss << "Called ExternalSubscriberBase::SubscribeExternalType using "
                  "invalid type ["
               << typeid(ExternalType).name() << "] while expect ["
               << ExternalTypeInfo().name() << "]";
            throw senseAD::ad_msg_bridge::InvalidExternalTypeException(
                ss.str());
        }
        return SubscribeExternalTypeImpl([external_cb](const void* ptr) {
            auto val = reinterpret_cast<const ExternalType*>(ptr);
            external_cb(*val);
        });
    }

    virtual bool SubscribeRawMessage(
        CallbackFunc<senseAD::mw::RawMessage> cb) = 0;

 protected:
    explicit ExternalSubscriberBase(const SubscriberConf& conf)
        : SubscriberBase(conf, nullptr),
          ::Shutdownable("ExternalSubscriberBase", conf.channel_name()) {}

    virtual bool SubscribeRsclTypeImpl(std::function<void(const void*)> cb) = 0;
    virtual bool SubscribeExternalTypeImpl(
        std::function<void(const void*)> external_cb) = 0;
};

/**
 * @brief Act as a middle layer between the user view (non template class with
 * template function) and the actual impl view (template class with non template
 * function)
 * Allow the bridge so export a non-templated interface class while able to
 * interact to the actual message type in runtime
 *
 * @tparam RsclType
 * @tparam ExternalType
 */
template <typename RsclType, typename ExternalType>
class ExternalSubscriber : public ExternalSubscriberBase {
 public:
    explicit ExternalSubscriber(const senseAD::ad_msg_bridge::Topic& topic)
        : ExternalSubscriberBase(topic.ToSubscriberConf()) {}

    virtual ~ExternalSubscriber() {}

    const std::type_info& ExternalTypeInfo() const final {
        return typeid(ExternalType);
    }
    const std::type_info& RsclTypeInfo() const final {
        return typeid(RsclType);
    }
    senseAD::serde::MsgMeta MsgMeta() const final {
        return senseAD::serde::MsgMeta::From<RsclType>();
    }

 private:
    bool SubscribeRsclTypeImpl(std::function<void(const void*)> cb) final {
        return SubscribeRsclTypeImplTyped([cb](auto val) { cb(&val); });
    }
    bool SubscribeExternalTypeImpl(
        std::function<void(const void*)> external_cb) final {
        return SubscribeExternalTypeImplTyped(
            [external_cb](const ExternalType& val) { external_cb(&val); });
    }

    bool SubscribeRawMessage(CallbackFunc<senseAD::mw::RawMessage> cb) final {
        return SubscribeRsclTypeImplTyped([cb](auto val) {
            auto e = std::make_shared<senseAD::mw::BorrowedRawMessage>(
                val->Bytes(), val->ByteSize(), val);
            auto v = std::make_shared<ReceivedMsg<senseAD::mw::RawMessage>>(e);
            cb(v);
        });
    }

    virtual bool SubscribeRsclTypeImplTyped(
        std::function<void(std::shared_ptr<ReceivedMsg<RsclType>>&)> cb) = 0;
    virtual bool SubscribeExternalTypeImplTyped(
        std::function<void(const ExternalType&)> external_cb) = 0;
};

}  // namespace ad_msg_bridge
}  // namespace senseAD
