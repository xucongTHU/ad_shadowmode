/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>

#include "cyber/cyber.h"

namespace apollo {
namespace cyber {
namespace message {

/**
 * @brief This is a type used for publish only!
 *
 */
class BorrowedRawMessage {
 public:
    BorrowedRawMessage() {}
    BorrowedRawMessage(const char *_data,
                       size_t _size,
                       const std::shared_ptr<void> _ref)
        : data(_data), size(_size), ref(_ref) {}

    BorrowedRawMessage &operator=(const BorrowedRawMessage &raw_msg) {
        if (this != &raw_msg) {
            this->data = raw_msg.data;
            this->size = raw_msg.size;
        }
        return *this;
    }

    ~BorrowedRawMessage() {}

    class Descriptor {
     public:
        std::string full_name() const {
            return "apollo.cyber.message.RawMessage";
        }
        std::string name() const { return "apollo.cyber.message.RawMessage"; }
    };

    static const Descriptor *descriptor() {
        static Descriptor desc;
        return &desc;
    }

    static void GetDescriptorString(const std::string &type,
                                    std::string *desc_str) {
        ProtobufFactory::Instance()->GetDescriptorString(type, desc_str);
    }

    bool SerializeToArray(void *dst, int size) const {
        if (dst == nullptr || size < ByteSize()) {
            return false;
        }

        memcpy(dst, data, size);
        return true;
    }

    bool SerializeToString(std::string *str) const {
        if (str == nullptr) {
            return false;
        }
        *str = {data, data + size};
        return true;
    }

    bool ParseFromArray(const void *, int) { return true; }

    bool ParseFromString(const std::string &) { return true; }

    int ByteSize() const { return static_cast<int>(size); }
    int ByteSizeLong() const { return static_cast<int>(size); }

    static std::string TypeName() { return "apollo.cyber.message.RawMessage"; }

    const char *data;
    size_t size;

    const std::shared_ptr<void> ref;
};
}  // namespace message
}  // namespace cyber
}  // namespace apollo
