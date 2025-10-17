/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
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

template <typename MessageT>
class ExternalPublisher : public Publisher<MessageT> {
 public:
    using Ptr = std::shared_ptr<ExternalPublisher<MessageT>>;

    explicit ExternalPublisher(NodeOwnerBase* const node_owner,
                               const PublisherConf& conf,
                               const bool internal_support = true);

    ~ExternalPublisher() override;

    void ShutdownImpl() override;

 protected:
    std::error_code PublishMsg(const SendMsg<MessageT>& msg,
                               uint64_t seq = 0) override;

 private:
    NodeOwnerBase* const node_owner_;

    std::shared_ptr<senseAD::ad_msg_bridge::ExternalPublisherBase>
        publisher_handle_;

    std::shared_ptr<InternalPublisher<MessageT>> internal_publisher_handle_;
};

template <typename MessageT>
ExternalPublisher<MessageT>::ExternalPublisher(NodeOwnerBase* const node_owner,
                                               const PublisherConf& conf,
                                               const bool internal_support)
    : Publisher<MessageT>(node_owner->GetName(), conf),
      node_owner_(node_owner) {
    ::Shutdownable::UpdateTTLogDetail("ExternalPublisher|" +
                                      conf.channel_name());

    static_cast<senseAD::ad_msg_bridge::Topic>(
        senseAD::ad_msg_bridge::BridgeCore::Instance()->GetExternalChannel(
            conf.channel_name()))
        .SetQueueSize(uint32_t(conf.qos_profile().depth()));
    publisher_handle_ =
        senseAD::ad_msg_bridge::ExternalCommRegsiter::Instance()
            ->CreateExternalPublisher<MessageT>(conf.channel_name());

    this->should_send_sd_msg_ = !conf.internal_compat() && publisher_handle_;
    if (this->should_send_sd_msg_) {
        this->sd_client_sptr_->AddWriter(node_owner_->GetName(),
                                         conf.channel_name(), conf.msg_type(),
                                         conf.msg_descriptor());
    }

    if (internal_support) {
        internal_publisher_handle_ =
            std::make_shared<InternalPublisher<MessageT>>(node_owner, conf);
    }
}

template <typename MessageT>
ExternalPublisher<MessageT>::~ExternalPublisher() {
    ::Shutdownable::Shutdown();
}

template <typename MessageT>
void ExternalPublisher<MessageT>::ShutdownImpl() {
    if (internal_publisher_handle_ != nullptr) {
        internal_publisher_handle_->Shutdown();
    }
    if (publisher_handle_ != nullptr) {
        publisher_handle_->Shutdown();
    }
    Publisher<MessageT>::ShutdownImpl();

    if (this->should_send_sd_msg_) {
        auto conf = static_cast<PublisherBase*>(this)->GetPublisherConf();
        this->sd_client_sptr_->LeaveWriter(node_owner_->GetName(),
                                           conf.channel_name());
    }
}

template <typename MessageT>
std::error_code ExternalPublisher<MessageT>::PublishMsg(
    const SendMsg<MessageT>& msg, uint64_t seq) {
    if (::Shutdownable::IsShutdown()) {
        AD_LERROR(ExternalPublisher)
            << PublisherBase::GetPublisherConf().channel_name()
            << ", ExternalPublisher already shutdown, not publishing msg";
        return std::error_code();
    }
    auto pre_ns = time::Time::SteadyNow().ToNanosecond();
    publisher_handle_->PublishWithConvert(msg);
    auto time_cost = time::Time::SteadyNow().ToNanosecond() - pre_ns;

    if (time_cost > 1e9) {
        AD_LERROR(ExternalPublisher)
            << "Topic Publish is blocking!!! " << PublisherBase::GetTopicName()
            << " publish used ns: " << time_cost;
    }
    return internal_publisher_handle_
               ? internal_publisher_handle_->PublishMsg(msg, seq)
               : std::error_code();
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
