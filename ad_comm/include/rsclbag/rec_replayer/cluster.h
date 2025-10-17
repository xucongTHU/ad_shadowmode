/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstdint>

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <limits>
#include <exception>
#include <unordered_map>
#include <deque>

#include "rsclbag/rec_replayer/common.h"
#include "rsclbag/rec_replayer/cluster_protocol.h"
#include "rsclbag/player/data_source/loaded_message_buffer.h"

class ReplayServer {
 public:
    ReplayServer(const RecReplayGlobalOption& option,
                 LoadedMessageBuffer* out_buffer);
    virtual ~ReplayServer();

    bool InitDataMetas();
    inline Channels GetChannels() const { return channels_; }

    // Stage 1. Load bag header and init with all client
    bool InitWithClients(uint32_t timeout_sec);

    // Stage 2. Sync time with all client
    bool TsyncWithClients(uint32_t timeout_sec);

    // Stage 3. Load data from buffer_raw_loaded_ and send to all client
    bool LoadAndSendData(uint64_t load_message_tp,
                         uint32_t max_frame_per_transfer,
                         uint32_t max_kb_per_transfer,
                         uint32_t timeout_sec,
                         uint64_t* loaded_cnt,
                         uint64_t* loaded_seq,
                         uint64_t* loaded_duration_ns = nullptr,
                         uint64_t* loaded_kb = nullptr);

    // Stage 4. Send Ready to Play ticket to all client
    bool SendReadyToPlay(const uint64_t allowed_seq,
                         const uint32_t timeout_sec);
    bool ReSendDataThatFailedSent(const uint32_t timeout_sec);

    bool StartPlay(const uint64_t start_at_master_tp);

    bool CmdPause();
    bool CmdResume(const uint64_t resume_master_tp,
                   const uint64_t expect_msg_tp_when_resume);
    bool CmdSetPlayRate(const double rate,
                        const uint64_t apply_master_tp,
                        const uint64_t apply_msg_tp);

    inline FrameLoader* GetFrameLoader() { return frame_loader_.get(); }
    inline bool IsNoMoreData() const {
        return bag_io_done_ && buffer_raw_loaded_.Empty();
    }
    inline uint64_t GetRawLoadedDurationNs() const {
        return buffer_raw_loaded_.InBufferTimeRange();
    }

 private:
    // server have the extra io thread
    void BagIoThread();
    bool SendDataInSegment(
        std::deque<std::shared_ptr<LoadedMessage>>* message_inrange,
        uint32_t timeout_sec);

 private:
    const RecReplayGlobalOption option_;

    std::string uuid_;
    bool running_ = true;
    bool bag_io_done_ = false;
    std::thread bag_io_thread_;
    struct ClientContext {
        RpcServerAddress addr;
        std::unique_ptr<senseAD::base::common::RpcClient> cli;
        // local machine time + offset = remote machine time
        // assume to be FIXED at the entire replay period
        uint64_t matched_tp{0};
        int64_t tp_offset{0};
    };
    std::vector<ClientContext> client_contexts_;

    std::unique_ptr<FrameLoader> frame_loader_;
    std::deque<std::shared_ptr<LoadedMessage>> message_inrange_;
    Channels channels_;
    // message first load to rawe buffer
    LoadedMessageBuffer buffer_raw_loaded_;
    // message will moved to syced buffer after sent to clients
    LoadedMessageBuffer* buffer_synced_;
};

class ReplayClient {
 public:
    enum class Status : uint8_t {
        NONE = 0,
        INIT = 1,
        TSYNC0 = 2,
        TSYNC1 = 3,
        PLAYING = 4,
    };

    ReplayClient(const RecReplayGlobalOption& option,
                 ControllablePlayer* player,
                 LoadedMessageBuffer* out_buffer);
    virtual ~ReplayClient();

    bool WaitInitWithServer(int32_t timeout_sec);

    bool WaitTsyncWithServer(int32_t timeout_sec);
    inline Channels GetChannels() const { return channels_; }
    inline int32_t GetBagGmtOffset() const { return bag_gmt_offset_; }
    inline int64_t GetTpOffset() const { return tp_offset_; }
    inline bool IsReplayEnd() const { return replay_end_; }

 private:
    senseAD::base::common::RpcStatus RpcHandle(
        const senseAD::base::common::RpcServer::Context& ctx,
        const char* in_data,
        const size_t in_size,
        std::vector<char>* out);
    void ConnectionChangeHandle(
        const senseAD::base::common::ConnectionId cid,
        const senseAD::base::common::RpcServer::ConnectionChange change);
    bool WaitStatus(const Status target_status, int32_t timeout_sec) const;

    RecReplayGlobalOption option_;
    ControllablePlayer* player_;
    LoadedMessageBuffer* out_buffer_;

    std::unique_ptr<senseAD::base::common::RpcServer> ser_;
    std::string master_uuid_;
    senseAD::base::common::ConnectionId master_cid_{-1};
    Status status_{Status::NONE};
    int64_t tp_offset_{0};
    int32_t bag_gmt_offset_{0};

    Channels channels_;
    std::unordered_map<uint32_t, std::string> channel_map_;

    bool replay_end_{false};
};

class MasterClientConnectionException : public std::exception {
 public:
    explicit MasterClientConnectionException(std::string const& message)
        : msg_(message) {}
    virtual char const* what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};
