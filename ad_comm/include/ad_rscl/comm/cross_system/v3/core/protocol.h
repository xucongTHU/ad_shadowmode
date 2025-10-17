/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <vector>
#include <string>

#include "ad_base/std/optional.h"
#include "ad_rscl/comm/cross_system/v3/core/common.h"
#include "ad_rscl/comm/cross_system/v3/core/system_state.h"
#include "ad_base/common/socket.h"

namespace senseAD {
namespace rscl {
namespace comm_cs3 {

//============================================
//* For the tcp connection between swc client <-> data dispatcher
//* - Only send the topic directly
//============================================

enum class DispatcherMsgType { INVALID, REG_MESSAGE, DATA_MESSAGE };
#pragma pack(push, 1)

//! Layout for TopicRegMessage
//! -> |TopicRegMessage|RootMessageHeader|

//! Layout for Topic Message Data
//! -> |sum(msg_sizes)|SendMessage|RootMessageHeader|

//! Expected Data Sequence
//! -> (TopicRegMessage) > (Topic Message Data) > ... > (Topic Message Data)

// Assume we will not mix different topic data to a same SendMessage
struct SendMessage {
    TopicID topic_id;
    // seq for each message in peer + topic is UNIQUE
    uint64_t peer_topic_seq{0};

    uint32_t n_msg{0};
    // max 5 msg in one message
    uint32_t msg_sizes[5];
};

struct RootMessageHeader {
    uint64_t peer_seq{0};
    // sent time
    uint64_t rscl_timestamp{0};
    bool is_reg_message{false};
};

#pragma pack(pop)

struct TopicRegMessage {
    std::string topic_name;
    TopicID topic_id;

    std::string type_name;
    std::string descriptor;

    template <class Archive>
    void serialize(Archive& ar) {  // NOLINT
        ar(topic_name, topic_id, type_name, descriptor);
    }
};

struct MsgData {
    MsgData() = default;
    MsgData(const char* _data_ptr, const size_t _data_size)
        : data_ptr(_data_ptr), data_size(_data_size) {}

    const char* data_ptr{nullptr};
    size_t data_size{0};
};

bool SendDatasToDataDispatcher(senseAD::base::common::TcpFrameClient* cli,
                               const RootMessageHeader& packet_header,
                               const SendMessage& message_header,
                               const std::vector<MsgData>& datas);
bool SendTopicRegMessageToDataDispatcher(
    senseAD::base::common::TcpFrameClient* cli,
    const RootMessageHeader& packet_header,
    const TopicRegMessage& message);

DispatcherMsgType DataDispatcherCheckMsg(const char* buffer_ptr,
                                         const size_t buffer_size);
bool DataDispatcherParseDatas(const char* buffer_ptr,
                              const size_t buffer_size,
                              RootMessageHeader* packet_header,
                              SendMessage* message_header,
                              std::vector<MsgData>* datas);
bool DataDispatcherParseTopicRegMessage(const char* buffer_ptr,
                                        const size_t buffer_size,
                                        RootMessageHeader* packet_header,
                                        TopicRegMessage* message);

constexpr char kReRegMsg[]{"RE_REG"};
//============================================
//* For the rpc between core -> core (InetRPC)
//============================================
//! Expected Data Sequence
//! (core1) -> req state -> (core2)
//! (core1) <- resp state <- (core2)
//! (core1) <- req state <- (core2)
//! (core1) -> resp state -> (core2)

using senseAD::rscl::cs3::idl::CoreToCoreReq;
using senseAD::rscl::cs3::idl::CoreToCoreResp;

//============================================
//* For the rpc between swc client -> core service (InetRPC)
//============================================

using senseAD::rscl::cs3::idl::SwcCliToCoreReq;
using senseAD::rscl::cs3::idl::SwcCliToCoreResp;

}  // namespace comm_cs3
}  // namespace rscl
}  // namespace senseAD
