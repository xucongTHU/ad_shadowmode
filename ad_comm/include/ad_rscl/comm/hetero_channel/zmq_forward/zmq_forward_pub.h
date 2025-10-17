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

#include "ad_base/common/macros.h"
#include "ad_rscl/comm/hetero_channel/basic_type.h"
#include "ad_rscl/idl/hetero_channel_config.pb.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

class ZmqForwardSessionManager;

using senseAD::rscl::idl::ZmqForwardChannelConfig;
class ZmqForwardPublisher : public PublisherBase {
 public:
    ZmqForwardPublisher(const std::string& topic,
                        const ZmqForwardChannelConfig& config);
    ~ZmqForwardPublisher() override;
    bool Publish(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg) override;

    std::string Type() const override;
    bool Establish() override;
    bool Close() override;
    bool IsEstablished() const override;
    void ShutdownImpl() override;

 private:
    std::atomic<bool> is_established_{false};

    std::string topic_;
    ZmqForwardChannelConfig config_;
    std::string publisher_id_;
    std::vector<char> reg_topic_msg_buffer_;

    std::shared_ptr<ZmqForwardSessionManager> mgr_;
};
}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
