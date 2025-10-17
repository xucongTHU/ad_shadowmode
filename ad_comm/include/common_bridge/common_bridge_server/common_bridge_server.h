/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 * WangXiantao <wangxiantao@senseauto.com>
 */

#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include <algorithm>
#include <cppzmq/zmq.hpp>
#include "common_bridge/common/ip_utils.h"
#include "common_bridge/common/protocol.h"
#include "ad_rscl/ad_rscl.h"
#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace rscl {
namespace common_bridge {

using senseAD::rscl::comm::BorrowedRawMessage;
using MsgObserverFn = std::function<void(const char*)>;

struct TopicInfo {
    void Set(const std::string& p_uuid,
             const std::string& p_topic,
             const std::string& p_type_name,
             const std::string& p_descriptor) {
        uuid = p_uuid;
        topic = p_topic;
        type_name = p_type_name;
        descriptor = p_descriptor;
    }
    std::string uuid;
    std::string topic;
    std::string type_name;
    std::string descriptor;
};

class CommonBridgeServer {
 public:
    CommonBridgeServer(const int listen_port = kZmqPort,
                       const int recv_timeout_ms = 1000);

    ~CommonBridgeServer();

    // non-blocking
    void Start();
    void Stop();

 private:
    void RegisterTopicDescriptor(const std::string& uuid,
                                 const std::string& topic,
                                 const std::string& type_name,
                                 const std::string& descriptor);

    void PublishMsg(const std::string& topic,
                    const std::string& uuid,
                    const char* data_ptr,
                    const uint64_t data_size);
    bool SendControlMsg(const ControlPacket& packet);
    bool Recv();
    void VerifyDataMsgSeq(uint64_t new_seq);
    void RepublishMessageThread();

    bool running_{true};
    std::shared_ptr<Node> node_;
    // uuid, publisher
    std::unordered_map<std::string, rscl::Publisher<BorrowedRawMessage>::Ptr>
        pubs_;
    std::unordered_map<std::string, TopicInfo> topic_info_;
    std::unique_ptr<zmq::context_t> zmq_ctx_ = nullptr;
    std::unique_ptr<zmq::socket_t> data_sub_sock_ = nullptr;
    std::unique_ptr<zmq::socket_t> control_pub_sock_ = nullptr;
    std::thread bg_recv_thread_;
    bool is_running_ = false;

    bool data_channel_established_{false};
    uint64_t last_seq_{0};

    std::mutex new_republish_mtx_;
    std::queue<zmq::message_t> new_republish_msgs_;
    std::condition_variable new_republish_cv_;
    std::thread republish_thread_;
};

}  // namespace common_bridge
}  // namespace rscl
}  // namespace senseAD
