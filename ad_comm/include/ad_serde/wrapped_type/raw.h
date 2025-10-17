/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#define UNUSED_PARAM(param) (void)(param)

#include <string>
#include <memory>
#include <utility>
#include <exception>
#include <vector>

#include "ad_serde/header.h"
#include "ad_serde/wrapped_type/base.h"

//* ==================================================================
//* MsgMeta
//* ==================================================================

namespace senseAD {
namespace serde {

template <>
struct MessageTypeMeta<senseAD::serde::RawMessage, void> {
    constexpr const char* GetMessageTypenamePrefix() { return ""; }

    inline static std::string GetMessageTypename() { return kEmptyTypeName; }
    inline static std::string GetMessageDescriptor() { return ""; }
};

template <>
struct MessageTypeMeta<senseAD::serde::BorrowedRawMessage, void> {
    constexpr const char* GetMessageTypenamePrefix() { return ""; }

    inline static std::string GetMessageTypename() { return kEmptyTypeName; }
    inline static std::string GetMessageDescriptor() { return ""; }
};

}  // namespace serde
}  // namespace senseAD

//* ==================================================================
//* ReceivedMsg
//* ==================================================================

template <>
struct ReceivedMsg<senseAD::serde::RawMessage, void> {
 public:
    explicit ReceivedMsg(
        const std::shared_ptr<senseAD::serde::BorrowedRawMessage> e)
        : may_owned_msg(e) {}
    ~ReceivedMsg() noexcept(true) {}

    inline const char* Bytes() const { return may_owned_msg->data; }
    inline size_t ByteSize() const { return may_owned_msg->size; }

    inline HeaderT Header() const {
        HeaderT header;
        ParseHeaderFromBufferTail(may_owned_msg->data, may_owned_msg->size,
                                  &header);
        return header;
    }
    inline bool IsValid() const { return true; }
    static constexpr bool DataIntegrityTrusted() { return true; }

 private:
    std::shared_ptr<senseAD::serde::BorrowedRawMessage> may_owned_msg;
};

//* ==================================================================
//* SendMsg
//* ==================================================================

template <>
struct SendMsg<senseAD::serde::RawMessage, void> {
 public:
    explicit SendMsg(const senseAD::serde::RawMessage::Reader& existed_msg) {
        raw_msg.data = existed_msg;
    }
    explicit SendMsg(senseAD::serde::RawMessage::Reader&& existed_msg) {
        raw_msg.data = std::move(existed_msg);
    }

    SendMsg() {}
    ~SendMsg() noexcept(true) {}

    inline senseAD::serde::RawMessage::Builder MsgBuilder() {
        return raw_msg.data;
    }
    inline const senseAD::serde::RawMessage::Reader& Msg() const {
        return raw_msg.data;
    }

    inline bool IsBufferBuilt() const { return true; }

    const SendMsg<senseAD::serde::RawMessage>& Finalize(
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        UNUSED_PARAM(option);
        return *this;
    }
    const SendMsg<senseAD::serde::RawMessage>& Finalize(
        uint64_t seq,
        const std::string& pub_node_name,
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        UNUSED_PARAM(seq);
        UNUSED_PARAM(pub_node_name);
        UNUSED_PARAM(option);
        return *this;
    }

    /**
     * Build a raw message to the target buffer.
     * If you want to loan the buffer, please call the SerializedSize() first to
     * calculate the size.
     *
     * @param seq the sequence number
     * @param target the target buffer to write the raw message to
     * @param pub_node_name the publisher node name
     */
    void BuildRawToAllocatedBuffer(
        uint64_t seq,
        uint8_t* target,
        const std::string& pub_node_name,
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        UNUSED_PARAM(seq);
        UNUSED_PARAM(pub_node_name);
        UNUSED_PARAM(option);
        memcpy(target, &raw_msg.data[0], raw_msg.data.size());
    }

    /**
     * Calculates the serialized size of the message in words.
     * @return The size of the message in bytes.
     */
    size_t SerializedSize() const { return raw_msg.data.size(); }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRawWithRef(
        const std::shared_ptr<void> ref) const {
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            &raw_msg.data[0], raw_msg.data.size(), ref);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRaw() const {
        return BorrowRawWithRef(nullptr);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> CopyToRaw() const {
        auto heap_raw_msg =
            std::make_shared<senseAD::serde::RawMessage>(raw_msg);
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            &heap_raw_msg->data[0], heap_raw_msg->data.size(), heap_raw_msg);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> CopyToRaw(
        uint64_t seq, const std::string& pub_node_name) const {
        UNUSED_PARAM(seq);
        UNUSED_PARAM(pub_node_name);
        auto heap_raw_msg =
            std::make_shared<senseAD::serde::RawMessage>(raw_msg);
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            &heap_raw_msg->data[0], heap_raw_msg->data.size(), heap_raw_msg);
    }

    inline size_t ByteSize() const { return raw_msg.data.size(); }

 private:
    mutable senseAD::serde::RawMessage raw_msg;
};

template <>
struct SendMsg<senseAD::serde::BorrowedRawMessage, void> {
 public:
    explicit SendMsg(
        const senseAD::serde::BorrowedRawMessage::Reader& existed_msg)
        : raw_msg(existed_msg.data, existed_msg.size, existed_msg.ref) {}
    SendMsg() {}

    inline senseAD::serde::BorrowedRawMessage::Builder MsgBuilder() {
        return raw_msg;
    }
    inline const senseAD::serde::BorrowedRawMessage::Reader& Msg() const {
        return raw_msg;
    }

    inline bool IsBufferBuilt() const { return true; }

    const SendMsg<senseAD::serde::BorrowedRawMessage>& Finalize(
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        UNUSED_PARAM(option);
        return *this;
    }
    const SendMsg<senseAD::serde::BorrowedRawMessage>& Finalize(
        uint64_t seq,
        const std::string& pub_node_name,
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        UNUSED_PARAM(seq);
        UNUSED_PARAM(pub_node_name);
        UNUSED_PARAM(option);
        return *this;
    }

    /**
     * Build a raw message to the target buffer.
     * If you want to loan the buffer, please call the SerializedSize() first to
     * calculate the size.
     *
     * @param seq the sequence number
     * @param target the target buffer to write the raw message to
     * @param pub_node_name the publisher node name
     */
    void BuildRawToAllocatedBuffer(
        uint64_t seq,
        uint8_t* target,
        const std::string& pub_node_name,
        const RsclMsgHeaderOptions& option = RsclMsgHeaderOptions()) const {
        UNUSED_PARAM(seq);
        UNUSED_PARAM(pub_node_name);
        UNUSED_PARAM(option);
        memcpy(target, raw_msg.data, raw_msg.size);
    }

    /**
     * Calculates the serialized size of the message in words.
     * @return The size of the message in bytes.
     */
    size_t SerializedSize() const { return raw_msg.size; }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRawWithRef(
        const std::shared_ptr<void> ref) const {
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            raw_msg.data, raw_msg.size, ref);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRaw() const {
        return BorrowRawWithRef(nullptr);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> CopyToRaw() const {
        auto heap_raw_msg = std::make_shared<senseAD::serde::RawMessage>();
        heap_raw_msg->data.resize(raw_msg.size);
        memcpy(&heap_raw_msg->data[0], raw_msg.data, raw_msg.size);
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            &heap_raw_msg->data[0], heap_raw_msg->data.size(), heap_raw_msg);
    }

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> CopyToRaw(
        uint64_t seq, const std::string& pub_node_name) const {
        UNUSED_PARAM(seq);
        UNUSED_PARAM(pub_node_name);
        auto heap_raw_msg = std::make_shared<senseAD::serde::RawMessage>();
        heap_raw_msg->data.resize(raw_msg.size);
        memcpy(&heap_raw_msg->data[0], raw_msg.data, raw_msg.size);
        return std::make_shared<senseAD::serde::BorrowedRawMessage>(
            &heap_raw_msg->data[0], heap_raw_msg->data.size(), heap_raw_msg);
    }

    inline size_t ByteSize() const { return raw_msg.size; }

 private:
    mutable senseAD::serde::BorrowedRawMessage raw_msg;
};
