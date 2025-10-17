/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#define UNUSED_PARAM(param) (void)(param)

#include <capnp/message.h>
#include <capnp/serialize-packed.h>

#include <string>
#include <memory>
#include <utility>
#include <exception>
#include <vector>

#include "ad_serde/header.h"
#include "ad_serde/wrapped_type/base.h"
#include "ad_serde/wrapped_type/raw.h"

DEFINE_AD_TYPE_HAS_FUNC_TRAIT(HasSerializeToCapnp, SerializeToCapnp)
DEFINE_AD_TYPE_HAS_FUNC_TRAIT(HasDeserializeFromCapnp, DeserializeFromCapnp)
DEFINE_AD_TYPE_HAS_SUBSTRUCT_TRAIT(HasSubstructCapnpType, CapnpType)

//* ==================================================================
//* MsgMeta
//* ==================================================================
namespace senseAD {
namespace serde {

template <class MessageT>
struct MessageTypeMeta<
    MessageT,
    typename std::enable_if<HasSubstructCapnpType<MessageT>::value,
                            void>::type> {
    static constexpr const char* GetMessageTypenamePrefix() {
        return MessageTypeMeta<
            typename MessageT::CapnpType>::GetMessageTypenamePrefix();
    }

    static std::string GetMessageTypename() {
        return MessageTypeMeta<
            typename MessageT::CapnpType>::GetMessageTypename();
    }
    static std::string GetMessageDescriptor() {
        return MessageTypeMeta<
            typename MessageT::CapnpType>::GetMessageDescriptor();
    }
};

}  // namespace serde
}  // namespace senseAD

//* ==================================================================
//* ReceivedMsg
//* ==================================================================

template <typename MessageT>
struct ReceivedMsg<
    MessageT,
    typename std::enable_if<HasSerializeToCapnp<MessageT>::value &&
                                HasDeserializeFromCapnp<MessageT>::value,
                            void>::type> {
 public:
    using CapnpType = typename MessageT::CapnpType;

    explicit ReceivedMsg(
        const std::shared_ptr<senseAD::serde::BorrowedRawMessage> e)
        : may_owned_msg_(e) {
        kj::ArrayPtr<const capnp::word> kjarr(
            reinterpret_cast<const capnp::word*>(e->data), e->size);

        try {
            ::capnp::FlatArrayMessageReader message_reader(
                kjarr, {kTraversalLimitInWords});
            auto reader = message_reader.getRoot<CapnpType>();
            valid_ = msg_.DeserializeFromCapnp(reader);
        } catch (const kj::Exception&) {
            valid_ = false;
        }

        if (valid_) {
            ParseHeaderFromBufferTail(e->data, e->size, &header_);
        }
    }

    /**
     * @brief Construct a new Received Msg object
     * !This constructor could ONLY used in below case
     * - no sender node to set header (msg bridge case)
     *
     * @param existed_msg
     * @param seq
     */
    explicit ReceivedMsg(const MessageT& existed_msg,
                         int seq = 0,
                         const std::string& pub_node_name = "") {
        msg_ = existed_msg;
        FillHeader(&header_, seq, pub_node_name, RsclMsgHeaderOptions(),
                   nullptr, 0);
        valid_ = true;
    }

    ~ReceivedMsg() noexcept(true) {}

    inline const char* Bytes() const {
        return may_owned_msg_ ? may_owned_msg_->data : nullptr;
    }
    inline size_t ByteSize() const {
        return may_owned_msg_ ? may_owned_msg_->size : 0;
    }

    inline HeaderT Header() { return header_; }

    inline const MessageT& Msg() const { return msg_; }

    inline bool IsValid() const { return valid_; }
    static constexpr bool DataIntegrityTrusted() { return true; }

 private:
    std::shared_ptr<senseAD::serde::BorrowedRawMessage> may_owned_msg_;

    bool valid_ = true;

    HeaderT header_;
    MessageT msg_;
};

//* ==================================================================
//* SendMsg
//* ==================================================================

template <typename MessageT>
struct SendMsg<
    MessageT,
    typename std::enable_if<HasSerializeToCapnp<MessageT>::value &&
                                HasDeserializeFromCapnp<MessageT>::value,
                            void>::type> {
 public:
    using CapnpType = typename MessageT::CapnpType;

    explicit SendMsg(const MessageT& existed_msg) { msg_ = existed_msg; }
    SendMsg() {}
    SendMsg(SendMsg&& rhs)
        : msg_(std::move(rhs.msg_)),
          init_build_msg_builder_(rhs.init_build_msg_builder_),
          init_build_buf_(rhs.init_build_buf_),
          kjarr_(std::move(rhs.kjarr_)) {}
    ~SendMsg() noexcept(true) {}

    /**
     * @brief Capnp message builder
     *
     * @return MessageT::Builder
     */
    inline MessageT* MsgBuilder() { return &msg_; }
    inline const MessageT& Msg() const { return msg_; }

    /**
     * @brief Important: The lifetime of the return borrowed message must be
     * shorter than SendMsg<T>. Otherwise, it cause segment fault
     *
     * @return std::shared_ptr<senseAD::serde::BorrowedRawMessage>
     */
    const SendMsg<MessageT>& Finalize(
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        //! Allow repeated build to compat to the message reuse
        // if (init_build_buf_) {
        //     throw senseAD::serde::SendMsgBufferRepeatedBuildException();
        // }
        BuildMsgBuilder();
        kjarr_ = capnp::messageToFlatArrayWithHeader(
            msg_builder_->getSegmentsForOutput(), 0, "", option);
        init_build_buf_ = true;
        return *this;
    }

    const SendMsg<MessageT>& Finalize(
        uint64_t seq,
        const std::string& pub_node_name = "",
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        //! Allow repeated build to compat to the message reuse
        // if (init_build_buf_) {
        //     throw senseAD::serde::SendMsgBufferRepeatedBuildException();
        // }
        BuildMsgBuilder();
        kjarr_ = capnp::messageToFlatArrayWithHeader(
            msg_builder_->getSegmentsForOutput(), seq, pub_node_name, option);
        init_build_buf_ = true;
        return *this;
    }

    inline bool IsBufferBuilt() const { return init_build_buf_; }
    /**
     * Build a raw message to the target buffer.
     * If you want to loan the buffer, please call the SerializedSize() first
     to
     * calculate the size.
     *
     * @param seq the sequence number
     * @param target the target buffer to write the raw message to
     */
    void BuildRawToAllocatedBuffer(
        uint64_t seq,
        uint8_t* target,
        const std::string& pub_node_name,
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        BuildMsgBuilder();
        capnp::messageToBufferWithHeader(msg_builder_->getSegmentsForOutput(),
                                         seq, target, pub_node_name, option);
    }

    /**
     * Calculates the serialized size of the message in words.
     *
     * @return The size of the message in bytes.
     */
    size_t SerializedSize(
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        BuildMsgBuilder();
        return capnp::computeSerializedSizeInWordsWithHeader(
                   msg_builder_->getSegmentsForOutput(), option) *
               sizeof(capnp::word);
    }

    /**
     * @brief Get the finalized buffer
     *
     * @param ref
     * @return std::shared_ptr<senseAD::serde::BorrowedRawMessage>
     */

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRawWithRef(
        const std::shared_ptr<void> ref) const {
        if (!init_build_buf_) {
            throw senseAD::serde::AccessSendMsgBufferBeforeBuildException(
                "BorrowRawWithRef");
        }
        auto bytes = kjarr_.asBytes();
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            reinterpret_cast<char*>(bytes.begin()), bytes.size(), ref);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRaw(
        uint64_t seq) const {
        return BorrowRawWithRef(nullptr, seq);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRaw() const {
        return BorrowRawWithRef(nullptr);
    }

    /**
     * @brief Copy the underly buffer into a new memory
     *
     * @return std::shared_ptr<senseAD::serde::BorrowedRawMessage>
     */
    std::shared_ptr<senseAD::serde::BorrowedRawMessage> CopyToRaw() const {
        BuildMsgBuilder();

        auto heap_kjarr = std::make_shared<kj::Array<capnp::word>>(
            capnp::messageToFlatArrayWithHeader(
                msg_builder_->getSegmentsForOutput(), 0, ""));
        auto bytes = heap_kjarr->asBytes();
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            reinterpret_cast<char*>(bytes.begin()), bytes.size(), heap_kjarr);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> CopyToRaw(
        uint64_t seq, const std::string& pub_node_name = "") const {
        BuildMsgBuilder();

        auto heap_kjarr = std::make_shared<kj::Array<capnp::word>>(
            capnp::messageToFlatArrayWithHeader(
                msg_builder_->getSegmentsForOutput(), seq, pub_node_name));
        auto bytes = heap_kjarr->asBytes();
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            reinterpret_cast<char*>(bytes.begin()), bytes.size(), heap_kjarr);
    }

    inline size_t ByteSize() const {
        if (!init_build_buf_) {
            throw senseAD::serde::AccessSendMsgBufferBeforeBuildException(
                "ByteSize");
        }
        auto bytes = kjarr_.asBytes();
        return bytes.size();
    }

 private:
    void BuildMsgBuilder() const {
        if (!init_build_msg_builder_) {
            msg_builder_ = std::make_unique<::capnp::MallocMessageBuilder>();
            msg_.SerializeToCapnp(msg_builder_->initRoot<CapnpType>());
            init_build_msg_builder_ = true;
        }
    }

    mutable std::unique_ptr<::capnp::MallocMessageBuilder> msg_builder_;
    mutable bool init_build_msg_builder_ = false;
    mutable bool init_build_buf_ = false;
    mutable kj::Array<capnp::word> kjarr_;

    MessageT msg_;
};
