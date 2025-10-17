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
namespace senseAD {
namespace rscl {
namespace comm {
namespace raw {

class UDPSubscriber : public RawSubscriberBase {
 public:
    using Ptr = std::shared_ptr<UDPSubscriber>;
    explicit UDPSubscriber(const CustomConfig& config,
                           const CallbackFunc& callback);
    virtual ~UDPSubscriber();

    bool Receive(std::shared_ptr<RecvRawData>* data) override;

    void SetNotificationCallback(const NotifyCallback& callback) override;

 private:
    bool ValidateParams();

    bool SetupRecvThread();

    int sockfd_;

    bool is_shutdown_;
    bool callback_mode_ = false;
    CallbackFunc callback_;
    NotifyCallback notify_callback_;
    std::thread recv_thread_;

    std::mutex queue_mutex_;
    size_t queue_size_;
    std::queue<std::shared_ptr<RecvRawData>> data_queue_;
    std::shared_ptr<RecvRawData> receive_data_;

    static constexpr int kTimeout = 3000;
    static constexpr int kMaxCtrlSize = 8192;
};

}  // namespace raw
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
