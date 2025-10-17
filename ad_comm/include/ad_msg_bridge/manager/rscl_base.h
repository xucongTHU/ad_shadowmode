/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 * KitFung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <string>
#include <memory>
#include <sstream>

#include "ad_msg_bridge/manager/exception.h"
#include "ad_msg_bridge/external_msg_comm/external_comm_base.h"
#include "ad_msg_bridge/manager/bridge_config.h"
#include "ad_base/class_loader/class_loader_register_macro.h"
#include "ad_mw/types.h"

namespace senseAD {
namespace ad_msg_bridge {

// ExternalCommunicatorBase is the base class of all external communicators
// used to manage the creation of external publishers and subscribers.
// It also manages the reception of external message playback recording.
class ExternalCommunicatorBase {
 public:
    using Ptr = std::shared_ptr<ExternalCommunicatorBase>;
    virtual ~ExternalCommunicatorBase() = default;

    /**
     * @brief typeid(ExternalType)
     * ExternalType is the actual message type while dealing with external sdk
     */
    virtual const std::type_info& ExternalTypeInfo() const = 0;
    /**
     * @brief typeid(RsclType)
      RsclType is the message type that used in rscl communication
     */
    virtual const std::type_info& RsclTypeInfo() const = 0;

    std::shared_ptr<ExternalPublisherBase> CreateExternalPublisher(
        const senseAD::ad_msg_bridge::Topic& topic_conf,
        const std::type_info* rscl_type = nullptr,
        const std::type_info* external_type = nullptr);

    std::shared_ptr<ExternalSubscriberBase> CreateExternalSubscriber(
        const senseAD::ad_msg_bridge::Topic& topic_conf,
        const std::type_info* rscl_type = nullptr,
        const std::type_info* external_type = nullptr);

 protected:
    virtual std::shared_ptr<ExternalPublisherBase> CreateExternalPublisherImpl(
        const senseAD::ad_msg_bridge::Topic& topic_conf) = 0;

    virtual std::shared_ptr<ExternalSubscriberBase>
    CreateExternalSubscriberImpl(
        const senseAD::ad_msg_bridge::Topic& topic_conf) = 0;
};

template <typename RsclType, typename ExternalType>
class ExternalCommunicator : public ExternalCommunicatorBase {
 public:
    virtual ~ExternalCommunicator() {}
    const std::type_info& RsclTypeInfo() const final {
        return typeid(RsclType);
    }
    const std::type_info& ExternalTypeInfo() const final {
        return typeid(ExternalType);
    }

    virtual std::string ClassName() { return "ExternalCommunicator"; }
};

}  // namespace ad_msg_bridge
}  // namespace senseAD
