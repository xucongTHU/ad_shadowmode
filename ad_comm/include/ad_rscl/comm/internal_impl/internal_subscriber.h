/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @brief An internal implementation for subscriber.
 * It will be used for the case the when sender/received is both internal
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "ad_rscl/comm/subscriber_base.h"
#include "ad_serde/msgmeta.h"
#include "ad_rscl/runtime.h"
#include "ad_rscl/common/internal_topics.h"
#include "ad_rscl/comm/internal_impl/serialized_maxsize_helper.h"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::mw::BorrowedRawMessage;

/**
 * @brief The interface/class used to handle all message receiving.
 * Provide full function that needed for subscription
 *
 * @tparam MessageT The message type in the channel
 */
template <typename MessageT>
class InternalSubscriber : public Subscriber<MessageT> {
 public:
    using Ptr = std::shared_ptr<InternalSubscriber<MessageT>>;
    using RealCallbackFunc =
        std::function<void(const std::shared_ptr<BorrowedRawMessage>&)>;
    /**
     * @brief Construct a new Subscriber object.
     *
     * @param node_owner node
     * @param cb The callback that will be automatically trigger when new
     * message received from the topic
     * @param conf All configuration parameter
     */
    InternalSubscriber(NodeOwnerBase* const node_owner,
                       const CallbackFunc<MessageT>& cb,
                       const SubscriberConf& conf);

    /**
     * @brief Clean up all internal handle
     *
     */
    ~InternalSubscriber() override;

    void ShutdownImpl() override;

    // /**
    //  * @brief Load received message into internal buffer
    //  *
    //  */
    // virtual void Observe() const;

    // virtual std::shared_ptr<void> GetLatestObservedVoid() const;

 private:
    NodeOwnerBase* const node_owner_;

    RealCallbackFunc real_cb_;

    std::vector<std::shared_ptr<MwSubscriberHandle>> subscriber_handles_;
};

/**
 * Implementation
 */

template <typename MessageT>
InternalSubscriber<MessageT>::InternalSubscriber(
    NodeOwnerBase* const node_owner,
    const CallbackFunc<MessageT>& cb,
    const SubscriberConf& conf)
    : Subscriber<MessageT>(node_owner->GetName(), cb, conf),
      node_owner_(node_owner) {
    ::Shutdownable::UpdateTTLogDetail("InternalSubscriber|" +
                                      conf.channel_name());

    if (Subscriber<MessageT>::hetero_only_) {
        return;
    }
    auto c = conf;
    c.set_msg_serialized_max_size(GetMsgSerializedMaxSize<MessageT>());

    // TODO(kit) Cuurent cyber subscriber cannot re-add easily. Need this
    // special handle
    auto ts_cb = this->GetThreadSafedUserCb();
    real_cb_ = [ts_cb](const std::shared_ptr<BorrowedRawMessage>& msg) {
        ts_cb(std::make_shared<ReceivedMsg<MessageT>>(msg));
    };
    // For local system
    subscriber_handles_.emplace_back(senseAD::mw::comm::Subscriber::New(
        node_owner_->InnerHandle(), real_cb_, ToMwSubscriberConf(c)));

    if (subscriber_handles_[0] != nullptr) {
        this->sd_client_sptr_->AddReader(node_owner_->GetName(),
                                         c.channel_name());
    }

    // For remote system
    auto itopics = senseAD::rscl::InternalAutoPublishTopics();
    if (itopics.find(c.channel_name()) != itopics.end()) {
        auto endpoints = senseAD::rscl::GlobalConfig::Instance()
                             ->RawIdlConf()
                             .cross_system()
                             .endpoint();
        for (const auto& ep : endpoints) {
            SubscriberConf peer_conf = c;
            peer_conf.set_channel_name(senseAD::rscl::PrefixedPeerChannelName(
                ep.hostname(), c.channel_name()));
            auto sub = senseAD::mw::comm::Subscriber::New(
                node_owner_->InnerHandle(), real_cb_,
                ToMwSubscriberConf(peer_conf));
            if (sub) {
                this->sd_client_sptr_->AddReader(node_owner_->GetName(),
                                                 peer_conf.channel_name());
            }
            subscriber_handles_.emplace_back(sub);
        }
    }
}

template <typename MessageT>
InternalSubscriber<MessageT>::~InternalSubscriber() {
    ::Shutdownable::Shutdown();
}

template <typename MessageT>
void InternalSubscriber<MessageT>::ShutdownImpl() {
    auto c = static_cast<SubscriberBase*>(this)->GetSubscriberConf();
    if (subscriber_handles_.size() > 0 && subscriber_handles_[0] != nullptr) {
        this->sd_client_sptr_->LeaveReader(node_owner_->GetName(),
                                           c.channel_name());
    }

    auto itopics = senseAD::rscl::InternalAutoPublishTopics();
    if (itopics.find(c.channel_name()) != itopics.end()) {
        auto endpoints = senseAD::rscl::GlobalConfig::Instance()
                             ->RawIdlConf()
                             .cross_system()
                             .endpoint();
        for (const auto& ep : endpoints) {
            SubscriberConf peer_conf = c;
            peer_conf.set_channel_name(senseAD::rscl::PrefixedPeerChannelName(
                ep.hostname(), c.channel_name()));
            this->sd_client_sptr_->LeaveReader(node_owner_->GetName(),
                                               peer_conf.channel_name());
        }
    }

    for (const auto& sub : subscriber_handles_) {
        if (sub != nullptr) {
            sub->Shutdown();
        }
    }
    Subscriber<MessageT>::ShutdownImpl();
}

// template <typename MessageT>
// void Subscriber<MessageT>::Observe() const {
//     subscriber_handle_->Observe();
// }

// template <typename MessageT>
// std::shared_ptr<void> Subscriber<MessageT>::GetLatestObservedVoid() const
// {
//     // auto msg = subscriber_handle_->GetLatestObserved();
//     // auto out_msg = std::make_shared<ReceivedMsg<MessageT>>(msg);

//     // TODO(kit) Need remote old subscriber be for impl this
//     return nullptr;
// }

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
