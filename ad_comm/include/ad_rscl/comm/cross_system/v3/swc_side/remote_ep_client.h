/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "ad_rscl/comm/cross_system/v3/core/common.h"
#include "ad_rscl/comm/cross_system/v3/core/protocol.h"
#include "ad_rscl/comm/cross_system/v3/core/stat_record.h"

namespace senseAD {
namespace rscl {
namespace comm_cs3 {

struct PeerTcpClient {
    PeerTcpClient(const std::string& ip,
                  const uint16_t port,
                  const PeerID _peer_id = 0);
    bool IsValid() const;
    bool CheckNeedReg();

    std::mutex mtx;
    PeerID peer_id;
    std::string name;
    uint64_t peer_seq{0};
    std::unique_ptr<senseAD::base::common::TcpFrameClient> cli{nullptr};
};

struct CachedSentTrafficStat {
    uint64_t new_sent_cnt{0};
    uint64_t new_dropped{0};
    uint64_t new_sent_bytes{0};
    inline void Clear() {
        new_sent_cnt = 0;
        new_dropped = 0;
        new_sent_bytes = 0;
    }
};

// When remote topo up, RemoteEndpoints update internal state
class RemoteEndpoints : public std::enable_shared_from_this<RemoteEndpoints> {
 public:
    RemoteEndpoints(const TopicID topic_id,
                    const std::string& topic_name,
                    const senseAD::serde::MsgMeta& msg_meta,
                    const std::shared_ptr<senseAD::base::common::ThreadPool>&
                        execute_pool = nullptr);
    ~RemoteEndpoints();

    void Enable();
    void Disable();

    bool SendReg();
    // Sync only api
    void SendTopicData(const char* serialized_data_ptr, const size_t data_size);
    // sync & async api. depend on config
    void SendTopicData(
        const std::shared_ptr<senseAD::mw::BorrowedRawMessage>& msg);

    void UpdateEndpoints(
        const std::vector<std::shared_ptr<PeerTcpClient>>& dsts);

    inline TopicID GetTopicID() const { return topic_id_; }
    inline std::string GetTopicName() const { return topic_name_; }

    void SwapCachedStats(
        std::unordered_map<PeerID, CachedSentTrafficStat>* out);

    inline std::vector<std::string> ServerAddresses() {
        std::lock_guard<std::mutex> lg(update_mtx_);
        std::vector<std::string> out;
        for (const auto& d : dsts_) {
            out.emplace_back(d->cli->ServerAddress());
        }
        return out;
    }

 private:
    bool enabled_{false};
    std::weak_ptr<RemoteEndpoints> weak_this_;

    std::mutex send_mtx_;
    std::mutex update_mtx_;

    TopicID topic_id_;
    std::string topic_name_;
    senseAD::serde::MsgMeta msg_meta_;

    RootMessageHeader root_header_;
    SendMessage msg_header_;
    TopicRegMessage reg_msg_;

    std::vector<std::shared_ptr<PeerTcpClient>> dsts_;
    std::shared_ptr<senseAD::base::common::ThreadPool> execute_pool_;

    bool last_reg_success_{false};

    std::unordered_map<PeerID, CachedSentTrafficStat> cached_stats_;
};

}  // namespace comm_cs3
}  // namespace rscl
}  // namespace senseAD
