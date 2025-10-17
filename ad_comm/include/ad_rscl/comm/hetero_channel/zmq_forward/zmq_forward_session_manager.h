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

#include "ad_base/common/macros.h"
#include "ad_base/common/socket.h"
#include "ad_base/common/shutdownable.h"
#include "ad_rscl/comm/hetero_channel/zmq_forward/protocol.h"
#include "ad_rscl/comm/hetero_channel/common/zmq_monitor.h"
#include "ad_rscl/comm/hetero_channel/basic_type.h"
#include "ad_rscl/idl/hetero_channel_config.pb.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

using senseAD::rscl::idl::ZmqForwardChannelConfig;

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
    std::string publisher_id;
    std::shared_ptr<senseAD::mw::BorrowedRawMessage> msg;
};

class ZmqForwardSessionManager : public ::Shutdownable {
 public:
    ~ZmqForwardSessionManager() override;

    void ShutdownImpl() override;
    bool Init(const std::string& publisher_id,
              const std::string& topic,
              const ZmqForwardChannelConfig& config);

    bool AddPublisher(const PublisherAttr& info);
    bool RemovePublisher(const std::string& publisher_id);

    bool Publish(const std::string& publisher_id,
                 const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg);

    void Close();

 private:
    void LoadExtraConfig(const std::string& topic);
    bool RegToForwardServer(const ZmqForwardChannelConfig& config);
    void InitDataConnectionSession(const std::string& publisher_id,
                                   const ZmqForwardChannelConfig& config);
    void InitSessionManagerOnce();
    void LoadExtraConfig(const ZmqForwardChannelConfig& config);

    void DataPubLoop() noexcept;
    void SendAllDataMsgPacket();
    void SendAllDataMsgPacketWithConcat();
    void ControlSubLoop() noexcept;

    void BuildHostInfo();
    void SendAuthRequestToServer(bool is_online, zmq::socket_t* sock);
    void ProcessHostAuthReply(const zmq::message_t& msg);

    bool SendMessage(const std::string& publisher_id,
                     const std::vector<char>& data_buf);
    bool SendDataMessage(const std::string& publisher_id);
    bool SendActionMessage(const std::string& publisher_id);

 private:
    // todo(someone): refactor it because this class contain too much variables
    std::atomic<bool> is_running_{false};
    std::unique_ptr<zmq::context_t> zmq_ctx_ = nullptr;

    std::mutex data_pub_mtx_;
    std::unordered_map<std::string,
                       std::shared_ptr<base::common::TcpFrameClient>>
        data_pub_tcp_cli_;
    std::unordered_map<std::string, std::weak_ptr<base::common::TcpFrameClient>>
        data_pub_weak_tcp_cli_;

    std::mutex control_sub_mtx_;
    std::unordered_map<std::string, std::shared_ptr<zmq::socket_t>>
        control_sub_sock_;
    std::unordered_map<std::string, std::weak_ptr<zmq::socket_t>>
        control_sub_weak_sock_;
    std::vector<zmq_pollitem_t> zmq_pollitem_;

    std::unique_ptr<std::thread> data_pub_thread_ = nullptr;
    std::unique_ptr<std::thread> control_sub_thread_ = nullptr;

    std::mutex user_auth_mtx_;
    // key: ip addr; value: socket
    std::unordered_map<std::string, std::shared_ptr<zmq::socket_t>>
        user_auth_sock_;
    // key: ip addr; value: socket reply status
    std::unordered_map<std::string, bool> auth_msgrecv_status_;
    HostInfoRequest host_info_;

    std::unordered_map<std::string, uint64_t> msg_seqs_;
    std::once_flag init_req_flag_;

    std::thread connect_manage_thread_;
    std::mutex thread_create_mtx_;

    std::mutex action_mtx_;
    std::queue<ActionMessage> action_queue_;

    std::mutex new_msg_mtx_;
    std::condition_variable new_msg_cv_;
    std::queue<Message> new_msg_queue_;

    std::mutex publisher_info_mtx_;
    std::unordered_map<std::string, PublisherAttr> publisher_info_;
    std::unordered_map<std::string, std::vector<char>> latest_data_buf_;
    std::vector<char> data_buf_;
    int32_t n_message_per_packet_ = 1;
    std::string topic_;

    DECLARE_AD_SINGLETON(ZmqForwardSessionManager)
};

}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
