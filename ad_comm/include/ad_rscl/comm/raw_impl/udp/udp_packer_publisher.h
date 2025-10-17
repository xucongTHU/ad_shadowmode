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

#include "ad_rscl/comm/raw_impl/udp/udp_common.h"
#include "ad_rscl/comm/raw_impl/udp/udp_publisher.h"
namespace senseAD {
namespace rscl {
namespace comm {
namespace raw {

class UDPPackerPublisher : public RawPublisherBase {
 public:
    using Ptr = std::shared_ptr<UDPPackerPublisher>;
    explicit UDPPackerPublisher(const CustomConfig& config);
    ~UDPPackerPublisher();

    bool Send(const SendRawData& data) override;

 private:
    UDPPublisher::Ptr udp_publisher_;
    char buffer_[kMaxUDPMsgSize];
    int buffer_offset_ = 0;
    int pkg_cnt_ = 0;
};

}  // namespace raw
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
