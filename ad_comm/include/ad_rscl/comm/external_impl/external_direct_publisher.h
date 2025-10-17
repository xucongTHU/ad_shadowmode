/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>

#include "ad_msg_bridge/external_msg_comm/service_register.h"
#include "ad_msg_bridge/external_msg_comm/external_comm_base.h"
#include "ad_rscl/common/errcode.h"
#include "ad_rscl/comm/internal_impl/internal_publisher.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename RsclType, typename ExternalType>
class ExternalDirectPublisher : public Publisher<RsclType> {
 public:
    using Ptr =
        std::shared_ptr<ExternalDirectPublisher<RsclType, ExternalType>>;

    explicit ExternalDirectPublisher(NodeOwnerBase* const node_owner,
                                     const PublisherConf& conf);

    virtual ~ExternalDirectPublisher();

    std::error_code PublishExternalOnly(const ExternalType& msg);

 protected:
    virtual std::error_code PublishMsg(const SendMsg<RsclType>& msg,
                                       uint64_t seq = 0);

 private:
    NodeOwnerBase* const node_owner_;
    std::shared_ptr<senseAD::ad_msg_bridge::ExternalPublisherBase>
        publisher_handle_;

    std::shared_ptr<InternalPublisher<RsclType>> internal_publisher_handle_;
};

template <typename RsclType, typename ExternalType>
ExternalDirectPublisher<RsclType, ExternalType>::ExternalDirectPublisher(
    NodeOwnerBase* const node_owner, const PublisherConf& conf)
    : Publisher<RsclType>(node_owner->GetName(), conf),
      node_owner_(node_owner) {
    static_cast<senseAD::ad_msg_bridge::Topic>(
        senseAD::ad_msg_bridge::BridgeCore::Instance()->GetExternalChannel(
            conf.channel_name()))
        .SetQueueSize(uint32_t(conf.qos_profile().depth()));
    publisher_handle_ =
        senseAD::ad_msg_bridge::ExternalCommRegsiter::Instance()
            ->CreateExternalDirectPublisher<RsclType, ExternalType>(
                conf.channel_name());

    this->should_send_sd_msg_ = !conf.internal_compat() && publisher_handle_;
    if (this->should_send_sd_msg_) {
        this->sd_client_sptr_->AddWriter(node_owner_->GetName(),
                                         conf.channel_name(), conf.msg_type(),
                                         conf.msg_descriptor());
    }

    if (conf.internal_compat()) {
        internal_publisher_handle_ =
            std::make_shared<InternalPublisher<RsclType>>(node_owner, conf);
    }
}

template <typename RsclType, typename ExternalType>
ExternalDirectPublisher<RsclType, ExternalType>::~ExternalDirectPublisher() {
    if (this->should_send_sd_msg_) {
        auto conf = static_cast<PublisherBase*>(this)->GetPublisherConf();
        this->sd_client_sptr_->LeaveWriter(node_owner_->GetName(),
                                           conf.channel_name());
    }
}

template <typename RsclType, typename ExternalType>
std::error_code
ExternalDirectPublisher<RsclType, ExternalType>::PublishExternalOnly(
    const ExternalType& msg) {
    auto pre_ns = time::Time::SteadyNow().ToNanosecond();
    auto ret = publisher_handle_->PublishWithoutConvert(msg);
    auto time_cost = time::Time::SteadyNow().ToNanosecond() - pre_ns;

    if (time_cost > 1e9) {
        AD_LERROR(ExternalDirectPublisher)
            << "Topic Publish is blocking!!! " << PublisherBase::GetTopicName()
            << " publish used ns: " << time_cost;
    }

    return ret ? std::error_code() : RsclError::kPublishExternalFailed;
}

template <typename RsclType, typename ExternalType>
std::error_code ExternalDirectPublisher<RsclType, ExternalType>::PublishMsg(
    const SendMsg<RsclType>& msg, uint64_t seq) {
    auto pre_ns = time::Time::SteadyNow().ToNanosecond();
    auto ret = publisher_handle_->PublishWithConvert(msg);
    auto time_cost = time::Time::SteadyNow().ToNanosecond() - pre_ns;

    if (time_cost > 1e9) {
        AD_LERROR(ExternalDirectPublisher)
            << "Topic Publish is blocking!!! " << PublisherBase::GetTopicName()
            << " publish used ns: " << time_cost;
    }
    if (!ret) {
        return RsclError::kPublishExternalFailed;
    }

    return internal_publisher_handle_
               ? internal_publisher_handle_->PublishMsg(msg, seq)
               : std::error_code();
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
