/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @brief An internal implementation for publisher.
 * It will be used for the case the when sender/received is both internal
 */

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "ad_rscl/comm/publisher_base.h"
#include "ad_serde/msgmeta.h"
#include "ad_rscl/common/errcode.h"
#include "ad_rscl/time/time.h"
#include "ad_rscl/comm/internal_impl/serialized_maxsize_helper.h"
#include "ad_rscl/common/feature_manager.h"

#include "ad_mw/types.h"
#include "ad_service_discovery/polyfill_service_discovery/sd_service_client.h"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::mw::BorrowedRawMessage;

template <typename MessageT>
class ExternalPublisher;
template <typename RsclType, typename ExternalType>
class ExternalDirectPublisher;

using SrvDiscEvent = senseAD::service_discovery::SrvDiscEvent;
using SrvDiscRoleAttr = senseAD::service_discovery::SrvDiscRoleAttr;

template <typename MessageT>
class InternalPublisher : public Publisher<MessageT> {
 public:
    using Ptr = std::shared_ptr<InternalPublisher<MessageT>>;
    InternalPublisher(NodeOwnerBase* const node_owner,
                      const PublisherConf& conf);

    ~InternalPublisher() override;

    void ShutdownImpl() override;

    void ToggleSwitch(const bool enable) override;

 protected:
    std::error_code PublishMsg(const SendMsg<MessageT>& msg,
                               const uint64_t seq) override;

 private:
    NodeOwnerBase* const node_owner_;
    std::shared_ptr<MwPublisherHandle> publisher_handle_;
    RsclMsgHeaderOptions header_option_;

    friend class ExternalPublisher<MessageT>;

    template <typename RsclType, typename ExternalType>
    friend class ExternalDirectPublisher;
};

template <typename MessageT>
InternalPublisher<MessageT>::InternalPublisher(NodeOwnerBase* const node_owner,
                                               const PublisherConf& conf)
    : Publisher<MessageT>(node_owner->GetName(), conf),
      node_owner_(node_owner) {
    ::Shutdownable::UpdateTTLogDetail("InternalPublisher|" +
                                      conf.channel_name());

    header_option_.enable_checksome = conf.enable_checksome();
    ToggleSwitch(true);
}

template <typename MessageT>
InternalPublisher<MessageT>::~InternalPublisher() {
    ::Shutdownable::Shutdown();
}

template <typename MessageT>
void InternalPublisher<MessageT>::ShutdownImpl() {
    ToggleSwitch(false);
    if (publisher_handle_ != nullptr) {
        publisher_handle_->Shutdown();
    }
    Publisher<MessageT>::ShutdownImpl();
}

template <typename MessageT>
void InternalPublisher<MessageT>::ToggleSwitch(const bool enable) {
    auto conf = this->GetPublisherConf();
    conf.set_msg_serialized_max_size(GetMsgSerializedMaxSize<MessageT>());
    if (enable) {
        publisher_handle_ = senseAD::mw::comm::Publisher::New(
            node_owner_->InnerHandle(), ToMwPublisherConf(conf));

        if (publisher_handle_) {
            bool ret = this->sd_client_sptr_->AddWriter(
                node_owner_->GetName(), conf.channel_name(), conf.msg_type(),
                conf.msg_descriptor());
            if (!ret) {
                AD_LDEBUG(SD) << conf.channel_name() << " add writer failed!";
            }
        }

        for (auto& p : this->hetero_pubs_) {
            if (!p->IsEstablished()) {
                p->Establish();
            }
        }
    } else {
        if (publisher_handle_) {
            this->sd_client_sptr_->LeaveWriter(this->node_name_,
                                               conf.channel_name());
        }
        publisher_handle_.reset();
        publisher_handle_ = nullptr;
        AD_LDEBUG(InternalPublisher)
            << "Disabled Publisher: " << PublisherBase::GetTopicName();
    }
}

template <typename MessageT>
std::error_code InternalPublisher<MessageT>::PublishMsg(
    const SendMsg<MessageT>& msg, const uint64_t seq) {
    auto handle = publisher_handle_;
    if (handle == nullptr) {
        AD_LERROR(PublishMsg)
            << "Topic Publish failed, publisher is nullptr!!! topic name:"
            << PublisherBase::GetTopicName();
        return RsclError::kPublisherIsNullptr;
    }

    uint64_t pre_ns = 0;
    std::error_code ret;
    // if middleware support loan buffer, try to calc the loan buffer size,
    // and loan the buffer from middleware.
    if (senseAD::rscl::common::IsFeatureSupported<
            senseAD::rscl::common::SupportedFeature::SUPPORT_LOAN_BUFFER>()) {
        size_t msg_size = msg.SerializedSize();
        auto loan_buffer = publisher_handle_->LoanBuffer(msg_size);
        if (loan_buffer == nullptr) {
            AD_LERROR(InternalPublisher)
                << "Topic Publish failed, loan buffer is nullptr!!! topic name:"
                << PublisherBase::GetTopicName();
            return RsclError::kLoanBufferIsNullptr;
        }
        msg.BuildRawToAllocatedBuffer(seq, loan_buffer->GetData(),
                                      PublisherBase::node_name_,
                                      header_option_);

        pre_ns = senseAD::base::time::CoarseSteadyClockNs();
        ret = publisher_handle_->PublishLoanBuffer(std::move(loan_buffer));
    } else {
        auto out_msg =
            msg.Finalize(seq, PublisherBase::node_name_, header_option_)
                .BorrowRaw();
        pre_ns = senseAD::base::time::CoarseSteadyClockNs();
        ret = publisher_handle_->Publish(out_msg);
    }

    auto time_cost = senseAD::base::time::CoarseSteadyClockNs() - pre_ns;
    if (time_cost > 1e9) {
        AD_LERROR(InternalPublisher)
            << "Topic Publish is blocking!!! " << PublisherBase::GetTopicName()
            << " publish used ns: " << time_cost;
    }
    return ret;
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
