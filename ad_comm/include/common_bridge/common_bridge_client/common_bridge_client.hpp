/**
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * chenjian <chenjian5@senseauto.com>
 */
#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <deque>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <condition_variable>

#include "ad_rscl/ad_rscl.h"
#include "common_bridge/common_bridge_client/zmq_forward_pub.h"

namespace senseAD {
namespace rscl {
namespace common_bridge {

struct ChannelInfo {
    std::string channel_name;
    // An unique short string to identify the message type
    std::string type;
    // An descriptor for serialization framework to recover message from binary
    // buffer
    std::string descriptor;
};
constexpr int kMaxInMemoryItem = 4096;

using senseAD::rscl::comm::BorrowedRawMessage;
using RawMessage = senseAD::mw::RawMessage;

struct ClientTopicInfo {
    struct alignas(64) RecvInfo {
        uint64_t recv_timestamp_ns = 0;
        uint64_t recv_bytes = 0;
        uint64_t recv_seq = 0;
    } recv_info;

    struct alignas(64) SendInfo {
        uint64_t send_timestamp_ns = 0;
        uint64_t send_bytes = 0;
        uint64_t send_seq = 0;
    } send_info;

    friend std::ostream &operator<<(std::ostream &os,
                                    const ClientTopicInfo &info) {
        os << "\n\trecv_timestamp_ns\trecv_bytes\trecv_seq\tsend_timestamp_"
              "ns"
              "\tsend_bytes\tsend_seq\n";
        os << '\t' << info.recv_info.recv_timestamp_ns << '\t'
           << info.recv_info.recv_bytes << '\t' << info.recv_info.recv_seq
           << '\t' << info.send_info.send_timestamp_ns << '\t'
           << info.send_info.send_bytes << '\t' << info.send_info.send_seq
           << '\n';
        return os;
    }

    bool Empty() const {
        return !(recv_info.recv_timestamp_ns & recv_info.recv_bytes &
                 recv_info.recv_seq & send_info.send_timestamp_ns &
                 send_info.send_bytes & send_info.send_seq);
    }
};

class CommonBridgeClient {
 public:
    explicit CommonBridgeClient(const std::string &data_addr = "",
                                uint32_t port = 0);
    ~CommonBridgeClient();

    bool Start(const std::vector<std::string> &topic_excluded,
               const std::vector<std::string> &topic_included);
    bool Stop();

 private:
    void MessageIoThread();
    void StatRecordDumpThread();
    bool RegisterChannel(const std::string &channel_name);
    void AddSubscriber();
    void NewMessageCallback(const std::shared_ptr<ReceivedMsg<RawMessage>> &msg,
                            const std::string &channel_name,
                            std::shared_ptr<ZmqForwardPublisher> pub);
    std::shared_ptr<rscl::Node> node_;
    senseAD::service_discovery::ServiceDiscovery *service_discovery_ = nullptr;
    bool running_ = false;

    std::shared_timed_mutex start_stop_mtx_;
    std::vector<std::string> forward_topic_list_;
    std::unordered_set<std::string> excluded_channels_;
    std::vector<rscl::SubscriberBase::Ptr> subs_;
    std::queue<std::shared_ptr<ChannelInfo>> channel_q_;
    std::thread subs_thread_;
    std::mutex register_mtx_;
    std::unordered_map<std::string, std::shared_ptr<ChannelInfo>>
        registered_channels_;
    std::unordered_map<std::string, std::shared_ptr<ZmqForwardPublisher>> pubs_;
    std::condition_variable sub_cv_;
    std::unordered_map<std::string, int> failed_msg_type_cnts_;

    std::thread io_thread_;
    std::mutex msg_mtx_;
    std::deque<std::tuple<std::shared_ptr<ReceivedMsg<RawMessage>>,
                          std::shared_ptr<ZmqForwardPublisher>>>
        msg_q_;

    uint64_t dropped_count_{0UL};
    uint64_t all_received_count_{0UL};
    std::atomic<uint64_t> all_sent_count_{0UL};
    std::string target_zmq_addr_;

    std::mutex stat_record_mtx_;
    std::thread print_stat_thread_;
    std::unordered_map<std::string, ClientTopicInfo> client_stat_record_;
};
}  // namespace common_bridge
}  // namespace rscl
}  // namespace senseAD
