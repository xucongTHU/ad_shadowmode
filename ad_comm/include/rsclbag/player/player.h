/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <vector>
#include <string>
#include <atomic>
#include <memory>
#include <unordered_map>

#include "rsclbag/player/common.h"
#include "rsclbag/player/message_loader.h"
#include "rsclbag/player/message_publisher.h"

class Player {
 public:
    Player(const std::vector<std::string> &bag_paths,
           const PlayerOptions &options = PlayerOptions());
    ~Player();

    bool Start();
    bool Stop();

    void WaitUntilPlayEnd() const;

 private:
    void ThreadTui();

    std::vector<std::string> bag_paths_;
    PlayerOptions options_;

    std::shared_ptr<rscl::Node> node_;

    std::unique_ptr<MessageLoader> message_loader_;
    std::unique_ptr<MessagePublisher> message_publisher_;

    bool stopped_ = true;
    bool paused_ = false;
    int32_t bag_gmt_offset_{0};

    std::thread tui_thread_;
};
