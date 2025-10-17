/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * caiwenkang <caiwenkang@senseauto.com>
 */

#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <utility>
#include <string>
#include <vector>
#include <condition_variable>

#include <vsomeip/vsomeip.hpp>
#include "ad_log/ad_log.hpp"
#include "ad_base/common/env.h"
#include "ad_rscl/comm/vsomeip_impl/vsomeip_config_manager.h"
#include "ad_rscl/comm/vsomeip_impl/serialize/someip_serialize.h"
#include "ad_rscl/comm/vsomeip_impl/common.h"
#include "ad_rscl/comm/vsomeip_impl/someip_wrapper.h"
#include "ad_rscl/comm/node_impl.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace someip {

template <typename MessageT>
class SomeIPPublisher {
 public:
    using Ptr = std::shared_ptr<SomeIPPublisher>;

    explicit SomeIPPublisher(const std::shared_ptr<SomeipWrapper>& app,
                             const std::string& topic_name,
                             const SomeipEventConfig& config)
        : config_(config), topic_name_(topic_name), someip_app_(app) {
        if (!Init()) {
            DeInit();
            throw std::runtime_error("Failed to initialize publisher");
        }
    }
    ~SomeIPPublisher() noexcept { DeInit(); }
    bool Publish(const MessageT& msg) {
        return PublishImpl(std::make_shared<MessageT>(msg));
    }
    bool Publish(const MessageT&& msg) {
        return PublishImpl(std::make_shared<MessageT>(std::move(msg)));
    }

    bool Publish(const std::shared_ptr<MessageT>& msg) {
        return PublishImpl(msg);
    }

 private:
    bool Init() {
        if (someip_app_) {
            someip_app_->OfferService(config_.config);
            someip_app_->OfferEvent(config_);
        } else {
            AD_LERROR(SomeIPPublisher) << "Failed to initialize publisher";
            return false;
        }
        return true;
    }
    void DeInit() noexcept {
        try {
            if (someip_app_) {
                someip_app_->StopOfferService(config_.config);
                someip_app_->StopOfferEvent(config_);
            } else {
                AD_LERROR(SomeIPPublisher)
                    << "Failed to deinitialize publisher";
            }
        } catch (const std::exception& e) {
            AD_LERROR(SomeIPPublisher)
                << "Exception in deinitializing publisher: " << e.what();
        }
    }

    bool PublishImpl(const std::shared_ptr<MessageT>& msg) {
        const MessageT& original_data = *msg;
        size_t buffer_size = SerializedSize(original_data);

        try {
            uint16_t e2e_offset = 0;
            if (config_.IsE2EEnabled()) {
                buffer_size = buffer_size + kE2EDataSize;
                e2e_offset = kE2EDataSize;
            }
            std::vector<uint8_t> payload(buffer_size, 0);
            if (!Serialize(original_data, payload.data() + e2e_offset,
                           buffer_size)) {
                AD_LERROR(SomeIPPublisher) << "Failed to serialize message";
                return false;
            }
            someip_app_->Notify(config_, std::move(payload));
            return true;
        } catch (const std::exception& e) {
            AD_LERROR(SomeIPPublisher) << "Failed to publish: " << e.what();
            return false;
        }
        return true;
    }

    std::string topic_name_;
    std::shared_ptr<SomeipWrapper> someip_app_;
    SomeipEventConfig config_;
    VSOMEIPConfigManager* vsomeip_config_manager_;
};

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
