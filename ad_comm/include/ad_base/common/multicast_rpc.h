/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "ad_base/common/udp.h"
#include "ad_log/ad_log.hpp"
namespace senseAD {
namespace base {
namespace common {

constexpr std::size_t kMultiCastMsgBufSizeMax = 4096;

struct MultiCastMsgBuf {
    uint32_t size;
    char buf[kMultiCastMsgBufSizeMax - 4];
};

class MultiCastRpcServer {
    using MultiCastRpcServerCallBack = Udp::UdpAsyncRecvCallBack;

 public:
    MultiCastRpcServer();
    ~MultiCastRpcServer();
    bool Init(const std::string &ip,
              uint16_t port,
              std::function<std::string(std::string &)> c);

 private:
    void StartReceive();
    void HandleReceive(char *buf, std::size_t size);

    Udp udp_;
    std::string multicast_ip_;
    uint16_t multicast_port_;
    MultiCastMsgBuf buf_recv_;
    std::function<std::string(std::string &)> cb_;

    std::atomic<bool> thread_stop_flag_;
    std::shared_ptr<std::thread> p_thread_;
};

class MultiCastRpcClient {
 public:
    MultiCastRpcClient();
    ~MultiCastRpcClient();
    bool Init(const std::string &ip, int16_t port);
    std::vector<std::string> &Request(const std::string &req);
    void StartReceive();

 private:
    void HandleReceive(char *buf, std::size_t size);

    Udp udp_;
    std::string multicast_ip_;
    uint16_t multicast_port_;
    MultiCastMsgBuf buf_recv_;
    bool recv_timeout_flag_;
    std::vector<std::string> rsp_vector_;
};

}  // namespace common
}  // namespace base
}  // namespace senseAD
