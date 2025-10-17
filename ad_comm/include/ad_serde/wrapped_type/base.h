/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <memory>
#include <string>
#include <utility>

#include "ad_serde/header.h"
#include "ad_serde/exception.h"

typedef RsclMsgHeader HeaderT;
constexpr uint64_t kTraversalLimitInWords = 16 * 1024 * 1024;

namespace senseAD {
namespace serde {
constexpr char kEmptyTypeName[] = "RawMessage";

template <class MessageT, class E = void>
struct MessageTypeMeta {
    static constexpr const char* GetMessageTypenamePrefix();

    static std::string GetMessageTypename();
    static std::string GetMessageDescriptor();
};

class RawMessage {
 public:
    using Reader = std::string;
    using Builder = std::string&;

    RawMessage() = default;
    RawMessage(const RawMessage& other) : data(other.data) {}
    RawMessage(RawMessage&& other) : data(std::move(other.data)) {}
    std::string data;
};
class BorrowedRawMessage {
 public:
    using Reader = BorrowedRawMessage;
    using Builder = BorrowedRawMessage&;

    BorrowedRawMessage() {}
    BorrowedRawMessage(const char* _data,
                       size_t _size,
                       const std::shared_ptr<void> _ref)
        : data(_data), size(_size), ref(_ref) {}
    // Must let the user to explicit copy
    BorrowedRawMessage(const BorrowedRawMessage& other) = delete;
    BorrowedRawMessage(BorrowedRawMessage&& other)
        : data(other.data), size(other.size), ref(std::move(other.ref)) {}

    const char* data = nullptr;
    size_t size = 0;

    // In case, we want to hold the source data pointer
    const std::shared_ptr<void> ref;
};

}  // namespace serde

// Keep for compatiable
namespace mw {
using RawMessage = senseAD::serde::RawMessage;
using BorrowedRawMessage = senseAD::serde::BorrowedRawMessage;
}  // namespace mw
}  // namespace senseAD

template <class MessageT, class E = void>
struct ReceivedMsg {
 public:
    explicit ReceivedMsg(
        const std::shared_ptr<senseAD::serde::BorrowedRawMessage>);

    explicit ReceivedMsg(const typename MessageT::Reader, int);

    ~ReceivedMsg() noexcept(true);

    const char* Bytes() const;
    size_t ByteSize() const;

    inline bool IsValid() const;
    static constexpr bool DataIntegrityTrusted();
};

template <typename MessageT, class E = void>
struct SendMsg {
 public:
    explicit SendMsg(const typename MessageT::Reader);

    SendMsg();
    SendMsg(SendMsg&& rhs);
    ~SendMsg() noexcept(true);

    std::shared_ptr<senseAD::serde::BorrowedRawMessage> BorrowRaw() const;
};
