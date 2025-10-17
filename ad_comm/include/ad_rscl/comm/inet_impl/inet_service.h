/**
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @brief An inet implementation for service.
 * Use Ip and Port to setup channel instead of service name
 */

#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <vector>
#include <unordered_set>

#include "ad_log/ad_log.hpp"
#include "ad_base/common/rpc.h"
#include "ad_rscl/comm/service_base.h"
#include "ad_base/std/optional.h"

namespace senseAD {
namespace rscl {
namespace comm {

using senseAD::base::common::ConnectionId;
using senseAD::base::common::RpcServer;
using senseAD::base::common::RpcStatus;

template <typename Request, typename Response>
class InetService : public Service<Request, Response> {
 public:
    struct Context {
        InetService<Request, Response> *ser_ptr{nullptr};
        std::string sender_ip;
        int sender_port;
        ConnectionId conn_id;
    };

    using Ptr = std::shared_ptr<InetService<Request, Response>>;
    using ServiceCallback =
        std::function<void(const std::shared_ptr<ReceivedMsg<Request>> &,
                           std::shared_ptr<SendMsg<Response>> &)>;

    using ServiceCallbackV2 =
        std::function<void(const Context &,
                           const std::shared_ptr<ReceivedMsg<Request>> &,
                           std::shared_ptr<SendMsg<Response>> &)>;

    // TODO(someone) support use topic as key instead of ip + port
    //! REMINDER: Should not ONLY use multicast to for service discovery
    // need to consider the case that two device isn't located in
    // same subnet.
    InetService(const std::string &ip,
                const uint16_t port,
                const ServiceCallback &cb,
                const ServiceConf &conf);
    InetService(const std::string &ip,
                const uint16_t port,
                const ServiceCallbackV2 &cb,
                const ServiceConf &conf);

    virtual ~InetService();
    void ShutdownImpl() override;

    void ToggleSwitch(const bool enable) override;

    senseAD::base::optional<ConnectionId> CurrentConnectionId() const {
        return conn_id_;
    }

    bool IsConnectionAlive(const ConnectionId conn_id) const {
        std::lock_guard<std::mutex> lg(conn_id_mtx_);
        return alive_conn_.find(conn_id) != alive_conn_.end();
    }

 private:
    RpcStatus ServerFn(const char *in_data_ptr,
                       const size_t in_data_size,
                       std::vector<char> *out_data);

    const std::string ip_;
    const uint16_t port_;
    ServiceCallbackV2 user_cb_;
    std::shared_ptr<RpcServer> rpc_ser_;

    senseAD::base::optional<ConnectionId> conn_id_{nullptr};

    mutable std::mutex conn_id_mtx_;
    std::unordered_set<ConnectionId> alive_conn_;
    Context context_;
};

template <typename Request, typename Response>
InetService<Request, Response>::InetService(const std::string &ip,
                                            const uint16_t port,
                                            const ServiceCallback &cb,
                                            const ServiceConf &conf)
    : Service<Request, Response>(conf),
      ip_(ip),
      port_(port),
      user_cb_([cb](auto, auto arg2, auto arg3) { cb(arg2, arg3); }) {
    context_.ser_ptr = this;
    ToggleSwitch(true);
}

template <typename Request, typename Response>
InetService<Request, Response>::InetService(const std::string &ip,
                                            const uint16_t port,
                                            const ServiceCallbackV2 &cb,
                                            const ServiceConf &conf)
    : Service<Request, Response>(conf), ip_(ip), port_(port), user_cb_(cb) {
    context_.ser_ptr = this;
    ToggleSwitch(true);
}

template <typename Request, typename Response>
InetService<Request, Response>::~InetService() {
    ::Shutdownable::Shutdown();
}

template <typename Request, typename Response>
void InetService<Request, Response>::ShutdownImpl() {
    Service<Request, Response>::ShutdownImpl();
    SwitchableNodeMemberBase::Disable();
}

template <typename Request, typename Response>
void InetService<Request, Response>::ToggleSwitch(const bool enable) {
    if (enable) {
        if (!rpc_ser_) {
            rpc_ser_ = std::make_shared<RpcServer>(
                ip_, port_,
                [this](auto ctx, auto arg1, auto arg2, auto arg3) -> RpcStatus {
                    this->context_.sender_ip = ctx.sender_ip;
                    this->context_.sender_port = ctx.sender_port;
                    this->context_.conn_id = ctx.conn_id;
                    auto ret = this->ServerFn(arg1, arg2, arg3);
                    return ret;
                },
                [this](auto id, auto event) {
                    std::lock_guard<std::mutex> lg(this->conn_id_mtx_);
                    switch (event) {
                        case RpcServer::ConnectionChange::NEW:
                            this->alive_conn_.emplace(id);
                            break;

                        case RpcServer::ConnectionChange::CLOSED:
                            this->alive_conn_.erase(id);
                            break;
                        default:
                            return;
                    }
                });
        }
    } else {
        rpc_ser_ = nullptr;
    }
}

template <typename Request, typename Response>
RpcStatus InetService<Request, Response>::ServerFn(
    const char *in_data_ptr,
    const size_t in_data_size,
    std::vector<char> *out_data) {
    auto req_raw = std::make_shared<BorrowedRawMessage>(in_data_ptr,
                                                        in_data_size, nullptr);
    auto rreq = std::make_shared<ReceivedMsg<Request>>(req_raw);
    auto rresp = std::make_shared<SendMsg<Response>>();
    user_cb_(context_, rreq, rresp);

    if (rresp == nullptr) {
        return RpcStatus::ERROR;
    }
    auto resp = rresp->Finalize().BorrowRaw();
    *out_data = std::vector<char>(resp->data, resp->data + resp->size);

    return RpcStatus::OK;
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
