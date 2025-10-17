/**
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @brief An inet implementation for client.
 * Use Ip and Port to setup channel instead of service name
 */

#pragma once

#include <future>
#include <memory>
#include <string>
#include <vector>

#include "ad_base/common/rpc.h"
#include "ad_rscl/comm/client_base.h"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::base::common::RpcClient;
using senseAD::base::common::RpcStatus;

template <typename Request, typename Response>
class InetClient : public Client<Request, Response> {
 public:
    using Ptr = std::shared_ptr<InetClient<Request, Response>>;

    InetClient(const std::string& ip,
               const uint16_t port,
               const ClientConf& conf);
    ~InetClient() = default;

    void ToggleSwitch(const bool enable) override;

    std::shared_ptr<ReceivedMsg<Response>> SendRequest(
        std::shared_ptr<SendMsg<Request>> request,
        const std::chrono::milliseconds timeout_ms =
            std::chrono::milliseconds(-1)) override;

    std::shared_future<std::shared_ptr<ReceivedMsg<Response>>> AsyncSendRequest(
        std::shared_ptr<SendMsg<Request>> request) override;

    bool IsServiceReady() const override;

    bool WaitForServiceReady(const std::chrono::milliseconds timeout_ms =
                                 std::chrono::milliseconds(-1)) const override;

    inline std::string Ip() const { return ip_; }

 private:
    const std::string ip_;
    const uint16_t port_;
    std::shared_ptr<RpcClient> cli_;
};

template <typename Request, typename Response>
InetClient<Request, Response>::InetClient(const std::string& ip,
                                          const uint16_t port,
                                          const ClientConf& conf)
    : Client<Request, Response>(conf), ip_(ip), port_(port) {
    ToggleSwitch(true);
}

template <typename Request, typename Response>
void InetClient<Request, Response>::ToggleSwitch(const bool enable) {
    if (enable) {
        if (!cli_) {
            cli_ = std::make_shared<RpcClient>(ip_, port_, false);
        }
    } else {
        cli_ = nullptr;
    }
}

template <typename Request, typename Response>
std::shared_ptr<ReceivedMsg<Response>>
InetClient<Request, Response>::SendRequest(
    std::shared_ptr<SendMsg<Request>> request,
    const std::chrono::milliseconds timeout_ms) {
    auto request_raw = request->Finalize().BorrowRaw();

    auto resp =
        cli_->SendRequest(request_raw->data, request_raw->size, timeout_ms);
    if (timeout_ms > std::chrono::milliseconds(0)) {
        if (resp.wait_for(timeout_ms) != std::future_status::ready) {
            AD_LERROR(InetRpc_Cli) << "Request fail due to timeout";
            return nullptr;
        }
    }

    auto v = resp.get();
    if (v.status != RpcStatus::OK) {
        AD_LDEBUG(InetRpc_Cli)
            << "Request fail error code: " << static_cast<int>(v.status);
        return nullptr;
    }

    auto storage_ptr = std::make_shared<std::vector<char>>();
    storage_ptr->swap(v.buf_storage);
    auto req_raw = std::make_shared<BorrowedRawMessage>(v.data_ptr, v.data_size,
                                                        storage_ptr);

    return std::make_shared<ReceivedMsg<Response>>(req_raw);
}

template <typename Request, typename Response>
std::shared_future<std::shared_ptr<ReceivedMsg<Response>>>
InetClient<Request, Response>::AsyncSendRequest(
    std::shared_ptr<SendMsg<Request>> request) {
    if (cli_ == nullptr) {
        std::promise<std::shared_ptr<ReceivedMsg<Response>>> p;
        p.set_value(nullptr);
        return std::shared_future<std::shared_ptr<ReceivedMsg<Response>>>(
            p.get_future());
    }

    auto request_raw = request->Finalize().BorrowRaw();
    auto resp = cli_->SendRequest(request_raw->data, request_raw->size);

    return std::async(
        std::launch::deferred,
        [resp = std::move(
             resp)]() mutable -> std::shared_ptr<ReceivedMsg<Response>> {
            auto v = resp.get();
            if (v.status != RpcStatus::OK) {
                AD_LERROR(InetRpc_Cli) << "Request fail error code: "
                                       << static_cast<int>(v.status);
                return nullptr;
            }
            auto storage_ptr = std::make_shared<std::vector<char>>();
            storage_ptr->swap(v.buf_storage);
            auto req_raw = std::make_shared<BorrowedRawMessage>(
                v.data_ptr, v.data_size, storage_ptr);

            return std::make_shared<ReceivedMsg<Response>>(req_raw);
        });
}

template <typename Request, typename Response>
bool InetClient<Request, Response>::IsServiceReady() const {
    return cli_ && cli_->IsReady();
}

template <typename Request, typename Response>
bool InetClient<Request, Response>::WaitForServiceReady(
    const std::chrono::milliseconds timeout_ms) const {
    return cli_ && cli_->WaitForServiceReady(timeout_ms);
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
