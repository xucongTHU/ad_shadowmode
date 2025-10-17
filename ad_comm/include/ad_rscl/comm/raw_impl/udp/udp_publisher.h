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
#include <vector>

#include "ad_rscl/comm/raw_impl/raw_comm.h"
namespace senseAD {
namespace rscl {
namespace comm {
namespace raw {

class UDPPublisher : public RawPublisherBase {
 public:
    using Ptr = std::shared_ptr<UDPPublisher>;
    explicit UDPPublisher(const CustomConfig& config);
    ~UDPPublisher();

    bool Send(const SendRawData& data) override;

 private:
    bool CreateServerAddr(const std::string& ip,
                          uint16_t port,
                          sockaddr_in* server_addr);

    bool ValidateParams();

    struct iovec iov_[1];
    int sockfd_;
    struct sockaddr_in server_addr_;
    struct msghdr msg_;
};

}  // namespace raw
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
