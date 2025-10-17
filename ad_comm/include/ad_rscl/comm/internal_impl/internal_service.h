/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @brief An internal implementation for service.
 * It will be used for the case the when sender/received is both internal
 */

#pragma once

#include <memory>
#include <string>
#include <stdexcept>

#include "ad_log/ad_log.hpp"
#include "ad_rscl/comm/service_base.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename Request, typename Response>
class InternalService : public Service<Request, Response> {
 public:
    using Ptr = std::shared_ptr<InternalService<Request, Response>>;
    using ServiceCallback =
        std::function<void(const std::shared_ptr<ReceivedMsg<Request>> &,
                           std::shared_ptr<SendMsg<Response>> &)>;

    InternalService(NodeOwnerBase *const node_owner,
                    const ServiceCallback &cb,
                    const ServiceConf &conf);

    ~InternalService() override;

    void ShutdownImpl() override;

    void ToggleSwitch(const bool enable) override;

 private:
    NodeOwnerBase *const node_owner_;
    ServiceCallback user_cb_;
    std::shared_ptr<MwServiceHandle> service_handle_;
};

template <typename Request, typename Response>
InternalService<Request, Response>::InternalService(
    NodeOwnerBase *const node_owner,
    const ServiceCallback &cb,
    const ServiceConf &conf)
    : Service<Request, Response>(conf),
      node_owner_(node_owner),
      user_cb_(cb) {
    ::Shutdownable::UpdateTTLogDetail("InternalService|" + conf.service_name());
    ToggleSwitch(true);
}

template <typename Request, typename Response>
InternalService<Request, Response>::~InternalService() {
    ::Shutdownable::Shutdown();
}

template <typename Request, typename Response>
void InternalService<Request, Response>::ShutdownImpl() {
    ToggleSwitch(false);
    if (service_handle_ != nullptr) {
        service_handle_->Shutdown();
    }
    Service<Request, Response>::Shutdown();
}

template <typename Request, typename Response>
void InternalService<Request, Response>::ToggleSwitch(const bool enable) {
    if (enable) {
        auto conf = static_cast<ServiceBase *>(this)->GetServiceConf();
        service_handle_ = senseAD::mw::comm::Service::New(
            node_owner_->InnerHandle(),
            [user_cb = user_cb_](const auto &req, auto &resp) {
                auto rreq = std::make_shared<ReceivedMsg<Request>>(req);
                auto rresp = std::make_shared<SendMsg<Response>>();
                user_cb(rreq, rresp);
                if (rresp == nullptr) {
                    AD_LFATAL(InternalService) << "Not allow nullptr response";
                    throw std::runtime_error("Not allow nullptr response");
                }
                resp = rresp->Finalize().BorrowRawWithRef(rresp);
            },
            ToMwServiceConf(conf));
        if (service_handle_) {
            this->sd_client_sptr_->AddService(node_owner_->GetName(),
                                              conf.service_name());
        }
    } else {
        if (service_handle_) {
            auto conf = static_cast<ServiceBase *>(this)->GetServiceConf();
            this->sd_client_sptr_->LeaveService(node_owner_->GetName(),
                                                conf.service_name());
        }
        service_handle_ = nullptr;
    }
}

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
