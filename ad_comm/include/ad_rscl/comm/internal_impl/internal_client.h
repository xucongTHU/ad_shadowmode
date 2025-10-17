/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @brief An internal implementation for client.
 * It will be used for the case the when sender/received is both internal
 */

#pragma once

#include <future>
#include <memory>
#include <string>

#include "ad_rscl/comm/client_base.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename Request, typename Response>
class InternalClient : public Client<Request, Response> {
 public:
    using Ptr = std::shared_ptr<InternalClient<Request, Response>>;

    InternalClient(NodeOwnerBase* const node_owner, const ClientConf& conf);

    ~InternalClient() override;

    void ShutdownImpl() override;

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

 private:
    NodeOwnerBase* const node_owner_;
    std::shared_ptr<MwClientHandle> client_handle_;
};

template <typename Request, typename Response>
InternalClient<Request, Response>::InternalClient(
    NodeOwnerBase* const node_owner, const ClientConf& conf)
    : Client<Request, Response>(conf),
      node_owner_(node_owner) {
    ::Shutdownable::UpdateTTLogDetail("InternalClient|" + conf.service_name());
    ToggleSwitch(true);
}

template <typename Request, typename Response>
InternalClient<Request, Response>::~InternalClient() {
    ::Shutdownable::Shutdown();
}

template <typename Request, typename Response>
void InternalClient<Request, Response>::ShutdownImpl() {
    ToggleSwitch(false);
    if (client_handle_ != nullptr) {
        client_handle_->Shutdown();
    }
    Client<Request, Response>::Shutdown();
}

template <typename Request, typename Response>
void InternalClient<Request, Response>::ToggleSwitch(const bool enable) {
    if (enable) {
        auto conf = static_cast<ClientBase*>(this)->GetClientConf();
        client_handle_ = senseAD::mw::comm::Client::New(
            node_owner_->InnerHandle(), ToMwClientConf(conf));
        if (client_handle_) {
            this->sd_client_sptr_->AddClient(node_owner_->GetName(),
                                             conf.service_name());
        }
    } else {
        if (client_handle_) {
            auto conf = static_cast<ClientBase*>(this)->GetClientConf();
            this->sd_client_sptr_->LeaveClient(node_owner_->GetName(),
                                               conf.service_name());
        }
        client_handle_ = nullptr;
    }
}

template <typename Request, typename Response>
std::shared_ptr<ReceivedMsg<Response>>
InternalClient<Request, Response>::SendRequest(
    std::shared_ptr<SendMsg<Request>> request,
    const std::chrono::milliseconds timeout_ms) {
    if (!static_cast<ClientBase*>(this)->IsEnabled()) {
        return nullptr;
    }
    auto rrequest = request->Finalize().BorrowRawWithRef(request);
    auto rresponse = client_handle_->SendRequest(rrequest, timeout_ms);
    return rresponse ? std::make_shared<ReceivedMsg<Response>>(rresponse)
                     : nullptr;
}

template <typename Request, typename Response>
std::shared_future<std::shared_ptr<ReceivedMsg<Response>>>
InternalClient<Request, Response>::AsyncSendRequest(
    std::shared_ptr<SendMsg<Request>> request) {
    // Copy Shared Ptr
    auto handle = client_handle_;
    if (handle == nullptr) {
        return std::async(
            []() -> std::shared_ptr<ReceivedMsg<Response>> { return nullptr; });
    }
    auto rrequest = request->Finalize().BorrowRawWithRef(request);
    auto rresponse_feature = client_handle_->AsyncSendRequest(rrequest);

    return std::async(
        std::launch::async,
        [rresponse_feature]() -> std::shared_ptr<ReceivedMsg<Response>> {
            auto v = rresponse_feature.get();
            return v ? std::make_shared<ReceivedMsg<Response>>(v) : nullptr;
        });
}

template <typename Request, typename Response>
bool InternalClient<Request, Response>::IsServiceReady() const {
    auto handle = client_handle_;
    if (handle == nullptr) {
        return false;
    }
    return client_handle_->IsServiceReady();
}

template <typename Request, typename Response>
bool InternalClient<Request, Response>::WaitForServiceReady(
    const std::chrono::milliseconds timeout_ms) const {
    // TODO(kitfung) Change to adapt to topology event
    std::chrono::milliseconds quote = timeout_ms;
    auto step_ms = std::chrono::milliseconds(200);
    while (!IsServiceReady()) {
        std::this_thread::sleep_for(step_ms);
        if (quote.count() >= 0) {
            quote -= step_ms;
            if (quote.count() <= 0) {
                break;
            }
        }
    }
    return IsServiceReady();
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
