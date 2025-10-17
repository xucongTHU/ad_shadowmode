/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <memory>
#include <atomic>
#include <vector>

#include "ad_msg_idl/std_msgs/header.capnp.h"

#include "ad_rscl/ad_rscl.h"
#include "rsclbag/player/common.h"
#include "rsclbag/player/data_source/loaded_message_buffer.h"
#include "rsclbag/player/data_source/publisher_map.h"
#include "ad_rscl/comm/internal_channel.h"

class MessagePublisher {
 public:
    MessagePublisher(rscl::Node *node,
                     const std::vector<Channel> &channels,
                     const PlayerOptions &options);
    ~MessagePublisher();
    bool BindToInputBuffer(std::weak_ptr<LoadedMessageBuffer> message_queue);

    bool Start();
    void Stop();

    void Pause();
    void Continue();
    void SetPlayRate(double play_rate);

    uint64_t GetCurPubTimestamp() const;
    int CurrentLoopIndex() const;
    bool IsRunning() const;

 private:
    void PublishLoop();
    void UpdateStateForNewLoop(int loop_index);

    rscl::Node *node_;
    std::vector<Channel> channels_;
    PlayerOptions options_;
    bool stopped_ = true;
    std::atomic<bool> paused_{false};

    // Pub Mock Time
    std::unique_ptr<senseAD::rscl::comm::SimTimeChannel> sim_time_channel_;

    // Pub Message In Bag
    std::unique_ptr<PublisherMap> publishers_map_;

    // Message from message loader
    int cur_loop_index_ = -1;
    uint64_t cur_pub_timestamp_ = 0;
    uint64_t base_pub_timestamp_ = 0;
    uint64_t base_sys_timestamp_ = 0;
    uint64_t paused_time_ns_ = 0;

    // 5 ms
    const uint64_t kPauseSleepUnitNs = 5 * 1e6;

    std::weak_ptr<LoadedMessageBuffer> message_queue_;
    std::thread publish_thread_;
};
