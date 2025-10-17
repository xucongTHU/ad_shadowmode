/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cppzmq/zmq.hpp>

#include <memory>
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "ad_base/common/socket.h"
#include "ad_rscl/comm/types.h"
#include "ad_rscl/comm/hetero_channel/common/zmq_monitor.h"
#include "ad_rscl/comm/hetero_channel/zmq_forward/protocol.h"
#include "ad_rscl/idl/hetero_channel_config.pb.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

constexpr uint32_t kHostInfoLeaseDuration = 10;

// key: source_ip, value: { key: topic, value: eval }
using ZmqRecvProp =
    std::unordered_map<std::string,
                       std::unordered_map<std::string, comm::TransmitEval>>;

class ZmqForwardBridgeService {
 public:
    // uuid, topic, type name, descriptor
    typedef std::function<void(const std::string&,
                               const std::string&,
                               const std::string&,
                               const std::string&)>
        RegTopicDescriptorHandler;

    // uuid, seq, pub_time, topic, data ptr, data size
    typedef std::function<void(const std::string&,
                               const uint64_t,
                               const uint64_t,
                               const std::string&,
                               const char*,
                               const uint64_t)>
        MsgDataHandler;

    // uuid
    typedef std::function<void(const std::string&)> UnknownUuidHandler;

    ZmqForwardBridgeService(const std::string& data_addr,
                            const std::string& accept_addr = "",
                            const int recv_timeout_ms = 200);

    ~ZmqForwardBridgeService();

    bool Recv();
    std::vector<HostInfoRequest> GetOnlineHosts() const;

    void SetOnRegTopicDescriptorHandler(
        const RegTopicDescriptorHandler& handler);
    void SetOnDataMsgDataHandler(const MsgDataHandler& handler);
    void SetUnknownUuidHandler(const UnknownUuidHandler& handler);
    void SetExclusive(bool ex) { exclusive_ = ex; }

    bool SendControlMsg(
        const senseAD::rscl::comm::hetero::ControlPacket& packet);

    uint32_t GetCurrConnectionCount() const { return curr_connection_; }

    const ZmqRecvProp& GetTransmitEval() {
        std::lock_guard<std::mutex> lock(recv_eval_mutex_);
        for (auto& kv : zmq_recv_props_) {
            for (auto& kv2 : kv.second) {
                kv2.second.UpdateFrameRatio();
            }
        }
        return zmq_recv_props_;
    }
    void LockTransmitEval() { recv_eval_mutex_.lock(); }
    void UnlockTransmitEval() { recv_eval_mutex_.unlock(); }

 private:
    int recv_timeout_ms_;
    ZmqRecvProp zmq_recv_props_;
    std::mutex recv_eval_mutex_;
    bool CheckEvent(int timeout_ms = 1000);
    void HostLivelinessCheck(const zmq::message_t& msg);

    std::unique_ptr<zmq::context_t> zmq_ctx_ = nullptr;
    std::unique_ptr<zmq::socket_t> user_auth_sock_ = nullptr;
    std::unique_ptr<zmq::socket_t> data_sub_sock_ = nullptr;
    std::unique_ptr<zmq::socket_t> control_pub_sock_ = nullptr;
    std::unique_ptr<base::common::TcpFrameServer> tcp_ser_ = nullptr;

    std::thread connect_manage_thread_;
    bool data_connected_ = false;
    uint32_t curr_connection_ = 0;
    std::atomic_bool is_running_{false};
    std::unordered_map<HostInfoRequest, uint64_t, HostInfoRequestHash>
        online_hosts_;
    bool exclusive_ = false;

    std::unordered_map<std::string, std::string> uuid_topic_mapping_;
    RegTopicDescriptorHandler reg_handler_;
    MsgDataHandler data_handler_;
    UnknownUuidHandler unknown_uuid_handler_;

    uint64_t control_sent_cnt_ = 0;
    uint64_t data_recv_cnt_ = 0;
    uint64_t reg_recv_cnt_ = 0;
};
}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
