/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 */
#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <string>
#include <functional>

namespace senseAD {
namespace base {
namespace common {
constexpr int kUdpEpollInEventMax = 32;
class Udp {
 public:
    using UdpAsyncRecvCallBack =
        std::function<void(char *, std::size_t, uint32_t, uint16_t)>;
    Udp() {}
    ~Udp();
    bool Init(UdpAsyncRecvCallBack cb);
    // set socket option
    bool SetNonblock(bool nonblock = true);
    inline bool IsNonblock() { return nonblock_flag_; }

    bool SetReuseAddr(bool reuse_addr = true);
    inline bool IsReuseAddr() { return reuseaddr_flag_; }

    bool SetBroadCast(bool brdcast = true);
    inline bool IsBroadCast() { return broadcast_flag_; }

    bool SetMultiCast(const std::string &ip,
                      uint16_t port,
                      int ttl = 1,
                      bool enable_loop = true);
    inline int GetMultiCastTTL() { return multicast_ttl_; }
    inline bool IsMultiCast() { return multicast_flag_; }
    inline bool IsMultiCastLoop() { return multicast_loop_flag_; }

    bool Bind(const std::string &ip, int16_t port);

    int Send(char *buf,
             std::size_t buf_size,
             const std::string &remote_ip,
             uint16_t remote_port);
    int Recv(char *buf,
             std::size_t size,
             uint32_t *remote_ip,
             uint16_t *remote_port);
    int Recv(char *buf, std::size_t size);
    int RecvWithTimeout(char *buf, std::size_t buf_size, int timeout_ms);

 private:
    inline void Close() {
        if (epoll_fd_ > 0) {
            close(epoll_fd_);
        }
        if (socket_ > 0) {
            close(socket_);
        }
    }
    int socket_{-1};
    std::string local_ip_;
    uint16_t local_port_;
    std::string multicast_ip_;
    uint16_t multicast_port_;
    bool nonblock_flag_{false};
    bool reuseaddr_flag_{false};
    bool broadcast_flag_{false};
    bool multicast_flag_{false};
    bool multicast_loop_flag_{false};
    int multicast_ttl_{1};

    int epoll_fd_{-1};
    struct epoll_event event, events[kUdpEpollInEventMax];
    UdpAsyncRecvCallBack async_recv_cb_;
};
}  // namespace common
}  // namespace base
}  // namespace senseAD
