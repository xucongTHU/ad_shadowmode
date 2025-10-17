/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include "ad_mw_cyber/message/borrowed_rawmessage.h"

namespace apollo {
namespace cyber {
namespace message {

// Template specialization for BorrowedRawMessage
inline bool SerializeToArray(const BorrowedRawMessage& message,
                             void* data,
                             int size) {
    return message.SerializeToArray(data, size);
}

inline bool ParseFromArray(const void* data,
                           int size,
                           BorrowedRawMessage* message) {
    return message->ParseFromArray(data, size);
}

inline int ByteSize(const BorrowedRawMessage& message) {
    return message.ByteSize();
}

}  // namespace message
}  // namespace cyber
}  // namespace apollo
