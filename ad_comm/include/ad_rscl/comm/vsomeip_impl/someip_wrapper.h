/**
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Gongshusheng<Gongshusheng@senseauto.com>
 */
#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <vsomeip/vsomeip.hpp>

#include "ad_base/common/env.h"
#include "ad_log/ad_log.hpp"
#include "ad_rscl/comm/vsomeip_impl/common.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace someip {

// to manage vsomeip::app : init() -> start() -> stop()
class SomeipWrapper {
 public:
    SomeipWrapper() = delete;
    explicit SomeipWrapper(const std::string& node_name) {
        std::string vsomeip_config_file =
            senseAD::base::common::SenseautoConfigDir() +
            "/resource_cfg/rscl_config/vsomeip.json";
        senseAD::base::common::SetEnvironmentVariable("VSOMEIP_CONFIGURATION",
                                                      vsomeip_config_file);
        app_ptr_ = vsomeip::runtime::get()->create_application(node_name);
        app_ptr_->init();
        thread_ptr_ =
            std::make_unique<std::thread>([&]() { app_ptr_->start(); });
    }

    ~SomeipWrapper() noexcept {
        try {
            Cleanup();
        } catch (...) {
            AD_LERROR(SomeipWrapper) << "Exception during cleanup ignored";
        }
    }
    void ClearAllHandler() { app_ptr_->clear_all_handler(); }

    void Stop() { app_ptr_->stop(); }

    // service dicovery wrap
    void OfferService(const SomeipConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->offer_service(config.service_id, config.instance_id,
                                config.major_version, config.minor_version);
    }

    void StopOfferService(const SomeipConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->stop_offer_service(config.service_id, config.instance_id,
                                     config.major_version,
                                     config.minor_version);
    }

    void RequestService(const SomeipConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->request_service(config.service_id, config.instance_id,
                                  config.major_version, config.minor_version);
    }

    void ReleaseService(const SomeipConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->release_service(config.service_id, config.instance_id);
    }

    void RegAvailabilityHandler(const SomeipConfig& config,
                                vsomeip::availability_handler_t cb) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->register_availability_handler(config.service_id,
                                                config.instance_id, cb, 1, 0);
    }

    void UnregAvailabilityHandler(const SomeipConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->unregister_availability_handler(config.service_id,
                                                  config.instance_id, 1, 0);
    }
    // Event related methods
    void OfferEvent(const SomeipEventConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        std::set<vsomeip::eventgroup_t> its_groups{config.event_group_id};
        app_ptr_->offer_event(config.service_id(), config.instance_id(),
                              config.event_id, its_groups,
                              vsomeip::event_type_e::ET_EVENT,
                              std::chrono::milliseconds::zero(), false, true,
                              nullptr, vsomeip::reliability_type_e::RT_UNKNOWN);
    }

    void StopOfferEvent(const SomeipEventConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->stop_offer_event(config.service_id(), config.instance_id(),
                                   config.event_id);
    }

    void RegMsgHandler(const SomeipMethodConfig& config,
                       vsomeip::message_handler_t cb) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->register_message_handler(
            config.service_id(), config.instance_id(), config.method_id, cb);
    }

    void UnregMsgHandler(const SomeipMethodConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->unregister_message_handler(
            config.service_id(), config.instance_id(), config.method_id);
    }

    void RegMsgHandler(const SomeipEventConfig& config,
                       vsomeip::message_handler_t cb) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->register_message_handler(
            config.service_id(), config.instance_id(), config.event_id, cb);
    }

    void UnregMsgHandler(const SomeipEventConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->unregister_message_handler(
            config.service_id(), config.instance_id(), config.event_id);
    }

    void Subscribe(const SomeipEventConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->subscribe(
            config.service_id(), config.instance_id(), config.event_group_id,
            static_cast<uint8_t>(config.major_version()), config.event_id);
    }

    void Unsubscribe(const SomeipEventConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->unsubscribe(config.service_id(), config.instance_id(),
                              config.event_group_id);
    }

    // sub event or field
    void RequestEvent(const SomeipEventConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        std::set<vsomeip::eventgroup_t> its_groups{config.event_group_id};

        vsomeip::event_type_e type_ =
            static_cast<vsomeip::event_type_e>(config.event_type);

        app_ptr_->request_event(config.service_id(), config.instance_id(),
                                config.event_id, its_groups, type_);
    }

    void ReleaseEvent(const SomeipEventConfig& config) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->release_event(config.service_id(), config.instance_id(),
                                config.event_id);
    }

    // handler for service side know client's subscription
    void RegSubStateChangeHandler(const SomeipEventConfig& config,
                                  vsomeip::subscription_handler_sec_t cb) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->register_subscription_handler(config.service_id(),
                                                config.instance_id(),
                                                config.event_group_id, cb);
    }

    // handler for client
    // TODO(gongshusheng): Consider if _is_selective should use default value
    void RegSubStateListenerHandler(const SomeipEventConfig& config,
                                    vsomeip::subscription_status_handler_t cb) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        app_ptr_->register_subscription_status_handler(
            config.service_id(), config.instance_id(), config.event_group_id,
            config.event_id, cb);
    }

    void Notify(const SomeipEventConfig& config,
                const std::shared_ptr<std::vector<uint8_t>>& raw_data) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        auto payload = vsomeip::runtime::get()->create_payload();
        payload->set_data(raw_data->data(), raw_data->size());
        app_ptr_->notify(config.service_id(), config.instance_id(),
                         config.event_id, payload);
    }

    void Notify(const SomeipEventConfig& config,
                std::vector<uint8_t>&& raw_data) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        auto payload = vsomeip::runtime::get()->create_payload();
        payload->set_data(std::move(raw_data));
        app_ptr_->notify(config.service_id(), config.instance_id(),
                         config.event_id, payload);
    }

    void Send(std::shared_ptr<vsomeip::message> message) {
        // LOG("Send msg");
        app_ptr_->send(message);
    }

    void SendRequest(const SomeipMethodConfig& config,
                     std::vector<uint8_t>&& raw_data) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        auto payload = vsomeip::runtime::get()->create_payload();
        payload->set_data(std::move(raw_data));
        auto message = vsomeip::runtime::get()->create_request();
        message->set_service(config.service_id());
        message->set_instance(config.instance_id());
        message->set_method(config.method_id);
        message->set_interface_version(config.major_version());
        message->set_message_type(vsomeip::message_type_e::MT_REQUEST);
        message->set_return_code(vsomeip::return_code_e::E_OK);
        message->set_payload(payload);
        app_ptr_->send(message);
    }

    void SendResponse(const SomeipMethodConfig& config,
                      const std::shared_ptr<vsomeip::message>& request,
                      std::vector<uint8_t>&& raw_data) {
        if (!config.IsValid()) {
            AD_LERROR(SomeipWrapper) << "Invalid config: " << config.ToString();
            return;
        }
        auto payload = vsomeip::runtime::get()->create_payload();
        payload->set_data(std::move(raw_data));
        auto message = vsomeip::runtime::get()->create_response(request);
        message->set_return_code(vsomeip::return_code_e::E_OK);
        message->set_message_type(vsomeip::message_type_e::MT_RESPONSE);
        message->set_reliable(false);
        message->set_payload(payload);
        app_ptr_->send(message);
    }

    vsomeip::client_t GetClientId() { return app_ptr_->get_client(); }

 private:
    void Cleanup() {
        if (app_ptr_) {
            app_ptr_->clear_all_handler();
            app_ptr_->stop();
        }
        if (thread_ptr_ && thread_ptr_->joinable()) {
            thread_ptr_->join();
        }
    }
    std::shared_ptr<vsomeip::application> app_ptr_;

    // offer_thread ,app->stop() thread exit
    std::unique_ptr<std::thread> thread_ptr_;

    std::atomic<bool> is_running_{false};
    std::atomic<bool> is_offered_{false};
    std::mutex notify_mutex_;
    std::condition_variable notify_condition_;
};

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
