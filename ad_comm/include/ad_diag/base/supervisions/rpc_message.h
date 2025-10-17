/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cstdint>

#include <type_traits>
#include <vector>
#include <memory>

#include "ad_diag/base/supervisions/common.h"

namespace senseAD {
namespace diag {

#define COPYABLE_CHECK(T)                               \
    static_assert(std::is_trivially_copyable<T>::value, \
                  "T must be a POD type.");

struct ReportMessage {
    uint64_t unique_id;
    SupervisionReport base_report;
    std::vector<char> full_report;

    DEFINE_SAVE_LOAD_FN(unique_id, base_report, full_report)
};

enum class MessageType : uint8_t {
    UNNKNOWN = 0,
    // Client to Server
    REGISTRATION = 1,
    // Bidirection
    PINGPONG = 2,
    // Client to Server
    REPORT = 3,
    // Server to Client
    CONTROL = 4
};

struct RegisterMessage {
    uint64_t unique_id;
    SupervisionEntity entity;

    DEFINE_SAVE_LOAD_FN(unique_id, entity)
};

struct PingPongMessage {
    uint64_t unique_id;
    bool enabled = false;

    DEFINE_SAVE_LOAD_FN(unique_id, enabled)
};

struct ControlMessage {
    SupervisionGroup target_group = SupervisionGroup::NONE;
    ImportantLevel target_level = ImportantLevel::NORMAL;

    DEFINE_SAVE_LOAD_FN(target_group, target_level)
};

struct RpcMessageContent {
    // Only one of the element will be not null
    std::unique_ptr<PingPongMessage> pingpong;
    std::unique_ptr<RegisterMessage> reg;
    std::unique_ptr<ReportMessage> report;
    std::unique_ptr<ControlMessage> control;

    DEFINE_SAVE_LOAD_FN(pingpong, reg, report, control)
};

// This struct not used in send side
struct RpcMessage {
    MessageType type = MessageType::UNNKNOWN;
    RpcMessageContent content;

    DEFINE_SAVE_LOAD_FN(type, content)
};

struct RpcMessages {
    uint64_t seq = 0;
    uint64_t timestamp = 0;
    std::vector<RpcMessage> messages;

    void AddPingPongMessage(const PingPongMessage& msg);
    void AddRegisterMessage(const RegisterMessage& msg);
    void AddReportMessage(const ReportMessage& msg);
    void AddControlMessage(const ControlMessage& msg);

    void Clear();

    DEFINE_SAVE_LOAD_FN(seq, timestamp, messages)
};

bool SerializeRpcMessages(const RpcMessages& msg, std::vector<char>* buffer);
bool ParseRpcMessages(const std::vector<char>& buffer, RpcMessages* msg);

#undef COPYABLE_CHECK
}  // namespace diag
}  // namespace senseAD
