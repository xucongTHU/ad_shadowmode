/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <condition_variable>
#include <mutex>
#include <memory>

#include "ad_rscl/ad_rscl.h"
#include "rsclbag/player/data_source/publisher_map.h"
#include "ad_msg_idl/std_msgs/header.capnp.h"
#include "rsclbag/rec_replayer/common.h"
#include "rsclbag/player/data_source/loaded_message_buffer.h"
#include "ad_rscl/comm/internal_channel.h"

using SimTimeChannel = senseAD::rscl::comm::SimTimeChannel;

class ReplayPlayer : public ControllablePlayer {
 public:
    ReplayPlayer(PlayRuntimeSetting* runtime_setting,
                 const PlayFixedSetting& fix_setting = PlayFixedSetting());
    virtual ~ReplayPlayer();

    bool Init(rscl::Node* node,
              const Channels& channels,
              LoadedMessageBuffer* msg_in);

    void ApplyDefaultState(double play_rate, bool pause) override;

    // Runtime Control
    bool StartPlayAt(const uint64_t start_play_ns) override;
    void Pause() override;
    bool ResumeAt(const uint64_t resume_tp,
                  const uint64_t expect_msg_tp_when_resume) override;
    bool SetPlayRate(const double rate,
                     const uint64_t clock_tp,
                     const uint64_t msg_tp) override;
    void RenewAllowToPlayTicket(const uint64_t allowed_seq) override;
    virtual void Publish(const LoadedMessage& msg);
    virtual void PublishSimTime(const uint64_t sim_time);

    inline bool PlayEnd() const { return play_end_; }
    inline uint64_t LatestPlayFrameTp() const { return latest_played_tp_; }
    inline uint64_t TotalPlayedCount() const { return total_played_count_; }
    inline uint64_t CurrentLoopIndex() const { return cur_loop_index_; }
    inline uint64_t MaxDelayPlayNs() const { return max_delay_play_ns_; }
    inline uint64_t DisorderCnt() const { return disorder_cnt_; }
    inline double GetPlayRate() const { return play_rate_; }
    inline uint64_t GetAllowedSeq() const { return allowed_seq_; }

 private:
    // Have to be run with <= 1 ms interval
    void PlayLoop();

 private:
    PlayRuntimeSetting* runtime_setting_;
    bool pub_sim_time_ = true;
    bool pub_msg_ = true;

    std::unique_ptr<PublisherMap> publishers_{nullptr};
    LoadedMessageBuffer* msg_in_;

    std::thread play_loop_thread_;
    std::atomic<bool> running_{false};

    // Pub Mock Time
    std::unique_ptr<SimTimeChannel> sim_time_channel_;

    std::mutex cv_mtx_;

    std::atomic<bool> started_{false};
    std::condition_variable started_cv_;
    std::chrono::steady_clock::time_point start_play_tp_;
    uint64_t start_play_ns_{0};
    uint64_t base_pub_timestamp_ns_{0};

    std::atomic<bool> paused_{false};
    std::atomic<bool> resume_ready_{false};
    std::chrono::steady_clock::time_point resume_tp_;
    uint64_t resume_tp_ns_{0};
    uint64_t expect_msg_tp_when_resume_{0};
    int64_t paused_time_ns_{0};

    bool play_end_{false};

    std::atomic<bool> need_resync_time_{false};

    double play_rate_ = 1.0;

    uint64_t allowed_seq_ = 0;
    uint64_t latest_played_tp_ = 0;
    uint64_t cur_loop_index_ = 0;
    uint64_t total_played_count_ = 0;
    uint64_t max_delay_play_ns_ = 0;
    uint64_t disorder_cnt_ = 0;
};
