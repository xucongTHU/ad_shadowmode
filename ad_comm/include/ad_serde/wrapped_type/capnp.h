/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#define UNUSED_PARAM(param) (void)(param)

#include <capnp/schema-loader.h>
#include <capnp/serialize-packed.h>
#include <capnp/message.h>
#include <capnp/serialize.h>

#include <string>
#include <memory>
#include <utility>
#include <exception>
#include <vector>

#include "ad_serde/header.h"
#include "ad_serde/wrapped_type/base.h"
#include "ad_serde/wrapped_type/raw.h"
#include "ad_serde/idl/message_schema.capnp.h"
#include "ad_base/common/hash.h"
#include "ad_log/ad_log.hpp"

DEFINE_AD_TYPE_HAS_SUBSTRUCT_TRAIT(HasCapnpPrivate, _capnpPrivate)

//* ==================================================================
//* MsgMeta
//* ==================================================================
namespace senseAD {
namespace serde {

template <class MessageT>
struct MessageTypeMeta<
    MessageT,
    typename std::enable_if<HasCapnpPrivate<MessageT>::value, void>::type> {
    static std::string GetMessageTypename() {
        auto scheme = capnp::Schema::from<MessageT>();
        return std::string(GetMessageTypenamePrefix()) +
               std::string(scheme.getShortDisplayName().cStr()) + "_" +
               std::to_string(scheme.getProto().getId()) + "_" +
               std::to_string(base::common::Hash(GetMessageDescriptor()));
    }
    static constexpr const char* GetMessageTypenamePrefix() { return "CAPNP@"; }

    static std::string GetMessageDescriptor() {
        static std::string ret;
        if (ret.empty()) {
            capnp::MallocMessageBuilder message_builder;
            auto schema_msg =
                message_builder.getRoot<senseAD::serde::idl::MessageSchema>();
            schema_msg.setTypeId(capnp::typeId<MessageT>());
            schema_msg.initPayloadAs<MessageT>();

            capnp::SchemaLoader loader;
            loader.loadCompiledTypeAndDependencies<MessageT>();
            auto schemas_array = loader.getAllLoaded();
            auto listBuilder = schema_msg.initSchemas(schemas_array.size());
            for (auto i : kj::indices(schemas_array)) {
                listBuilder.setWithCaveats(i, schemas_array[i].getProto());
            }
            auto schema_flatarray = capnp::messageToFlatArray(message_builder);
            auto schema_bytes = schema_flatarray.asBytes();
            static std::once_flag assign_flag;
            std::call_once(assign_flag, [&]() {
                ret.assign(schema_bytes.begin(), schema_bytes.end());
            });
        }
        return ret;
    }
};

struct MsgMeta;
bool CheckDescriptorCompatible(const std::string& pub_desc,
                               const std::string& sub_desc);
bool CompareChannelMeta(const MsgMeta& lm, const MsgMeta& rm);

}  // namespace serde
}  // namespace senseAD

//* ==================================================================
//* ReceivedMsg
//* ==================================================================

template <typename MessageT>
struct ReceivedMsg<
    MessageT,
    typename std::enable_if<HasCapnpPrivate<MessageT>::value, void>::type> {
 public:
    explicit ReceivedMsg(
        const std::shared_ptr<senseAD::serde::BorrowedRawMessage> e)
        : may_owned_msg(e),
          kjarr(reinterpret_cast<const capnp::word*>(may_owned_msg->data),
                may_owned_msg->size) {
        try {
            message_ptr = std::shared_ptr<::capnp::FlatArrayMessageReader>(
                new ::capnp::FlatArrayMessageReader(kjarr,
                                                    {kTraversalLimitInWords}));
            reader = message_ptr->getRoot<MessageT>();
        } catch (const kj::Exception&) {
            valid_ = false;
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
    explicit ReceivedMsg(const typename MessageT::Reader existed_msg,
                         int seq = 0,
                         const std::string& pub_node_name = "") {
        // Copy to capnp builder
        ::capnp::MallocMessageBuilder msg;
        msg.setRoot(existed_msg);
        // serialization
        auto out_kjarr = capnp::messageToFlatArrayWithHeader(
            msg.getSegmentsForOutput(), seq, pub_node_name);
        // Copy
        auto data_store = std::make_shared<std::vector<char>>(
            reinterpret_cast<uint8_t*>(out_kjarr.begin()),
            reinterpret_cast<uint8_t*>(out_kjarr.begin()) +
                out_kjarr.size() * sizeof(capnp::word));
        may_owned_msg = std::make_shared<senseAD::serde::BorrowedRawMessage>(
            data_store->data(), data_store->size(), data_store);
        // Rebuild read structure
        kjarr = kj::ArrayPtr<const capnp::word>(
            reinterpret_cast<const capnp::word*>(may_owned_msg->data),
            may_owned_msg->size);
        message_ptr = std::shared_ptr<::capnp::FlatArrayMessageReader>(
            new ::capnp::FlatArrayMessageReader(kjarr,
                                                {kTraversalLimitInWords}));
        reader = message_ptr->getRoot<MessageT>();

        valid_ = true;
    }

    ~ReceivedMsg() noexcept(true) {}

    /**
     * @brief The raw bytes for the underlying message
     *
     * @return const char*
     */
    inline const char* Bytes() const { return may_owned_msg->data; }
    /**
     * @brief The raw bytes size for the underlying message
     *
     * @return size_t
     */
    inline size_t ByteSize() const { return may_owned_msg->size; }

    inline HeaderT Header() {
        HeaderT header;
        ParseHeaderFromBufferTail(may_owned_msg->data, may_owned_msg->size,
                                  &header);
        return header;
    }

    /**
     * @brief Capnp message reader
     *
     * @return MessageT::Reader&
     */
    inline typename MessageT::Reader& Msg() const { return reader; }

    inline bool IsValid() const { return valid_; }
    static constexpr bool DataIntegrityTrusted() { return false; }

 private:
    std::shared_ptr<senseAD::serde::BorrowedRawMessage> may_owned_msg;
    kj::ArrayPtr<const capnp::word> kjarr;
    std::shared_ptr<::capnp::FlatArrayMessageReader> message_ptr;
    bool valid_ = true;
    mutable typename MessageT::Reader reader;
};

//* ==================================================================
//* SendMsg
//* ==================================================================

template <typename MessageT>
struct SendMsg<
    MessageT,
    typename std::enable_if<HasCapnpPrivate<MessageT>::value, void>::type> {
 public:
    explicit SendMsg(const typename MessageT::Reader existed_msg)
        : msg(std::make_unique<::capnp::MallocMessageBuilder>()),
          msg_builder(nullptr),
          init_build(false) {
        msg->setRoot(existed_msg);
        msg_builder = msg->getRoot<MessageT>();
    }
    SendMsg()
        : msg(std::make_unique<::capnp::MallocMessageBuilder>()),
          msg_builder(msg->initRoot<MessageT>()),
          init_build(false) {}
    SendMsg(SendMsg&& rhs)
        : msg(std::move(rhs.msg)),
          msg_builder(msg->getRoot<MessageT>()),
          init_build(rhs.init_build),
          kjarr(std::move(rhs.kjarr)) {}
    ~SendMsg() noexcept(true) {}

    /**
     * @brief Capnp message builder
     *
     * @return MessageT::Builder
     */
    inline typename MessageT::Builder MsgBuilder() { return msg_builder; }
    inline typename MessageT::Reader Msg() const {
        return msg_builder.asReader();
    }
    inline bool IsBufferBuilt() const { return init_build; }
    /**
     * @brief Important: The lifetime of the return borrowed message must be
     * shorter than SendMsg<T>. Otherwise, it cause segment fault
     *
     * @return std::shared_ptr<senseAD::serde::BorrowedRawMessage>
     */

    const SendMsg<MessageT>& Finalize(
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        //! Allow repeated build to compat to the message reuse
        // if (init_build) {
        //     throw senseAD::serde::SendMsgBufferRepeatedBuildException();
        // }
        kjarr = capnp::messageToFlatArrayWithHeader(msg->getSegmentsForOutput(),
                                                    0, "", option);
        init_build = true;
        return *this;
    }

    const SendMsg<MessageT>& Finalize(
        uint64_t seq,
        const std::string& pub_node_name = "",
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        //! Allow repeated build to compat to the message reuse
        // if (init_build) {
        //     throw senseAD::serde::SendMsgBufferRepeatedBuildException();
        // }
        kjarr = capnp::messageToFlatArrayWithHeader(msg->getSegmentsForOutput(),
                                                    seq, pub_node_name, option);
        init_build = true;
        return *this;
    }

    /**
     * Build a raw message to the target buffer.
     * If you want to loan the buffer, please call the SerializedSize() first to
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
        capnp::messageToBufferWithHeader(msg->getSegmentsForOutput(), seq,
                                         target, pub_node_name, option);
    }

    /**
     * Calculates the serialized size of the message in words.
     *
     * @return The size of the message in bytes.
     */
    size_t SerializedSize(
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        return capnp::computeSerializedSizeInWordsWithHeader(
                   msg->getSegmentsForOutput(), option) *
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
        if (!init_build) {
            throw senseAD::serde::AccessSendMsgBufferBeforeBuildException(
                "BorrowRawWithRef");
        }
        auto bytes = kjarr.asBytes();
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            reinterpret_cast<char*>(bytes.begin()), bytes.size(), ref);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRaw() const {
        return BorrowRawWithRef(nullptr);
    }

    std::string SerializeAsString() const {
        auto raw_message = this->Finalize().BorrowRaw();
        std::string data{raw_message->data,
                         raw_message->data + raw_message->size};
        return data;
    }

    /**
     * @brief Copy the underly buffer into a new memory
     *
     * @return std::shared_ptr<senseAD::serde::BorrowedRawMessage>
     */
    std::shared_ptr<senseAD::serde::BorrowedRawMessage> CopyToRaw() const {
        std::shared_ptr<kj::Array<capnp::word>> heap_kjarr;
        heap_kjarr = std::make_shared<kj::Array<capnp::word>>(
            capnp::messageToFlatArray(*msg));
        auto bytes = heap_kjarr->asBytes();
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            reinterpret_cast<char*>(bytes.begin()), bytes.size(), heap_kjarr);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> CopyToRaw(
        uint64_t seq, const std::string& pub_node_name = "") const {
        std::shared_ptr<kj::Array<capnp::word>> heap_kjarr;
        heap_kjarr = std::make_shared<kj::Array<capnp::word>>(
            capnp::messageToFlatArrayWithHeader(msg->getSegmentsForOutput(),
                                                seq, pub_node_name));
        auto bytes = heap_kjarr->asBytes();
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            reinterpret_cast<char*>(bytes.begin()), bytes.size(), heap_kjarr);
    }

    inline size_t ByteSize() const {
        if (!init_build) {
            throw senseAD::serde::AccessSendMsgBufferBeforeBuildException(
                "ByteSize");
        }
        auto bytes = kjarr.asBytes();
        return bytes.size();
    }

 private:
    mutable std::unique_ptr<::capnp::MallocMessageBuilder> msg;
    typename MessageT::Builder msg_builder;
    mutable bool init_build;
    mutable kj::Array<capnp::word> kjarr;
};
