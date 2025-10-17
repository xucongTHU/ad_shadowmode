/**
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <vector>
#include <string>

#include "ad_base/std/optional.h"
#include "ad_serde/header.h"
#include "ad_serde/ad_serde.h"

using MsgMeta = senseAD::serde::MsgMeta;

class MsgTransformLogic {
 public:
    virtual ~MsgTransformLogic() = default;
    virtual std::string Name() const = 0;
    virtual bool Process(
        const std::string& msg_channel,
        const uint64_t& inbag_timestamp_ns,
        const uint64_t& expected_play_timestamp_ns,
        const senseAD::base::optional<RsclMsgHeader>* rscl_header,
        const char* msg_buffer,
        const size_t msg_size,
        std::vector<char>* processed_output) = 0;
    /**
     * @brief Process message data during recording
     * @param msg_channel Topic/channel name
     * @param arrived_timestamp_ns Message arrival timestamp (nanoseconds)
     * @param rscl_header Original RSCL message header
     * @param msg_buffer Raw message buffer
     * @param msg_size Size of the message buffer
     * @param out_timestamp [out] Modified timestamp after processing
     * @param out_msg_buffer_with_header [out] Output buffer containing
     * processed message with header（optional）
     * @param msg_changed [out] Flag indicating if message was modified
     * @return true if processing successful, false otherwise
     *
     * Process incoming message data during recording. Subclasses can implement
     * custom processing logic, like filtering, transforming or validating
     * messages before recording.
     */
    virtual bool ProcessData(
        const std::string& msg_channel,
        const uint64_t& arrived_timestamp_ns,
        const senseAD::base::optional<RsclMsgHeader>* rscl_header,
        const char* msg_buffer,
        const size_t msg_size,
        uint64_t* out_timestamp,
        std::vector<char>* out_msg_buffer_with_header,
        bool* msg_changed) = 0;

    /**
     * @brief Process message metadata
     * @param msg_channel Topic/channel name
     * @param ori_msgmeta Original message metadata
     * @param out_msgmeta [out] Modified message metadata after processing
     * @return true if processing successful, false otherwise
     *
     * Process message metadata information. Subclasses can implement custom
     * processing logic, like modifying message type, descriptor or other
     * metadata properties.
     */
    virtual bool ProcessMsgMeta(const std::string& msg_channel,
                                const MsgMeta& ori_msgmeta,
                                MsgMeta* out_msgmeta) = 0;
};
