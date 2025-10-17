/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * caiwenkang <caiwenkang@senseauto.com>
 */
#pragma once

#include <thread>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <utility>

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

template <typename M0>
using MessageCallback = std::function<void(const std::shared_ptr<M0>&)>;

template <typename MessageT>
class SomeIPSubscriber {
 public:
    using Ptr = std::shared_ptr<SomeIPSubscriber<MessageT>>;

    explicit SomeIPSubscriber(const std::shared_ptr<SomeipWrapper>& app,
                              const std::string& topic_name,
                              const MessageCallback<MessageT>& cb,
                              const SomeipEventConfig& config)
        : user_cb_(cb),
          config_(config),
          topic_name_(topic_name),
          someip_app_(app) {
        if (!Init()) {
            DeInit();
            throw std::runtime_error("Failed to initialize subscriber");
        }
    }
    ~SomeIPSubscriber() noexcept { DeInit(); }

 private:
    bool Init() {
        if (someip_app_) {
            someip_app_->RequestService(config_.config);
            SetMessageCallback();
            someip_app_->RequestEvent(config_);
            someip_app_->Subscribe(config_);
        } else {
            AD_LERROR(SomeIPSubscriber) << "Failed to initialize subscriber";
            return false;
        }
        return true;
    }
    void DeInit() noexcept {
        if (someip_app_) {
            try {
                someip_app_->Unsubscribe(config_);
                someip_app_->ReleaseEvent(config_);
                someip_app_->ReleaseService(config_.config);
            } catch (const std::exception& e) {
                AD_LERROR(SomeIPSubscriber)
                    << "Exception during cleanup: " << e.what();
            }
        }
    }
    void SetMessageCallback() {
        real_cb_ = [this](const std::shared_ptr<vsomeip_v3::message>& msg) {
            if (user_cb_) {
                auto data = std::make_shared<MessageT>();
                auto payload = msg->get_payload();

                uint16_t e2e_offset = 0;
                if (config_.IsE2EEnabled()) {
                    e2e_offset = kE2EDataSize;
                }
                if (!Deserialize(data.get(), payload->get_data() + e2e_offset,
                                 payload->get_length())) {
                    AD_LERROR(SomeIPSubscriber)
                        << "Failed to deserialize message";
                }
                user_cb_(data);
            }
        };
        someip_app_->RegMsgHandler(config_, real_cb_);
        // someip_app_->RegSubStateListenerHandler(config_, cb_);
    }

    MessageCallback<MessageT> user_cb_;
    vsomeip::message_handler_t real_cb_;
    SomeipEventConfig config_;
    std::string topic_name_;
    std::shared_ptr<SomeipWrapper> someip_app_;
    VSOMEIPConfigManager* vsomeip_config_manager_;
};

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
