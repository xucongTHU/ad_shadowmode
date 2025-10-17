/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cppzmq/zmq.hpp>
#include <thread>
#include <memory>
#include <string>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <vector>

#include "ad_serde/msgmeta.h"
#include "ad_mw/comm.h"

namespace senseAD {
namespace rscl {
namespace common_bridge {

class ZmqForwardPublisher {
 public:
    ZmqForwardPublisher(const std::string& topic,
                        const senseAD::serde::MsgMeta& meta);
    ~ZmqForwardPublisher();
    bool Publish(const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg);
    bool IsEstablished() const;

    std::string GetTopicName() const { return topic_; }

 private:
    bool Establish();
    std::atomic<bool> is_established_{false};
    std::string topic_;
    std::string publisher_id_;
    senseAD::serde::MsgMeta meta_;
};

}  // namespace common_bridge
}  // namespace rscl
}  // namespace senseAD
