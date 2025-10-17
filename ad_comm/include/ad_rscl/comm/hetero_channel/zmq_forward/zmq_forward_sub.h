/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cppzmq/zmq.hpp>

#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <unordered_map>

#include "ad_base/common/macros.h"
#include "ad_base/common/shutdownable.h"
#include "ad_rscl/comm/hetero_channel/basic_type.h"
#include "ad_rscl/idl/hetero_channel_config.pb.h"
#include "ad_rscl/comm/hetero_channel/zmq_forward/zmq_forward_bridge_service.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

using senseAD::rscl::idl::ZmqForwardChannelConfig;

class ZmqForwardReceivedBuffer final : public Buffer {
 public:
    ZmqForwardReceivedBuffer(const char* data_ptr, const size_t size)
        : aligned_buffer_(data_ptr, size) {}
    ~ZmqForwardReceivedBuffer() final {}
    size_t ByteSize() const final { return aligned_buffer_.size(); }
    const char* Bytes() const final { return aligned_buffer_.data(); }

 private:
    std::string aligned_buffer_;
};

class ZmqForwardBridgeServiceManager : public ::Shutdownable {
 public:
    ~ZmqForwardBridgeServiceManager() override;

    void ShutdownImpl() override;
    int AddHandlerForConnection(
        const ZmqForwardChannelConfig& config,
        ZmqForwardBridgeService::MsgDataHandler&& handler);
    void ClearHandlerForConnection(const int token);

 private:
    struct ZmqRecvService {
        ~ZmqRecvService() {
            running = false;
            if (recv_thread.joinable()) {
                recv_thread.join();
            }
        }
        std::shared_ptr<ZmqForwardBridgeService> service;
        std::mutex mtx;
        std::thread recv_thread;
        std::string hashed_key;
        bool running = true;

        std::vector<ZmqForwardBridgeService::MsgDataHandler> on_data_fns;
    };

    std::atomic<bool> is_running_{false};
    std::mutex g_mtx_;

    int base_token_ = 0;
    // key: service hashed key, value: service weak ptr
    std::unordered_map<std::string, std::weak_ptr<ZmqRecvService>> services_;

    // key: token, value: service shared ptr
    std::unordered_map<int, std::shared_ptr<ZmqRecvService>>
        token_services_map_;

    DECLARE_AD_SINGLETON(ZmqForwardBridgeServiceManager)
};

class ZmqForwardSubscriber : public SubscriberBase {
 public:
    ZmqForwardSubscriber(const std::string& topic,
                         const ZmqForwardChannelConfig& config,
                         const CallbackFunc& fn);
    ZmqForwardSubscriber(const std::string& topic,
                         const ZmqForwardChannelConfig& config,
                         CallbackFunc&& fn);
    ~ZmqForwardSubscriber() override;

    std::string Type() const override;
    bool Establish() override;
    bool Close() override;
    bool IsEstablished() const override;
    void ShutdownImpl() override;

 private:
    std::string topic_;
    ZmqForwardChannelConfig config_;

    std::atomic<bool> is_running_{false};

    int bridge_token_ = -1;
};

}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
