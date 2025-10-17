/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <memory>
#include <queue>

#include "ad_rscl/comm/raw_impl/raw_comm.h"
#include "ad_rscl/comm/raw_impl/udp/udp_common.h"
#include "ad_rscl/comm/raw_impl/udp/udp_subscriber.h"
namespace senseAD {
namespace rscl {
namespace comm {
namespace raw {

class UDPPackerSubscriber : public RawSubscriberBase {
 public:
    using Ptr = std::shared_ptr<UDPPackerSubscriber>;
    explicit UDPPackerSubscriber(const CustomConfig& config,
                                 const CallbackFunc& callback);
    ~UDPPackerSubscriber();

    bool Receive(std::shared_ptr<RecvRawData>* data) override;

    void SetNotificationCallback(const NotifyCallback& callback) override;

 private:
    void SplitAndCallback(const RecvRawData& data) noexcept;

 private:
    CallbackFunc user_callback_;
    UDPSubscriber::Ptr udp_subscriber_;
};

}  // namespace raw
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
