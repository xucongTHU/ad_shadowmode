/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * caiwenkang <caiwenkang@senseauto.com>
 */

#pragma once

#include <memory>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "ad_log/ad_log.hpp"
#include "ad_base/common/env.h"
#include "ad_rscl/comm/node_impl.h"
#include "ad_rscl/comm/vsomeip_impl/someip_wrapper.h"
#include "ad_rscl/comm/vsomeip_impl/vsomeip_config_manager.h"
#include "ad_rscl/comm/vsomeip_impl/serialize/someip_serialize.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace someip {

template <typename RequestT, typename ResponseT>
using RequestHandler =
    std::function<std::shared_ptr<ResponseT>(const std::shared_ptr<RequestT>&)>;

template <typename RequestT, typename ResponseT>
class SomeIPService {
 public:
    using Ptr = std::shared_ptr<SomeIPService<RequestT, ResponseT>>;

    explicit SomeIPService(const std::shared_ptr<SomeipWrapper>& app,
                           const std::string& service_name,
                           const RequestHandler<RequestT, ResponseT>& handler,
                           const SomeipMethodConfig& config)
        : request_handler_(handler),
          config_(config),
          service_name_(service_name),
          someip_app_(app) {
        if (!Init()) {
            DeInit();
            throw std::runtime_error("Failed to initialize SOME/IP service");
        }
    }

    ~SomeIPService() noexcept { DeInit(); }

 private:
    bool Init() {
        if (!someip_app_) {
            AD_LERROR(SomeIPService)
                << "Failed to initialize service, someip_app is null";
            return false;
        }

        if (!request_handler_) {
            AD_LERROR(SomeIPService) << "Request handler is not set";
            return false;
        }

        someip_app_->OfferService(config_.config);
        // someip_app_->RequestService(config_.config);
        SetRequestCallback();
        return true;
    }

    void DeInit() noexcept {
        try {
            if (someip_app_) {
                someip_app_->StopOfferService(config_.config);
                // someip_app_->ReleaseService(config_.config);
            }
        } catch (const std::exception& e) {
            AD_LERROR(SomeIPService)
                << "Exception during service cleanup: " << e.what();
        }
    }

    void SetRequestCallback() {
        request_callback_ =
            [this](const std::shared_ptr<vsomeip_v3::message>& request) {
                if (request->get_message_type() !=
                        vsomeip_v3::message_type_e::MT_REQUEST &&
                    request->get_message_type() !=
                        vsomeip_v3::message_type_e::MT_REQUEST_NO_RETURN) {
                    return;
                }
                if (!request_handler_) {
                    AD_LERROR(SomeIPService) << "No request handler set";
                    return;
                }
                auto payload = request->get_payload();
                if (!payload || payload->get_length() == 0) {
                    AD_LERROR(SomeIPService)
                        << "Received empty request payload";
                    return;
                }

                auto request_data = std::make_shared<RequestT>();
                uint16_t e2e_offset = 0;
                if (config_.IsE2EEnabled()) {
                    e2e_offset = kE2EDataSize;
                }
                if (!Deserialize(request_data.get(),
                                 payload->get_data() + e2e_offset,
                                 payload->get_length())) {
                    AD_LERROR(SomeIPService) << "Failed to deserialize request";
                    return;
                }

                std::shared_ptr<ResponseT> response;
                response = request_handler_(request_data);
                if (response) {
                    size_t buffer_size = SerializedSize(*response);
                    uint16_t e2e_offset = 0;

                    if (config_.IsE2EEnabled()) {
                        buffer_size += kE2EDataSize;
                        e2e_offset = kE2EDataSize;
                    }

                    std::vector<uint8_t> response_payload(buffer_size, 0);
                    if (!Serialize(*response,
                                   response_payload.data() + e2e_offset,
                                   buffer_size)) {
                        AD_LERROR(SomeIPService)
                            << "Failed to serialize response";
                        return;
                    }
                    someip_app_->SendResponse(config_, request,
                                              std::move(response_payload));
                }
            };

        someip_app_->RegMsgHandler(config_, request_callback_);
    }

    RequestHandler<RequestT, ResponseT> request_handler_;
    vsomeip::message_handler_t request_callback_;
    std::string service_name_;
    std::shared_ptr<SomeipWrapper> someip_app_;
    SomeipMethodConfig config_;
};

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
