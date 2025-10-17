/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once

#include <cppzmq/zmq.hpp>
#include <thread>
#include <memory>
#include <string>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "ad_mw/types.h"
#include "ad_base/common/macros.h"
#include "common_bridge/common/protocol.h"

namespace senseAD {
namespace rscl {
namespace common_bridge {

struct ZmqForwardConfig {
    std::string receive_ip;
    uint32_t base_port;
};

struct PublisherAttr {
    std::string publisher_id;
    std::string topic;
    std::string msg_type;
    std::string msg_descriptor;
    uint64_t seq = 0;
    bool is_send_reg_topic = false;
};

struct ActionMessage {
    int action;
    std::string publisher_id;
};

struct Message {
    uint64_t receievd_time;
    std::string publisher_id;
    std::shared_ptr<senseAD::mw::BorrowedRawMessage> msg;
};

inline std::string PackDataZmqAddr(const std::string& receive_ip,
                                   uint32_t target_port) {
    return "tcp://" + receive_ip + ":" + std::to_string(target_port);
}

class ZmqForwardSession {
 public:
    ~ZmqForwardSession();
    bool Init(const std::string& data_addr = "", uint32_t port = 0);
    bool AddPublisher(const PublisherAttr& info);
    bool Publish(const std::string& publisher_id,
                 const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg);
    void SetWhitelistTopics(const std::unordered_set<std::string>& topics) {
        whitelist_topics_ = topics;
    }

 private:
    void DataPubLoop() noexcept;
    void ControlSubLoop() noexcept;
    bool SendMessage(const std::string& publisher_id,
                     const std::vector<char>& data_buf);
    bool SendDataMessage(const std::string& publisher_id,
                         const std::vector<char>& data_buf);
    bool SendActionMessage(const std::string& publisher_id);
    void Close();

 private:
    std::atomic<bool> is_running_{false};
    std::unique_ptr<zmq::context_t> zmq_ctx_ = nullptr;

    std::shared_ptr<zmq::socket_t> data_pub_sock_;
    std::weak_ptr<zmq::socket_t> data_pub_weak_sock_;

    std::shared_ptr<zmq::socket_t> control_sub_sock_;
    std::weak_ptr<zmq::socket_t> control_sub_weak_sock_;
    zmq_pollitem_t zmq_pollitem_;

    std::unique_ptr<std::thread> data_pub_thread_ = nullptr;
    std::unique_ptr<std::thread> control_sub_thread_ = nullptr;

    std::mutex action_mtx_;
    std::queue<ActionMessage> action_queue_;

    std::mutex new_msg_mtx_;
    std::condition_variable new_msg_cv_;
    std::queue<Message> new_msg_queue_;

    std::mutex publisher_info_mtx_;
    std::unordered_map<std::string, PublisherAttr> publisher_info_;
    std::unordered_map<std::string, std::vector<char>> latest_data_buf_;
    std::vector<char> data_buf_;

    std::unordered_set<std::string> whitelist_topics_;
    std::string data_bind_addr_;
    uint64_t send_bytes_{0};
};

class ZmqForwardSessionManager {
 public:
    ~ZmqForwardSessionManager();
    bool Init(const std::string& data_addr, uint32_t port);
    void SetWhitelistTopics(const std::string& data_addr,
                            const std::unordered_set<std::string>& topics);
    bool AddPublisher(const PublisherAttr& info);
    bool Publish(const std::string& publisher_id,
                 const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg);

 private:
    std::unordered_map<std::string, std::shared_ptr<ZmqForwardSession>>
        sessions_;
    std::mutex session_mtx_;
    DECLARE_AD_SINGLETON(ZmqForwardSessionManager)
};

}  // namespace common_bridge
}  // namespace rscl
}  // namespace senseAD
