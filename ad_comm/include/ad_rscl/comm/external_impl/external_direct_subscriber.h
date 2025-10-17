/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 * KitFung <fengzhongjie@senseauto.com>
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

template <typename RsclType, typename ExternalType>
class ExternalDirectSubscriber : public Subscriber<RsclType> {
 public:
    using Ptr =
        std::shared_ptr<ExternalDirectSubscriber<RsclType, ExternalType>>;

    ExternalDirectSubscriber(
        NodeOwnerBase* const node_owner,
        const ExternalCallbackFunc<ExternalType>& external_cb,
        const CallbackFunc<RsclType>& cb,
        const SubscriberConf& conf);

    /**
     * @brief Clean up all internal handle
     */
    virtual ~ExternalDirectSubscriber();

    senseAD::base::optional<senseAD::serde::MsgMeta> UnderlyMsgMeta() const {
        if (subscriber_handle_) {
            return {subscriber_handle_->MsgMeta()};
        } else {
            return {};
        }
    }

 private:
    void Init(NodeOwnerBase* const node_owner,
              const ExternalCallbackFunc<ExternalType>& external_cb,
              const SubscriberConf& conf);

    NodeOwnerBase* const node_owner_;

    std::shared_ptr<senseAD::ad_msg_bridge::ExternalSubscriberBase>
        subscriber_handle_;
    // handle for replay
    std::shared_ptr<Subscriber<RsclType>> replay_handle_;
    CallbackFunc<RsclType> user_cb_;
};

template <typename RsclType, typename ExternalType>
ExternalDirectSubscriber<RsclType, ExternalType>::ExternalDirectSubscriber(
    NodeOwnerBase* const node_owner,
    const ExternalCallbackFunc<ExternalType>& external_cb,
    const CallbackFunc<RsclType>& cb,
    const SubscriberConf& conf)
    : Subscriber<RsclType>(node_owner->GetName(), cb, conf),
      node_owner_(node_owner) {
    Init(node_owner, external_cb, conf);
}

template <typename RsclType, typename ExternalType>
void ExternalDirectSubscriber<RsclType, ExternalType>::Init(
    NodeOwnerBase* const node_owner,
    const ExternalCallbackFunc<ExternalType>& external_cb,
    const SubscriberConf& conf) {
    if (Subscriber<RsclType>::hetero_only_) {
        return;
    }

    auto meta = senseAD::serde::MsgMeta::From<RsclType>();

    auto self = this;
    if (external_cb) {
        static_cast<senseAD::ad_msg_bridge::Topic>(
            senseAD::ad_msg_bridge::BridgeCore::Instance()->GetExternalChannel(
                conf.channel_name()))
            .SetQueueSize(uint32_t(conf.qos_profile().depth()));
        subscriber_handle_ =
            senseAD::ad_msg_bridge::ExternalCommRegsiter::Instance()
                ->CreateExternalDirectSubscriber<RsclType, ExternalType>(
                    conf.channel_name(), [self, external_cb](const auto& v) {
                        if (!self->IsEnabled()) {
                            return;
                        }
                        std::lock_guard<std::mutex> lock(self->cb_mtx_);
                        external_cb(v);
                    });
    } else {
        subscriber_handle_ =
            senseAD::ad_msg_bridge::ExternalCommRegsiter::Instance()
                ->CreateExternalSubscriber<RsclType>(
                    conf.channel_name(),
                    Subscriber<RsclType>::GetThreadSafedUserCb());
    }

    this->should_send_sd_msg_ = !conf.internal_compat() && subscriber_handle_;
    if (this->should_send_sd_msg_) {
        this->sd_client_sptr_->AddReader(node_owner_->GetName(),
                                         conf.channel_name());
    }

    if (conf.internal_compat()) {
        replay_handle_ = std::make_shared<InternalSubscriber<RsclType>>(
            node_owner, Subscriber<RsclType>::GetThreadSafedUserCb(), conf);
    }
    Subscriber<RsclType>::ToggleSwitch(true);
}

template <typename RsclType, typename ExternalType>
ExternalDirectSubscriber<RsclType, ExternalType>::~ExternalDirectSubscriber() {
    if (this->should_send_sd_msg_) {
        auto conf = static_cast<SubscriberBase*>(this)->GetSubscriberConf();
        this->sd_client_sptr_->LeaveReader(node_owner_->GetName(),
                                           conf.channel_name());
    }
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
