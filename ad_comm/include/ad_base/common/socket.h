
/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <string>
#include <functional>
#include <memory>
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <utility>
#include <deque>
#include <atomic>

#include "ad_base/common/thread_pool.h"

namespace senseAD {
namespace base {
namespace common {

typedef int64_t ConnectionId;

struct FrameMsg {
    std::string sender_ip;
    int sender_port;

    std::vector<char> msg;
    ConnectionId connection_id = 0;
};

/**
 * Format for TcpFrame
 *
 * | kTcpFramePrefix | size_t msg_len | char msg[msg_len] |
 *
 */

class TcpFrameClient {
 public:
    TcpFrameClient(const std::string& ip, const uint16_t port);
    ~TcpFrameClient();

    bool TryConnect(bool auto_disconnect = true);
    void Disconnect();
    void SetMsgQueueSize(const uint32_t msg_queue_size);

    //! IMPORTANT: The frame_buffer cannot be free before call Send
    bool SendMore(const char* frame_buffer, const size_t len);
    bool Send(const char* frame_buffer,
              const size_t len,
              bool non_block = false);
    bool Send(const std::vector<std::pair<const char*, size_t>>& frame_buffers);

    bool Recv(FrameMsg* msg,
              const std::chrono::milliseconds timeout_ms =
                  std::chrono::milliseconds(1000));

    inline void EnableSlientConnect() { slient_connect_ = true; }

    bool IsValid() const;
    inline bool IsRejected() const { return is_rejected_; }

    std::string ServerAddress() const;

    static size_t MaxSendBufferPerSend();

 private:
    void AutoDisconnect();
    void BackgroundThread();

    std::string ip_;
    uint32_t port_ = 0;
    int sockfd_ = 0;
    struct timeval tv_;

    bool valid_ = false;

    bool auto_disconnect_ = true;
    bool running_ = false;
    bool should_attempt_reconnect_ = false;
    bool is_rejected_ = false;

    std::thread bg_thread_;
    std::chrono::steady_clock::time_point expected_ack_tp_;
    std::chrono::steady_clock::time_point last_reconnect_tp_;

    size_t buf_cnt_ = 0;
    constexpr static size_t kMaxBufCnt = IOV_MAX / 2 - 2;
    iovec iov_bufs_[kMaxBufCnt * 2 + 2];

    uint32_t msg_queue_size_ = 1 << 6;
    std::mutex bg_msg_queue_mtx_;
    std::deque<FrameMsg> bg_msg_queue_;
    std::condition_variable bg_msg_queue_cv_;

    int thread_idx_ = 0;

    bool slient_connect_{false};
};

class TcpFrameServer {
 public:
    TcpFrameServer(const std::string& ip, const uint16_t port);
    ~TcpFrameServer();

    void SetOnlyAcceptIp(const std::string& only_accept_ip);
    void SetWhitelistIps(const std::unordered_set<std::string>& whitelist_ips);

    void SetMsgQueueSize(const uint32_t msg_queue_size);

    enum class ConnectionChange {
        NEW,
        CLOSED,
        REJECTED,
    };
    // Don't Put Blocking logic here
    using ConnectionChangeFn =
        std::function<void(const ConnectionId, const ConnectionChange)>;
    void SetOnConnectionChangeHandler(ConnectionChangeFn fn);

    bool TryBind();
    void Disconnect();

    bool Recv(FrameMsg* frame,
              const std::chrono::milliseconds timeout_ms =
                  std::chrono::milliseconds(1000));
    bool Send(const ConnectionId id,
              const char* frame_buffer,
              const size_t len);
    bool Send(const ConnectionId id,
              const std::vector<std::pair<const char*, size_t>>& frame_buffers);
    bool Boardcast(
        const std::vector<std::pair<const char*, size_t>>& frame_buffers);
    bool Boardcast(const char* frame_buffer, const size_t len);

    bool IsValid() const;

    inline const std::unordered_map<int, sockaddr_storage>& GetAllConnInfo()
        const {
        return cli_addrs_;
    }

 private:
    void BackgroundThread();
    void StopBackgroundThread();
    void AckThread();
    void ProcessData(int cfd, std::vector<char>* data_buffer);
    void DataProcessingThread();
    void ReadAllDataFromFd(const int cfd, char* buf, int recved_buflen);

    bool running_ = false;
    std::thread bg_thread_;
    std::thread ack_thread_;

    std::string ip_;
    uint32_t port_ = 0;
    int sockfd_ = 0;
    int epfd_ = 0;
    int intrfb_ = 0;

    std::unordered_set<std::string> whitelist_ips_;
    std::function<void(const ConnectionId, const ConnectionChange)>
        connection_change_fn_;
    std::shared_ptr<ThreadPool> connection_change_thread_pool_;
    std::string only_accept_ip_;
    bool valid_ = false;

    std::mutex bg_msg_queue_mtx_;
    uint32_t msg_queue_size_ = 1 << 12;
    std::deque<FrameMsg> bg_msg_queue_;
    std::unordered_set<int> ack_fd_set_;

    std::mutex gfd_mtx_;
    std::unordered_set<int> gfd_set_;

    std::deque<FrameMsg> bg_msg_user_queue_;
    std::condition_variable bg_msg_queue_cv_;

    std::thread data_process_thread_;
    std::condition_variable data_queue_cv_;
    std::mutex data_buffers_mtx_;
    std::unordered_map<int, std::queue<std::vector<char>>> data_buffers_;
    std::queue<int> data_notify_queue_;

    std::mutex cli_addrs_mtx_;
    std::unordered_map<int, sockaddr_storage> cli_addrs_;
    std::unordered_map<int, std::pair<std::string, int>> cli_addrs_ipport_;

    constexpr static uint32_t kReadBufSize = 1024 * 1024;
    std::mutex data_buffers_pool_mtx_;
    std::queue<std::vector<char>> data_buffers_pool_;
};

// Utils for inner Impl
constexpr char kTcpFramePrefix[] = "#TF#";
constexpr char kTcpKeepAlivePrefix[] = "#TF##KA#";
constexpr char kTcpRejectedPrefix[] = "#TF##RJ#";
constexpr size_t kTcpFramePrefixSizeInBuf = sizeof(kTcpFramePrefix) - 1;
/**
 * @brief The header size for the header that pack the user frame
 */
constexpr size_t kTcpFrameHeaderSize =
    kTcpFramePrefixSizeInBuf + sizeof(size_t);
constexpr size_t kTcpKeepAlivePrefixSizeInBuf = sizeof(kTcpKeepAlivePrefix) - 1;
constexpr size_t kTcpRejectedPrefixSizeInBuf = sizeof(kTcpRejectedPrefix) - 1;
constexpr uint32_t kAckIntervalMs = 1000;
constexpr uint32_t kMinAutoReconnectIntervalMs = 500;
constexpr uint32_t kDisconnectIntervalMs = kAckIntervalMs + 500;
constexpr uint32_t kRecvBlockMs = 500;

std::string IpAddrStr(const sockaddr_in& addr);
std::pair<std::string, int> IpAddrIpPort(const sockaddr_storage& addr);

bool EnableTcpKeepalive(int sock);
bool ReadTcpFrameFromHead(std::vector<char>* buf,
                          std::vector<char>* frame_data,
                          size_t maxReqBufferSize = 0);

}  // namespace common
}  // namespace base
}  // namespace senseAD
