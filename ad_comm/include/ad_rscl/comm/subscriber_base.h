/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ad_diag/base/supervisions/supervision_comm.h"
#include "ad_base/common/shutdownable.h"
#include "ad_mw/ad_mw.h"
#include "ad_log/ad_log.hpp"
#include "ad_serde/ad_serde.h"
#include "ad_rscl/comm/hetero_channel/channel_factory.h"
#include "ad_rscl/comm/types.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename M0>
using CallbackFunc =
    std::function<void(const std::shared_ptr<ReceivedMsg<M0>>&)>;
template <typename M0>
using ExternalCallbackFunc = std::function<void(const M0&)>;

/**
 * @brief Base type for subscriber.
 * For the usecase that the user will not call member functions that have
 * message
 * type in its signature
 *
 */
class SubscriberBase : public SwitchableNodeMemberBase, public ::Shutdownable {
 public:
    using Ptr = std::shared_ptr<SubscriberBase>;

    virtual ~SubscriberBase();

    void ShutdownImpl() override;

    // /**
    //  * @brief Load received message into internal buffer
    //  *
    //  */
    // virtual void Observe() const = 0;

    /**
     * @brief Get the Topic Name
     *
     * @return const std::string&
     */
    const std::string& GetTopicName() const;
    /**
     * @brief Subscriber Configuration
     *
     * @return const SubscriberConf&
     */
    const SubscriberConf& GetSubscriberConf() const;

    /**
     * @brief Get the Msg Property frame ratio essential info
     *
     * @return const TransmitEval&
     */
    const TransmitEval& GetTransmitEval();

    const TransmitEval& GetTransmitEvalAndReportSupervision();
    /**
     * @brief set real_cb_callable_
     */
    void ToggleSwitch(const bool enable) override;
    // template <typename MessageT>
    // std::shared_ptr<MessageT> GetLatestObserved() const {
    //     return std::dynamic_pointer_cast<MessageT>(GetLatestObservedVoid());
    // }

 protected:
    SubscriberBase(const std::string& node_name, const SubscriberConf& conf);
    // virtual std::shared_ptr<void> GetLatestObservedVoid() const = 0;
    virtual void TriggerUserCallback(
        const std::shared_ptr<BorrowedRawMessage>& msg) const;

    std::string node_name_;
    SubscriberConf conf_;

    std::vector<std::shared_ptr<hetero::SubscriberBase>> hetero_subs_;
    bool hetero_only_ = false;
    bool real_cb_callable_ = true;
    senseAD::diag::CommSupervision supervision_;

    std::shared_ptr<CommInstanceCountHelper> instance_cnt_ref_;
};

// For the case that user want to store a typed pointer
template <typename MessageT>
class Subscriber : public SubscriberBase {
 public:
    using Ptr = std::shared_ptr<Subscriber<MessageT>>;

    ~Subscriber() override {
        if (sim_time_back_listener_handle_ != nullptr) {
            sim_time_back_listener_handle_->Shutdown();
        }
        ::Shutdownable::Shutdown();
    }

    void ShutdownImpl() override { SubscriberBase::ShutdownImpl(); }

 protected:
    Subscriber(const std::string& node_name,
               const CallbackFunc<MessageT>& cb,
               const SubscriberConf& conf)
        : SubscriberBase(node_name, conf), given_cb_(cb) {
        ::Shutdownable::UpdateTTLogDetail("Subscriber|" + conf.channel_name());
        if (!conf_.has_msg_type()) {
            conf_.set_msg_type(senseAD::serde::MessageTypeMeta<
                               MessageT>::GetMessageTypename());
        }
        if (!conf_.has_msg_descriptor()) {
            conf_.set_msg_descriptor(senseAD::serde::MessageTypeMeta<
                                     MessageT>::GetMessageDescriptor());
        }
        if (conf.disable_transmit_eval()) {
            transmit_eval().Disable();
        }

        Subscriber<MessageT>* self = this;
        std::string name = node_name;
        self->ToggleSwitch(!conf.default_disable_callback());
        if (conf.advance_verification() &&
            !ReceivedMsg<MessageT>::DataIntegrityTrusted()) {
            dyn_reader_ = senseAD::serde::DynamicMessageReader::New<MessageT>();
        }

        sim_time_back_listener_handle_ =
        time::SimClockSource::AddOnSimTimeBackListener([self](auto pre_ns, auto ns) {
            AD_LERROR_EVERY_SEC(Subscriber, 1) << "trigger sim time back."
                << " pre ns: " << pre_ns << " now ns: " << ns;
            std::lock_guard<std::mutex> lock(self->pre_seq_map_mtx_);
            self->pre_frame_seq_map_.clear();
            return;
        });

        const std::string& topic_name = GetTopicName();
        if (topic_name.rfind("/internal", 0) == 0) {
            internal_topic_flag_ = true;
        }
        pre_frame_seq_map_.reserve(10);
        thread_safed_user_cb_ =
            [self](const std::shared_ptr<ReceivedMsg<MessageT>>& v) {
                if (!self->IsEnabled()) {
                    return;
                }
                if (!v->IsValid()) {
                    AD_LERROR_EVERY_SEC(Subscriber, 3)
                        << "Receive a failed-to-parse message in topic: "
                        << dynamic_cast<SubscriberBase*>(self)->GetTopicName();
                    return;
                }
                if (self->dyn_reader_) {
                    if (!self->dyn_reader_->VerifyData(v->Bytes(),
                                                       v->ByteSize())) {
                        AD_LERROR(Subscriber)
                            << "Receive a verify failed message in topic: "
                            << dynamic_cast<SubscriberBase*>(self)
                                   ->GetTopicName();
                        return;
                    }
                }
                const std::string& topic_name =
                    static_cast<SubscriberBase*>(self)->GetTopicName();
                auto const& header = v->Header();
                if (!self->internal_topic_flag_ && header.is_enabled) {
                    uint64_t cur_seq = header.seq;
                    const std::string& node_id = header.unique_node_id;
                    std::lock_guard<std::mutex> lock(self->pre_seq_map_mtx_);
                    auto& seq_map = self->pre_frame_seq_map_;
                    auto insert_result = seq_map.insert(
                        std::make_pair(node_id, cur_seq));
                    auto it = insert_result.first;
                    if (!insert_result.second) {
                        uint64_t prev_seq = it->second;
                        if ((cur_seq == prev_seq) && (cur_seq != 0)) {
                            AD_LERROR_EVERY_SEC(Subscriber, 1)
                                << "Receive a repeated message in topic: "
                                << topic_name << " node id: " << node_id
                                << " repeated seq: " << cur_seq;
                            return;
                        }
                        if (cur_seq < prev_seq) {
                            AD_LERROR_EVERY_SEC(Subscriber, 1)
                                << "Receive a history message in topic: "
                                << topic_name << " node id: " << node_id
                                << " cur_seq: " << cur_seq
                                << " prev_seq: " << prev_seq;
                            return;
                        }
                        if (cur_seq - prev_seq > 1) {
                            AD_LERROR_EVERY_SEC(Subscriber, 1)
                                << "Receive a jumped message in topic: "
                                << topic_name << " node id: " << node_id
                                << " cur_seq: " << cur_seq
                                << " prev_seq: " << prev_seq;
                        }
                        it->second = cur_seq;
                    }
                }
                {
                    std::lock_guard<std::mutex> lock(self->cb_mtx_);
                    self->transmit_eval() += v->ByteSize();
                    if (self->real_cb_callable_) {
                        self->given_cb_(v);
                    }
                }
            };
    }

    inline const CallbackFunc<MessageT>& GetThreadSafedUserCb() const {
        return thread_safed_user_cb_;
    }

    void ToggleSwitch(const bool enable) override {
        real_cb_callable_ = enable;
    }

 protected:
    void TriggerUserCallback(
        const std::shared_ptr<BorrowedRawMessage>& msg) const final {
        auto out_msg = std::make_shared<ReceivedMsg<MessageT>>(msg);
        thread_safed_user_cb_(out_msg);
    }

    std::mutex cb_mtx_;
    std::mutex pre_seq_map_mtx_;

 private:
    bool internal_topic_flag_{false};
    std::unordered_map<std::string, uint64_t> pre_frame_seq_map_;
    const CallbackFunc<MessageT> given_cb_;
    CallbackFunc<MessageT> thread_safed_user_cb_;
    std::unique_ptr<senseAD::serde::DynamicMessageReader> dyn_reader_{nullptr};
    ListenerHandle::Ptr sim_time_back_listener_handle_;
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
