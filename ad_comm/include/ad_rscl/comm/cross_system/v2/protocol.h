/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <vector>

#include "ad_base/std/optional.h"
#include "ad_rscl/comm/cross_system/v2/common.h"

namespace senseAD {
namespace rscl {
namespace comm_cs {

/**
 * @brief Protocol for the interaction between hosts
 *  Packet Layout:
 *  |Content Buffer|Header Buffer|size for header buffer(32bit)|
 *
 */

// Layout: |0|RootMessage(SyncStateMessage)|size|
struct SyncStateMessage {
    // std::vector<key_t> changed_pub_key;
    // std::vector<key_t> changed_sub_key;
    RuntimeState runtime_state;

    template <class Archive>
    void serialize(Archive& ar) {  // NOLINT
        ar(runtime_state);
        // ar(changed_pub_key, changed_sub_key, runtime_state);
    }
};

// Assume we will not mix different topic data to a same SendMessage
// Layout: |sum(msg_sizes)|RootMessage(SendMessage)|size|
struct SentMessage {
    key_t topic_key;
    uint32_t n_msg;
    // max 5 msg in one message
    uint32_t msg_sizes[5];

    template <class Archive>
    void serialize(Archive& ar) {  // NOLINT
        ar(topic_key, n_msg, msg_sizes);
    }
};

enum struct MessageType : uint8_t {
    SYNC = 1,
    SYNC_REQ = 2,
    DATA = 3,
};

struct RootMessage {
    // Assume to be unique
    AddrInfo host_addr;

    // sent time
    uint64_t rscl_timestamp;

    MessageType type;

    SyncStateMessage sync_msg;
    SentMessage sent_msg;

    template <class Archive>
    void serialize(Archive& ar) {  // NOLINT
        ar(host_addr, rscl_timestamp, type, sync_msg, sent_msg);
    }
};

base::optional<RootMessage> ParseBuffer(const char* buf,
                                        size_t size,
                                        uint32_t* seq = nullptr);

}  // namespace comm_cs
}  // namespace rscl
}  // namespace senseAD
