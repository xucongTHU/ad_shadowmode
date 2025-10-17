/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <utility>

namespace senseAD {
namespace rscl {
namespace common_bridge {

constexpr uint32_t kZmqPort = 5800;
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
    char uuid[16];  // uuid for one topic
    uint64_t seq;   // seq for the entire data channel (all topic use one data
                    // channel)
    uint64_t pub_time;
    // Need ensure the byte size of DataHeader is a multiple of 8
    char reserved[7];
};

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

void BuildDataRegTopicDescriptorMessage(const std::string& uuid,
                                        const std::string& topic,
                                        const std::string& msg_type,
                                        const std::string& msg_descriptor,
                                        std::vector<char>* out_buf);
void BuildDataMsgData(const std::string& uuid,
                      const uint64_t seq,
                      const uint64_t pub_time,
                      const char* msg,
                      const uint64_t size,
                      std::vector<char>* out_buf);

}  // namespace common_bridge
}  // namespace rscl
}  // namespace senseAD
