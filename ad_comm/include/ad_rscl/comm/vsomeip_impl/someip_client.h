/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * caiwenkang <caiwenkang@senseauto.com>
 */

#pragma once

#include <memory>
#include <chrono>
#include <future>
#include <mutex>
#include <condition_variable>
#include <map>
#include <atomic>
#include <functional>
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
using ServiceCallback = std::function<void(const std::shared_ptr<ResponseT>&,
                                           vsomeip_v3::return_code_e)>;

template <typename RequestT, typename ResponseT>
class SomeIPClient {
 public:
    using Ptr = std::shared_ptr<SomeIPClient<RequestT, ResponseT>>;

    explicit SomeIPClient(const std::shared_ptr<SomeipWrapper>& app,
                          const std::string& service_name,
                          const SomeipMethodConfig& config)
        : config_(config), service_name_(service_name), someip_app_(app) {
        if (!Init()) {
            DeInit();
            throw std::runtime_error("Failed to initialize SOME/IP client");
        }
    }

    ~SomeIPClient() noexcept { DeInit(); }

    std::pair<std::shared_ptr<ResponseT>, vsomeip_v3::return_code_e>
    SendRequest(
        const RequestT& request,
        std::chrono::milliseconds timeout = std::chrono::milliseconds(5000)) {
        std::promise<
            std::pair<std::shared_ptr<ResponseT>, vsomeip_v3::return_code_e>>
            response_promise;
        auto response_future = response_promise.get_future();

        // Set callback for this specific request
        auto callback = [&response_promise](
                            const std::shared_ptr<ResponseT>& response,
                            vsomeip_v3::return_code_e result) {
            response_promise.set_value(std::make_pair(response, result));
        };

        // Register callback for the request
        SetResponseCallback(callback);

        // Send the request
        if (!SendRequestInternal(request)) {
            std::shared_ptr<ResponseT> empty_response = nullptr;
            response_promise.set_value(
                std::make_pair(empty_response,
                               vsomeip_v3::return_code_e::E_MALFORMED_MESSAGE));
        }

        // Wait for the response with timeout
        if (response_future.wait_for(timeout) == std::future_status::timeout) {
            // Cleanup callback on timeout
            ResetResponseCallback();
            std::shared_ptr<ResponseT> empty_response = nullptr;
            return std::make_pair(empty_response,
                                  vsomeip_v3::return_code_e::E_TIMEOUT);
        }

        return response_future.get();
    }

    bool AsyncSendRequest(
        const RequestT& request,
        const ServiceCallback<RequestT, ResponseT>& callback) {
        // Set the callback for this request
        SetResponseCallback(callback);

        // Send the request
        return SendRequestInternal(request);
    }

 private:
    bool IsAvailable() const { return is_available_; }
    bool Init() {
        if (someip_app_) {
            someip_app_->RequestService(config_.config);
            someip_app_->RegAvailabilityHandler(
                config_.config,
                [this](vsomeip_v3::service_t service_id,
                       vsomeip_v3::instance_t instance_id, bool available) {
                    AD_LINFO(SomeIPClient) << "Service id: " << service_id
                                           << ", instance id: " << instance_id
                                           << ", available: " << available;
                    is_available_ = available;
                });
            return true;
        } else {
            AD_LERROR(SomeIPClient)
                << "Failed to initialize client, someip_app is null";
            return false;
        }
    }

    void DeInit() noexcept {
        try {
            if (someip_app_) {
                someip_app_->ReleaseService(config_.config);
                someip_app_->UnregAvailabilityHandler(config_.config);
                // someip_app_->ClearAllHandler();
            }
        } catch (const std::exception& e) {
            AD_LERROR(SomeIPClient)
                << "Exception during client cleanup: " << e.what();
        }
    }

    void SetResponseCallback(
        const ServiceCallback<RequestT, ResponseT>& callback) {
        std::lock_guard<std::mutex> lock(callback_mutex_);
        response_callback_ = callback;

        if (!response_handler_set_) {
            response_handler_ = [this](
                                    const std::shared_ptr<vsomeip_v3::message>&
                                        msg) {
                if (msg->get_message_type() !=
                        vsomeip_v3::message_type_e::MT_RESPONSE &&
                    msg->get_message_type() !=
                        vsomeip_v3::message_type_e::MT_ERROR) {
                    return;
                }
                auto payload = msg->get_payload();

                if (!payload) {
                    AD_LERROR(SomeIPClient) << "Received empty payload";
                    return;
                }

                vsomeip_v3::return_code_e result =
                    static_cast<vsomeip_v3::return_code_e>(
                        msg->get_return_code());

                // Get the current callback
                ServiceCallback<RequestT, ResponseT> callback = nullptr;
                {
                    std::lock_guard<std::mutex> lock(callback_mutex_);
                    callback = response_callback_;
                    // Reset callback after processing for sync requests
                    // For async requests, user should handle this
                    response_callback_ = nullptr;
                }

                if (callback) {
                    auto response = std::make_shared<ResponseT>();
                    if (result == vsomeip_v3::return_code_e::E_OK &&
                        payload->get_length() > 0) {
                        uint16_t e2e_offset = 0;
                        if (config_.IsE2EEnabled()) {
                            e2e_offset = kE2EDataSize;
                        }

                        if (!Deserialize(response.get(),
                                         payload->get_data() + e2e_offset,
                                         payload->get_length())) {
                            AD_LERROR(SomeIPClient)
                                << "Failed to deserialize response";
                            result =
                                vsomeip_v3::return_code_e::E_MALFORMED_MESSAGE;
                        }
                    }
                    callback(response, result);
                } else {
                    AD_LERROR(SomeIPClient)
                        << "Received response but no callback registered";
                }
            };

            someip_app_->RegMsgHandler(config_, response_handler_);
            response_handler_set_ = true;
        }
    }

    void ResetResponseCallback() {
        std::lock_guard<std::mutex> lock(callback_mutex_);
        response_callback_ = nullptr;
    }

    bool SendRequestInternal(const RequestT& request) {
        if (!IsAvailable()) {
            AD_LERROR(SomeIPClient) << "Service not available";
            return false;
        }
        try {
            size_t buffer_size = SerializedSize(request);
            uint16_t e2e_offset = 0;

            if (config_.IsE2EEnabled()) {
                buffer_size += kE2EDataSize;
                e2e_offset = kE2EDataSize;
            }

            std::vector<uint8_t> payload(buffer_size, 0);
            if (!Serialize(request, payload.data() + e2e_offset, buffer_size)) {
                AD_LERROR(SomeIPClient) << "Failed to serialize request";
                return false;
            }

            // Let the SOME/IP stack handle request ID generation
            someip_app_->SendRequest(config_, std::move(payload));
            return true;
        } catch (const std::exception& e) {
            AD_LERROR(SomeIPClient)
                << "Exception during request sending: " << e.what();
            return false;
        }
    }

    std::string service_name_;
    std::shared_ptr<SomeipWrapper> someip_app_;
    SomeipMethodConfig config_;

    std::mutex callback_mutex_;
    ServiceCallback<RequestT, ResponseT> response_callback_;
    vsomeip::message_handler_t response_handler_;
    bool response_handler_set_ = false;
    std::atomic<bool> is_available_{false};
};

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
