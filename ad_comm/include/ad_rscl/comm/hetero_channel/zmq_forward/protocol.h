/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <utility>

#include "ad_base/common/socket.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

constexpr uint32_t kMaxTopicNameLength = 256;
#pragma pack(push, 1)

// Control Action
constexpr uint8_t kControlAction_PubDescriptor = 1;
constexpr uint8_t kControlAction_PauseDataSend = 2;
constexpr uint8_t kControlAction_ResumeDataSend = 3;
constexpr uint8_t kControlAction_ReSendLastData = 4;
constexpr const char* kBroadcastUuid = "xxxxxxxxxxxxxxxx";

struct ControlPacket {
    char uuid[16];
    uint8_t action;
};

// Data Header
constexpr uint8_t kDataHeaderType_RegTopic = 1;
constexpr uint8_t kDataHeaderType_MsgData = 2;

// one data packet = |DataHeader|DataRegTopicDescriptor or DataMsgData|
struct DataHeader {
    uint8_t type;
    uint8_t n_msg = 1;
    char uuid[16];
    char source_ip[16];
    uint64_t seq;
    uint64_t pub_time;

    // Need ensure the byte size of DataHeader is a multiple of 8
    char reserved[6];
};

static_assert(sizeof(DataHeader) % 8 == 0,
              "Need ensure the byte size of DataHeader is a multiple of 8");

// |sizeof(DataRegTopicDescriptor)|typename_size|descriptor_size|
struct DataRegTopicDescriptor {
    char topic[kMaxTopicNameLength];
    uint64_t type_size;
    uint64_t descriptor_size;
};

// |sizeof(DataMsgData)|msg_size|
struct DataMsgData {
    uint64_t msg_size;
};

#pragma pack(pop)

struct HostInfoRequest {
    std::string user_name;
    std::string user_ip;
    std::string process_name;
    bool is_online;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(user_name, user_ip, process_name, is_online);
    }
};

struct HostInfoRequestHash {
    size_t operator()(const HostInfoRequest& src) const {
        std::string hash_key = src.user_name + src.user_ip + src.process_name;
        return std::hash<std::string>()(hash_key);
    }
};

inline bool operator==(const HostInfoRequest& lhs, const HostInfoRequest& rhs) {
    return lhs.user_name == rhs.user_name && lhs.user_ip == rhs.user_ip &&
           lhs.process_name == rhs.process_name;
}

struct HostInfoReply {
    std::vector<HostInfoRequest> online_hosts;
    bool server_exclusive;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(online_hosts);
        archive(server_exclusive);
    }
};

void BuildDataRegTopicDescriptorMessage(const std::string& uuid,
                                        const std::string& topic,
                                        const std::string& msg_type,
                                        const std::string& msg_descriptor,
                                        std::vector<char>* out_buf);

bool SendDataMsgData(const std::string& uuid,
                     const uint64_t seq,
                     const uint64_t pub_time,
                     const char* msg,
                     const uint64_t size,
                     base::common::TcpFrameClient* client);

bool SendNDataMsgDataAtOnePacket(
    const std::string& uuid,
    const uint64_t seq,
    const uint64_t pub_time,
    const std::vector<std::pair<const char*, const uint64_t>>& msgs,
    base::common::TcpFrameClient* client);

void BuildStructMessageToBuf(const HostInfoReply& data, std::string* out_buf);
void BuildStructMessageToBuf(const HostInfoRequest& data, std::string* out_buf);

void BuildBufToMessageStruct(const char* data,
                             size_t size,
                             HostInfoReply* out_struct);
void BuildBufToMessageStruct(const char* data,
                             size_t size,
                             HostInfoRequest* out_struct);

}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
