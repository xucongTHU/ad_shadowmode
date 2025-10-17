/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "ad_base/common/shutdownable.h"
#include "ad_log/ad_log.hpp"
#include "ad_msg_idl/std_msgs/header.capnp.h"
#include "ad_mw/ad_mw.h"
#include "ad_serde/ad_serde.h"
#include "ad_diag/base/supervisions/supervision_comm.h"

#include "ad_rscl/idl/runtime_state.capnp.h"
#include "ad_rscl/comm/hetero_channel/channel_factory.h"
#include "ad_rscl/comm/types.h"
#include "ad_rscl/time/time.h"
#include "ad_rscl/common/errcode.h"

namespace senseAD {
namespace rscl {

namespace comm_cs3 {
class RemoteEndpoints;
}

namespace comm {

// Template function for creating a properly referenced BorrowedRawMessage
// General version for all message types
template <typename T>
inline std::shared_ptr<senseAD::serde::BorrowedRawMessage>
CreateSelfRefBorrowedRaw(const std::shared_ptr<SendMsg<T>>& msg) {
    return msg->BorrowRawWithRef(msg);
}

// Specialization for BorrowedRawMessage type
template <>
inline std::shared_ptr<senseAD::serde::BorrowedRawMessage>
CreateSelfRefBorrowedRaw<senseAD::serde::BorrowedRawMessage>(
    const std::shared_ptr<SendMsg<senseAD::serde::BorrowedRawMessage>>& msg) {
    if (msg->MsgBuilder().ref == nullptr) {
        return msg->CopyToRaw();
    } else {
        return msg->BorrowRawWithRef(msg);
    }
}

template <typename MessageT>
class Publisher;

/**
 * @brief Base type for publisher.
 * For the usecase that the user will not call member functions that have
 * message
 * type in its signature
 *
 */
class PublisherBase : public SwitchableNodeMemberBase, public ::Shutdownable {
 public:
    using Ptr = std::shared_ptr<PublisherBase>;
    virtual ~PublisherBase();

    void ShutdownImpl() override;

    /**
     * @brief Publish the message using the derived typed publisher
     *
     * @tparam MessageT
     * @param msg
     * @return whether the provided type fit the underlying typed publisher
     */
    template <typename MessageT>
    bool Publish(const SendMsg<MessageT>& msg) {
        auto self = dynamic_cast<Publisher<MessageT>*>(this);
        if (!self) {
            return false;
        }
        self->Publish(msg);
        return true;
    }

    /**
     * @brief Publish the message using the derived typed publisher
     *
     * @tparam MessageT
     * @param msg
     * @return whether the provided type fit the underlying typed publisher
     */
    template <typename MessageT>
    bool Publish(const std::shared_ptr<SendMsg<MessageT>>& msg) {
        return Publish(*msg);
    }

    /**
     * @brief Get the Topic Name object
     *
     * @return const std::string&
     */
    const std::string& GetTopicName() const;

    /**
     * @brief Get the Publisher Conf object
     *
     * @return const PublisherConf&
     */
    const PublisherConf& GetPublisherConf() const;

    /**
     * @brief Get the Msg Property frame ratio essential info
     *
     * @return const TransmitEval&
     */
    const TransmitEval& GetTransmitEval();

    const TransmitEval& GetTransmitEvalAndReportSupervision();

    /**
     * @brief For convenient, in case your message have a std_msgs::Header
     *
     * @param frame_id
     * @param hdr
     */
    void FillHeader(const std::string& frame_id,
                    senseAD::msg::std_msgs::Header::Builder hdr);
    /**
     * @brief For convenient, in case your message have a std_msgs::Header
     *
     * @param frame_id
     * @param timestamp
     * @param hdr
     */
    void FillHeader(const std::string& frame_id,
                    uint64_t timestamp,
                    senseAD::msg::std_msgs::Header::Builder hdr);

    template <typename MessageT>
    static std::shared_ptr<SendMsg<MessageT>> NewSendMsg() {
        return std::make_shared<SendMsg<MessageT>>();
    }

 protected:
    explicit PublisherBase(const std::string& node_name,
                           const PublisherConf& conf);
    void HeterosPublish(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg);
    void RemotePublish(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg);

    std::string node_name_;
    PublisherConf conf_;
    std::atomic<uint32_t> seq_{0};

    std::vector<std::shared_ptr<hetero::PublisherBase>> hetero_pubs_;
    bool hetero_only_ = false;
    senseAD::diag::CommSupervision supervision_;

    std::shared_ptr<comm_cs3::RemoteEndpoints> remote_eps_;
    std::shared_ptr<CommInstanceCountHelper> instance_cnt_ref_;
};

// For the case that user want to store a typed pointer
template <typename MessageT>
class Publisher : public PublisherBase {
 public:
    using Ptr = std::shared_ptr<Publisher<MessageT>>;

    ~Publisher() override { ::Shutdownable::Shutdown(); }

    void ShutdownImpl() override { PublisherBase::ShutdownImpl(); }

    static std::shared_ptr<SendMsg<MessageT>> NewSendMsg() {
        return std::shared_ptr<SendMsg<MessageT>>(new SendMsg<MessageT>());
    }

    // Need to be virtual, in case that the subclass cannot follow this flow
    inline virtual std::error_code Publish(const SendMsg<MessageT>& msg) {
        if (!hetero_only_) {
            std::error_code ret;
            ret = PublishMsg(msg, channel_seq_);
            if (ret.value() != 0) {
                channel_seq_++;
                return ret;
            }
        }
        PostPublish(msg);
        channel_seq_++;
        return RsclError::kSuccess;
    }

    inline virtual std::error_code Publish(
        const std::shared_ptr<SendMsg<MessageT>>& msg) {
        if (!hetero_only_) {
            std::error_code ret;
            ret = PublishMsg(*msg, channel_seq_);
            if (ret.value() != 0) {
                channel_seq_++;
                return ret;
            }
        }
        PostPublish(msg);
        channel_seq_++;
        return RsclError::kSuccess;
    }

 protected:
    Publisher(const std::string& node_name, const PublisherConf& conf)
        : PublisherBase(node_name, conf) {
        ::Shutdownable::UpdateTTLogDetail("Publisher|" + conf.channel_name());
    }

    virtual std::error_code PublishMsg(const SendMsg<MessageT>& msg,
                                       const uint64_t seq) = 0;

    inline void PostPublishCommon(const SendMsg<MessageT>& msg) {
        if (!msg.IsBufferBuilt()) {
            msg.Finalize(channel_seq_, PublisherBase::node_name_);
        }
        // for calculate frame ratio
        this->transmit_eval() += msg.ByteSize();
    }

    inline void PostPublishBorrowedRaw(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg) {
        if (!hetero_pubs_.empty()) {
            HeterosPublish(msg);
        }
        RemotePublish(msg);
    }

    void PostPublish(const SendMsg<MessageT>& msg) {
        PostPublishCommon(msg);

        if (hetero_pubs_.empty() && !remote_eps_) {
            return;
        }
        auto rmsg = msg.CopyToRaw(channel_seq_, PublisherBase::node_name_);
        PostPublishBorrowedRaw(rmsg);
    }

    void PostPublish(const std::shared_ptr<SendMsg<MessageT>>& msg) {
        PostPublishCommon(*msg);

        if (hetero_pubs_.empty() && !remote_eps_) {
            return;
        }

        auto rmsg = CreateSelfRefBorrowedRaw<MessageT>(msg);
        PostPublishBorrowedRaw(rmsg);
    }

 private:
    std::atomic<uint64_t> channel_seq_{0};
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
