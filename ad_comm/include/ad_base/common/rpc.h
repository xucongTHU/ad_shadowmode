
/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <future>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <utility>

#include "ad_base/common/socket.h"
#include "ad_base/common/thread_pool.h"

namespace senseAD {
namespace base {
namespace common {

// Data Stream format: |RequestId|Data|

enum RpcStatus : uint16_t {
    OK = 0,
    ERROR = 1,

    TIMEOUT = 98,
    CONNECTION_ERROR = 99,
    UNEXPECTED_ERROR = 100
};
struct RpcResponse {
    RpcStatus status;
    const char* data_ptr;
    size_t data_size;

    // Inner Storage
    std::vector<char> buf_storage;
};

using RpcRequestId = uint32_t;

class RpcClient {
 public:
    RpcClient(const std::string& ip,
              const uint16_t port,
              bool log_failure = true);
    ~RpcClient();

    bool IsReady() const;
    bool WaitForServiceReady(const std::chrono::milliseconds timeout_ms);
    /**
     * !IMPORTANT: We promise that the user frame_buffer start offset in packet
     * will be align to word size
     */
    std::future<RpcResponse> SendRequest(
        const char* frame_buffer,
        const size_t len,
        const std::chrono::milliseconds timeout =
            std::chrono::milliseconds(5000));

    std::future<RpcResponse> SendRequest(
        const std::vector<std::pair<const char*, size_t>>& frame_buffers,
        const std::chrono::milliseconds timeout =
            std::chrono::milliseconds(5000));

    inline static size_t MaxRequestBufferPerRequest() {
        return senseAD::base::common::TcpFrameClient::MaxSendBufferPerSend();
    }

 private:
    void ResponseMappingThread();

    std::unique_ptr<senseAD::base::common::TcpFrameClient> cli_;
    std::string ip_port_;

    std::mutex user_multi_thread_access_mtx_;

    std::mutex big_mtx_;
    std::unordered_map<RpcRequestId, std::promise<RpcResponse>>
        request_fut_maps_;
    std::unordered_map<RpcRequestId, std::chrono::steady_clock::time_point>
        request_timeout_maps_;
    RpcRequestId last_id_ = 0;

    bool running_ = true;
    std::thread response_waiting_thread_;
};

class RpcServer {
 public:
    struct Context {
        RpcServer* ser_ptr{nullptr};
        std::string sender_ip;
        int sender_port;
        ConnectionId conn_id;
    };

    /**
     * !IMPORTANT: We promise that the input data ptr will be align to word size
     */
    using HandleFn =
        std::function<RpcStatus(const char*, const size_t, std::vector<char>*)>;
    using HandleFnWithContext = std::function<RpcStatus(
        const Context&, const char*, const size_t, std::vector<char>*)>;
    using ConnectionChange = TcpFrameServer::ConnectionChange;
    using ConnectionChangeFn = TcpFrameServer::ConnectionChangeFn;

    RpcServer(const std::string& ip,
              const uint16_t port,
              const HandleFn& fn,
              TcpFrameServer::ConnectionChangeFn change_fn = nullptr);
    RpcServer(const std::string& ip,
              const uint16_t port,
              const HandleFnWithContext& fn,
              TcpFrameServer::ConnectionChangeFn change_fn = nullptr);
    ~RpcServer();

    void SetWhitelistIps(const std::unordered_set<std::string>& whitelist_ips);
    void SetOnClientChangeHandler(TcpFrameServer::ConnectionChangeFn fn);

 private:
    void Init(const std::string& ip,
              const uint16_t port,
              const HandleFnWithContext& fn,
              TcpFrameServer::ConnectionChangeFn change_fn);
    void MainLoop();
    void ClientChangeFn(const ConnectionId cfd,
                        const TcpFrameServer::ConnectionChange change_event);

    std::unique_ptr<senseAD::base::common::TcpFrameServer> ser_;
    std::string ip_port_;
    std::unique_ptr<senseAD::base::common::ThreadPool> execute_pool_;

    bool running_ = true;
    std::thread recv_thread_;
    HandleFnWithContext handle_fn_;

    std::mutex id_map_mtx_;
    ConnectionId non_reuse_id_{0};
    std::unordered_map<ConnectionId, ConnectionId> non_reuse_id_map_;
    TcpFrameServer::ConnectionChangeFn client_change_fn_;
    Context cur_context_;
};

}  // namespace common
}  // namespace base
}  // namespace senseAD
