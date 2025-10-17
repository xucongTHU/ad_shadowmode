/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once

#include <memory>
#include <thread>

#include "ad_base/common/shutdownable.h"
#include "ad_rscl/ad_rscl.h"
#include "ad_msg_idl/std_msgs/header.capnp.h"

namespace senseAD {
namespace rscl {
namespace comm {

class SimTimeChannel : public ::Shutdownable {
 public:
    explicit SimTimeChannel(rscl::Node *node);
    ~SimTimeChannel() override;
    void ShutdownImpl() override;
    bool CreatePublisher(const bool disable_cross_system = false);
    bool Publish(uint64_t sim_time);

 private:
    rscl::Node *node_ = nullptr;
    std::atomic<bool> is_channel_exist_{false};
    std::atomic<bool> is_running_{true};
    std::unique_ptr<std::thread> handle_ = nullptr;
    senseAD::service_discovery::ServiceDiscovery *sd_ = nullptr;
    rscl::Publisher<senseAD::msg::std_msgs::Time>::Ptr mock_time_pub_ = nullptr;
};
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
