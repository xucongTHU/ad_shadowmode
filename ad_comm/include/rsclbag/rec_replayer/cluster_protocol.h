/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstdint>

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>

#include "ad_base/std/optional.h"
#include "ad_base/common/rpc.h"
#include "rsclbag/player/data_source/frame_loader.h"

// Request -> Binary
//* | header size | header serialzation |
//* or | header size | data header serialzation | Data 1 | Data 2 |

// Resp -> Binary
//* | header serialzation |

enum class RequestType : uint8_t {
    // HeaderOnly
    INIT_STAGE = 0,
    TSYNC_INIT_STAGE = 1,
    TSYNC_OFFSET_UPDATE_STAGE = 2,
    START_STAGE = 3,
    RENEW_TICKET = 4,

    PAUSE_CMD = 5,
    RESUME_CMD = 6,
    PLAY_RATE_CMD = 7,

    // Header +
    DATA_PACKET = 8
};

std::string ToString(const RequestType& t);

struct ChannelToExchange {
    uint32_t channel_id;

    std::string channel_name;
    std::string msg_type;
    std::string msg_descriptor;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(channel_id, channel_name, msg_type, msg_descriptor);
    }
};

struct InitReqPacket {
    std::string master_uuid;

    // setting
    bool looping_with_continous_timestamp = false;
    uint32_t preload_buffer_sec = 2;
    bool need_tsync = true;

    uint32_t play_rate10;
    bool pause;

    // bag channels
    std::vector<ChannelToExchange> channels;
    int32_t bag_gmt_offset;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(master_uuid, looping_with_continous_timestamp,
                preload_buffer_sec, need_tsync, play_rate10, pause, channels,
                bag_gmt_offset);
    }
};

struct TsyncStagePacketReq {
    uint64_t master_local_sent_time;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(master_local_sent_time);
    }
};

struct TsyncStagePacketResp {
    uint64_t master_local_sent_time;
    uint64_t client_local_received_time;
    uint64_t client_local_sent_time;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(master_local_sent_time, client_local_received_time,
                client_local_sent_time);
    }
};

struct TsyncOffsetUpdateStagePacket {
    // master time + offset = this machine time
    int64_t master_to_here_time_offset;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(master_to_here_time_offset);
    }
};

struct StartPacket {
    uint64_t start_at_master_tp;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(start_at_master_tp);
    }
};

struct RenewTicketPacket {
    uint64_t allowed_seq;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(allowed_seq);
    }
};

struct ResumeCmdPacket {
    uint64_t resume_master_tp;
    uint64_t expect_msg_tp_when_resume;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(resume_master_tp, expect_msg_tp_when_resume);
    }
};

struct PlayRateCmdPacket {
    double rate;
    uint64_t apply_master_tp;
    uint64_t apply_msg_tp;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(rate, apply_master_tp, apply_msg_tp);
    }
};

struct DataPacket {
    std::vector<uint32_t> data_lens;

    std::vector<uint64_t> seqs;
    std::vector<uint32_t> channel_ids;
    std::vector<uint64_t> msg_timestamp_ns;
    std::vector<int> loop_indexs;

    inline static DataPacket From(
        const std::vector<std::shared_ptr<LoadedMessage>>& msgs) {
        DataPacket out;
        for (const auto& msg : msgs) {
            out.data_lens.emplace_back(msg->content->Msg().size());
            out.seqs.emplace_back(msg->seq);
            out.channel_ids.emplace_back(msg->msg_channel_id);
            out.msg_timestamp_ns.emplace_back(msg->msg_timestamp_ns);
            out.loop_indexs.emplace_back(msg->loop_index);
        }
        return out;
    }

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(data_lens, seqs, channel_ids, msg_timestamp_ns, loop_indexs);
    }
};

struct ClusterRequestHeader {
    RequestType req_type;

    senseAD::base::optional<InitReqPacket> init_req;
    senseAD::base::optional<TsyncStagePacketReq> tsync_req;
    senseAD::base::optional<TsyncOffsetUpdateStagePacket>
        tsync_offset_update_req;
    senseAD::base::optional<StartPacket> start_req;
    senseAD::base::optional<RenewTicketPacket> renew_ticket_req;
    senseAD::base::optional<ResumeCmdPacket> resume_cmd_req;
    senseAD::base::optional<PlayRateCmdPacket> play_rate_cmd_req;
    senseAD::base::optional<DataPacket> data_req;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(req_type);

#define AR_FN(enum_type, type, var) \
    case enum_type:                 \
        if (!var) var = {type()};   \
        archive(*var);              \
        break;

        switch (req_type) {
            AR_FN(RequestType::INIT_STAGE, InitReqPacket, init_req)
            AR_FN(RequestType::TSYNC_INIT_STAGE, TsyncStagePacketReq, tsync_req)
            AR_FN(RequestType::TSYNC_OFFSET_UPDATE_STAGE,
                  TsyncOffsetUpdateStagePacket, tsync_offset_update_req)
            AR_FN(RequestType::START_STAGE, StartPacket, start_req)
            AR_FN(RequestType::RENEW_TICKET, RenewTicketPacket,
                  renew_ticket_req)
            AR_FN(RequestType::RESUME_CMD, ResumeCmdPacket, resume_cmd_req)
            AR_FN(RequestType::PLAY_RATE_CMD, PlayRateCmdPacket,
                  play_rate_cmd_req)
            AR_FN(RequestType::DATA_PACKET, DataPacket, data_req)

            default:
                break;
        }
    }

#undef AR_FN
};

struct ClusterResponse {
    RequestType resp_type;

    senseAD::base::optional<TsyncStagePacketResp> tsync_resp;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(resp_type);

        switch (resp_type) {
            case RequestType::TSYNC_INIT_STAGE:
                if (!tsync_resp) tsync_resp = {TsyncStagePacketResp()};
                archive(*tsync_resp);
                break;

            default:
                break;
        }
    }
};

std::vector<char> SerializeClusterResponse(const ClusterResponse& resp);

bool SendClusterRequestAndParseResponse(senseAD::base::common::RpcClient* cli,
                                        const uint32_t timeout_ms,
                                        const ClusterRequestHeader& req,
                                        ClusterResponse* response,
                                        bool wait_for_resp = true);

bool SendClusterData(
    senseAD::base::common::RpcClient* cli,
    const uint32_t timeout_ms,
    const ClusterRequestHeader& req,
    const std::vector<std::pair<const char*, size_t>>& msg_datas);

senseAD::base::common::RpcStatus ParseClusterRequestAndResponse(
    const char* request_buf,
    const size_t request_size,
    const std::unordered_map<uint32_t, std::string>& channel_map,
    ClusterRequestHeader* received_hdr,
    std::vector<std::shared_ptr<LoadedMessage>>* received_msg_datas);
