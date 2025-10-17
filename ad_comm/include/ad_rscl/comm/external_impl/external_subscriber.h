/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "ad_msg_bridge/external_msg_comm/service_register.h"
#include "ad_msg_bridge/external_msg_comm/external_comm_base.h"
#include "ad_rscl/comm/internal_impl/internal_subscriber.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename MessageT>
class ExternalSubscriber : public Subscriber<MessageT> {
 public:
    using Ptr = std::shared_ptr<ExternalSubscriber<MessageT>>;

    /**
     * @brief Construct a new External Subscriber object
     *
     * @param node_owner root node of this subscriber
     * @param cb The callback that will be automatically trigger when new
     * message received from the topic.
     * @param conf All configuration parameter.
     * @param internal_compat Whether create a internal subscriber
     */
    ExternalSubscriber(NodeOwnerBase* const node_owner,
                       const CallbackFunc<MessageT>& cb,
                       const SubscriberConf& conf);

    /**
     * @brief Clean up all internal handle
     */
    ~ExternalSubscriber() override;

    void ShutdownImpl() override;

    senseAD::base::optional<senseAD::serde::MsgMeta> UnderlyMsgMeta() const {
        if (subscriber_handle_) {
            return {subscriber_handle_->MsgMeta()};
        } else {
            return {};
        }
    }

 private:
    NodeOwnerBase* const node_owner_;

    void Init(NodeOwnerBase* const node_owner, const SubscriberConf& conf);

    std::shared_ptr<senseAD::ad_msg_bridge::ExternalSubscriberBase>
        subscriber_handle_;

    // handle for replay
    std::shared_ptr<Subscriber<MessageT>> replay_handle_;
};

template <typename MessageT>
ExternalSubscriber<MessageT>::ExternalSubscriber(
    NodeOwnerBase* const node_owner,
    const CallbackFunc<MessageT>& cb,
    const SubscriberConf& conf)
    : Subscriber<MessageT>(node_owner->GetName(), cb, conf),
      node_owner_(node_owner) {
    ::Shutdownable::UpdateTTLogDetail("ExternalSubscriber|" +
                                      conf.channel_name());

    Init(node_owner, conf);
}

template <typename MessageT>
ExternalSubscriber<MessageT>::~ExternalSubscriber() {
    // internal subscriber will RemoveReader in its destructor.

    ::Shutdownable::Shutdown();
}

template <typename MessageT>
void ExternalSubscriber<MessageT>::ShutdownImpl() {
    if (subscriber_handle_ != nullptr) {
        subscriber_handle_->Shutdown();
    }
    if (this->should_send_sd_msg_) {
        auto conf = static_cast<SubscriberBase*>(this)->GetSubscriberConf();
        this->sd_client_sptr_->LeaveReader(node_owner_->GetName(),
                                           conf.channel_name());
    }
    Subscriber<MessageT>::Shutdown();
}

template <typename MessageT>
void ExternalSubscriber<MessageT>::Init(NodeOwnerBase* const node_owner,
                                        const SubscriberConf& conf) {
    if (Subscriber<MessageT>::hetero_only_) {
        return;
    }

    static_cast<senseAD::ad_msg_bridge::Topic>(
        senseAD::ad_msg_bridge::BridgeCore::Instance()->GetExternalChannel(
            conf.channel_name()))
        .SetQueueSize(uint32_t(conf.qos_profile().depth()));
    subscriber_handle_ =
        senseAD::ad_msg_bridge::ExternalCommRegsiter::Instance()
            ->CreateExternalSubscriber<MessageT>(
                conf.channel_name(),
                Subscriber<MessageT>::GetThreadSafedUserCb());

    this->should_send_sd_msg_ = !conf.internal_compat() && subscriber_handle_;
    if (this->should_send_sd_msg_) {
        this->sd_client_sptr_->AddReader(node_owner_->GetName(),
                                         conf.channel_name());
    }

    if (conf.internal_compat()) {
        replay_handle_ = std::make_shared<InternalSubscriber<MessageT>>(
            node_owner, Subscriber<MessageT>::GetThreadSafedUserCb(), conf);
    }
    Subscriber<MessageT>::ToggleSwitch(true);
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
