/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstdint>

#include <vector>
#include <string>
#include <limits>
#include <set>
#include <atomic>
#include <unordered_map>

#include "ad_rscl/ad_rscl.h"
#include "rsclbag/player/data_source/loaded_message_buffer.h"

using senseAD::rscl::comm::RawMessage;

enum class LoopMode {
    NO_LOOP,
    RESTART_LOOP,    // timestamp will JUMP BACK when repeat
    CONTINOUS_NODE,  //  timestamp will continue increment when repeat
};
std::string LoopModeStr(const LoopMode &mode);

struct PlayerOptions {
    /**
     * @brief Should Play repeatedly?
     */
    LoopMode loop_mode = LoopMode::NO_LOOP;
    /**
     * @brief Should publish the sim time?
     */
    bool pub_sim_time = true;
    /**
     * @brief Play Rate
     */
    double play_rate = 1.0;

    /**
     * @brief Set play start time = bag_begin_time + relative_start_sec
     * mutually exclusive [begin_time_ns, begin_time_datetime_str,
     * relative_start_sec]
     */
    double relative_start_sec = 0;
    /**
     * @brief Set play start time , format "%Y-%m-%d %H:%M:%S"
     * mutually exclusive [begin_time_ns, begin_time_datetime_str,
     * relative_start_sec]
     */
    std::string begin_time_datetime_str;
    /**
     * @brief Set play start time = bag_begin_time
     * mutually exclusive [begin_time_ns, begin_time_datetime_str,
     * relative_start_sec]
     */
    uint64_t begin_time_ns = 0;
    /**
     * @brief Set play end time = end_time_ns
     * mutually exclusive [end_time_ns, end_time_datetime_str]
     */
    uint64_t end_time_ns = std::numeric_limits<uint64_t>::max();
    /**
     * @brief Set play end time , format "%Y-%m-%d %H:%M:%S"
     * mutually exclusive [end_time_ns, end_time_datetime_str]
     */
    std::string end_time_datetime_str;

    /**
     * @brief If true, not play the bag immediately.
     *
     */
    bool pause = false;

    std::set<std::string> excluded_channels;
    std::set<std::string> included_channels;
    std::unordered_map<std::string, std::string> remap_channels;

    std::string msg_transform_lib;

    FrameLoaderAttr ToFrameLoaderAttr() const;
};
