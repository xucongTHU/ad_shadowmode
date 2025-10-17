/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstdint>

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <limits>

#include "ad_base/common/rpc.h"
#include "ad_rscl/ad_rscl.h"

enum class SelfReplayRole {
    REPLAY_SERVER,
    REPLAY_CLIENT,
};

// the communication is always start by the server
struct RpcServerAddress {
    std::string ip;
    int port;
};

struct PlayFixedSetting {
    std::set<std::string> data_paths;

    bool looping_with_continous_timestamp = false;
    std::string msg_rewrite_lib = "";

    double relative_start_sec = 0.0;
    double duration_sec = std::numeric_limits<double>::max();

    uint64_t begin_time_ns = 0;
    uint64_t end_time_ns = std::numeric_limits<uint64_t>::max();

    std::set<std::string> excluded_channels;
    std::set<std::string> included_channels;

    uint32_t preload_buffer_sec = 2;
    bool need_tsync = true;

    // For debug only
    bool pub_sim_time = true;
    bool pub_msg = true;

    void Print() const;
};

struct PlayRuntimeSetting {
    // play rate * 10
    uint32_t play_rate10{10};
    bool pause{false};

    void Print() const;
};

struct RecReplayGlobalOption {
    SelfReplayRole role;
    int self_port;

    // The data source ip
    std::string server_ip;

    std::vector<RpcServerAddress> clients;

    PlayFixedSetting fixed_setting;
    PlayRuntimeSetting default_runtime_setting;

    void Print() const;
};

struct TuiDashboard {
    PlayRuntimeSetting setting;

    std::shared_ptr<PlayRuntimeSetting> runtime_setting;
};
//=======================================

class ControllablePlayer {
 public:
    virtual void ApplyDefaultState(double play_rate, bool pause) = 0;
    virtual bool StartPlayAt(const uint64_t start_play_tp) = 0;
    virtual void Pause() = 0;
    virtual bool ResumeAt(const uint64_t resume_tp,
                          const uint64_t expect_msg_tp_when_resume) = 0;
    virtual bool SetPlayRate(const double rate,
                             const uint64_t clock_tp,
                             const uint64_t msg_tp) = 0;
    virtual void RenewAllowToPlayTicket(const uint64_t allowed_seq) = 0;
};

//======================================
