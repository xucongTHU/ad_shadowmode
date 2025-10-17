/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>

#include "ad_rscl/comm/cross_system/v2/common.h"
#include "ad_base/common/socket.h"
#include "ad_rscl/comm/types.h"

namespace senseAD {
namespace rscl {
namespace comm_cs {

using senseAD::base::common::ConnectionId;

enum class PeerStatus { PENDING, CONNECT_INIT, CONNECTED, REJECTED };
using ReconnectFunc = std::function<void()>;
class TcpPeerConnectionManager {
 public:
    TcpPeerConnectionManager(const std::vector<AddrInfo>& peer_addrs,
                             const AddrInfo& host_info);
    ~TcpPeerConnectionManager();

    void EnableConnection();
    void DisableConnection();

    void RegReconnectFunc(ReconnectFunc func);

    std::shared_ptr<base::common::TcpFrameClient> TryGetPeerCli(
        const key_t peer_key);

    /**
     * @brief Notify the manager about Peer have Connected in Server side
     *
     * @param peer_key
     * @param cid
     * @return bool whether it is a new session
     */
    bool SetPeerSerConnect(const key_t peer_key, const ConnectionId& cid);
    /**
     * @brief  Notify the manager about Peer have Disconnected in Server side
     *
     * @param cid
     * @return key_t peer_key
     */
    key_t SetPeerSerDisconnect(const ConnectionId& cid);

    bool SendSyncReqToPeer(const key_t& peer_key);
    bool SendSyncToPeer(const key_t& peer_key, const RuntimeState& host_state);
    bool SendSyncToPeer(const key_t& peer_key,
                        const RuntimeState& host_state,
                        base::common::TcpFrameClient* cli);
    bool SendSyncToAllPeer(const RuntimeState& host_state);
    bool SendDataToPeer(const key_t& peer_key,
                        const TopicInfo& info,
                        const senseAD::rscl::comm::BorrowedRawMessage& msg,
                        size_t* sent_size = nullptr,
                        bool non_block = false);

 private:
    void PeerCliManageThread();

    std::mutex cli_mtx_;
    std::vector<AddrInfo> peer_addrs_;
    AddrInfo host_info_;

    bool enabled_ = false;
    std::thread managing_thread_;

    std::unordered_map<ConnectionId, key_t> connection_peer_maps_;
    std::unordered_set<key_t> peers_pool_;
    std::unordered_map<key_t, PeerStatus> peer_status_;
    std::unordered_map<key_t, std::shared_ptr<base::common::TcpFrameClient>>
        peer_clis_;
    std::unordered_map<key_t, std::atomic<int>> seq_;
    std::unordered_map<key_t, std::shared_ptr<std::mutex>> send_mtx_;

    ReconnectFunc reconnect_func_ = nullptr;
};

}  // namespace comm_cs
}  // namespace rscl
}  // namespace senseAD
