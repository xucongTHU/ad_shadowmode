/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <atomic>
#include <fstream>
#include <queue>
#include <map>
#include <utility>

#include "ad_rscl/comm/cross_system/v2/cross_system_service_v2.h"
#include "ad_rscl/comm/cross_system/v2/protocol.h"
#include "ad_rscl/comm/cross_system/v2/tcp_peer.h"
#include "ad_service_discovery/service_discovery/service_discovery.h"
#include "ad_base/common/socket.h"
#include "ad_base/std/optional.h"
#include "ad_base/common/thread_pool.h"
#include "ad_rscl/comm/cross_system/v2/stat_record.h"

namespace senseAD {
namespace rscl {
namespace comm_cs {

using ListenerToken =
    senseAD::service_discovery::ServiceDiscovery::ListenerToken;

class CrossSystemServiceV2TcpImpl : public CrossSystemServiceV2Impl {
 public:
    CrossSystemServiceV2TcpImpl();
    ~CrossSystemServiceV2TcpImpl() override;

    void ShutdownImpl() override;
    bool Init(rscl::Node *node, const CrossSystemServiceV2Param param) override;

    void Reset();
    bool Start() override;
    bool Stop() override;

    StatRecord GetStatistics() const override;

    const OverallState *GetOverallStatePtr() const override;
    CrossSystemServiceV2Param param() const override;

    // For UNITTEST DEBUG
    std::unordered_set<key_t> GetOwnedRsclPublisherNames() const;
    std::unordered_set<key_t> GetOwnedRsclSubscriberNames() const;

    /**
     * @brief
     * Current service discovery change listener is not that reliable.
     * Need a regular sync to remove the unused subscriber, publisher
     */
    void TryRemoveUnusedHostPubSub();

 private:
    bool HandlingIncorrectHostSubCount(
        const std::unordered_set<key_t> &latest_hostsub_cnt,
        const std::unordered_map<key_t, std::vector<TopicInfo>>
            &latest_hostsub_topicinfo);
    bool HandlingIncorrectHostPubCount(
        const std::unordered_set<key_t> &latest_hostpub_cnt,
        const std::unordered_map<key_t, std::vector<TopicInfo>>
            &latest_hostpub_topicinfo);

    void ClearPeerOldHandles(const key_t peer_key);

    void RegHostMachineTopicChangeHandler();

    // Affect incoming
    void HandleUnsubFromHost(const key_t &topic_key,
                             const std::string &node_name);
    void HandleSubFromHost(const key_t &topic_key, const TopicInfo &info);
    void HandleReconnect();

    // Affect outgoing
    void HandleUnpubFromHost(const key_t &topic_key,
                             const std::string &node_name);
    void HandlePubFromHost(const key_t &topic_key, const TopicInfo &info);
    void CreateSubIfNeeded(const OutgoingTopicPeerMap &latest_map,
                           const key_t &topic_key);
    void RemoveSubIfNoLongerNeeded(const OutgoingTopicPeerMap &latest_map,
                                   const key_t &topic_key,
                                   const int hostub_cnt);

    void HandlePeerSync(const RootMessage &m, const ConnectionId &cid);
    void HandlePeerData(const RootMessage &m, const char *data_ptr);

    OutgoingTopicPeerMap UpdateBiChannelMatch(const base::optional<key_t> k);

    void HandleOnConnection(
        const ConnectionId &cid,
        const base::common::TcpFrameServer::ConnectionChange &c);
    void ServingHandler();

    key_t ToHostPubKey(const std::string &peer_key) const;
    bool IsExcludedTopic(const std::string &topic_name) const;
    bool IsIncomingExcludedTopic(const std::string &topic_name) const;
    std::string GetOutgoingTopicName(const std::string &host_topic_name) const;

    uint32_t GetTopicPeerMapSyncStore(uint16_t idx);
    void SetTopicPeerMapStore(uint16_t idx,
                              uint32_t sync_cnt,
                              std::unordered_set<key_t> target_peers);
    std::shared_ptr<std::unordered_set<key_t>> GetTopicPeerMapTargetStore(
        uint16_t idx);
    void SubDataEnqueue(
        const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>
            &msg,
        uint16_t sync_idx,
        const std::string &topic_key_cpy,
        const senseAD::rscl::comm_cs::TopicInfo &info_cpy);

 private:
    rscl::Node *node_ = nullptr;
    std::string node_name_;
    CrossSystemServiceV2Param param_;
    OverallState state_;

    // For serving incoming action and data
    bool running_ = false;
    std::unique_ptr<base::common::TcpFrameServer> host_ser_;
    std::thread serving_thread_;

    std::unique_ptr<base::common::ThreadPool> sync_thread_pool_;
    const int kNDataThreadPool = 10;
    std::vector<std::unique_ptr<base::common::ThreadPool>> data_thread_pools_;

    mutable std::mutex tohost_pub_mtx_;
    /**
     * @brief publisher ptr
     * key: fake peer key, value: topic & info pair
     * Updater: Peer SYNC request or Peer Quit Connection
     */
    std::unordered_map<
        key_t,
        std::unordered_map<key_t, std::shared_ptr<rscl::comm::PublisherBase>>>
        tohost_pubs_;
    /**
     * @brief in case the not support multiple publisher
     * key: topic, value: real peer keys
     * Updater: Peer SYNC request or Peer Quit Connection
     */
    std::unordered_map<key_t, std::unordered_set<key_t>> tohost_pubs_ref_;
    /**
     * @brief peer publisher topic info
     * key: real peer key, value: topic & info pair
     * Updater: Peer SYNC request or Peer Quit Connection
     */
    std::unordered_map<key_t, std::unordered_map<key_t, TopicInfo>>
        tohost_pubs_info_;

    mutable std::mutex fromhost_sub_mtx_;
    /**
     * @brief subscriber ptr
     * key: topic, value: subscriber ptr
     * Updater: HostMachineTopicChangeHandler
     */
    std::unordered_map<key_t, std::shared_ptr<rscl::comm::SubscriberBase>>
        fromhost_subs_;

    // Peers
    std::unique_ptr<TcpPeerConnectionManager> peer_conn_mgr_;

    std::vector<ListenerToken> sd_tokens_;
    std::unique_ptr<base::common::ThreadPool> sd_reg_thread_pool_;

    // Record host service count
    std::mutex hostsub_cnt_mtx_;
    std::unordered_map<key_t, int> hostsub_cnt_;
    std::unordered_map<key_t, int> hostsub_gc_checkcnt_;
    std::unordered_set<key_t> pre_missing_hostsub_;

    std::mutex hostpub_cnt_mtx_;
    std::unordered_map<key_t, int> hostpub_cnt_;
    std::unordered_map<key_t, int> hostpub_gc_checkcnt_;
    std::unordered_set<key_t> pre_missing_hostpub_;

    const int kGcDisposeThreshold = 4;
    std::unique_ptr<senseAD::rscl::task::SimpleTimerTaskImpl> periodic_task_;
    std::atomic<bool> need_resync_{false};

    std::shared_ptr<StatRecordMonitor> stat_record_;
    std::unique_ptr<StatRecordReporter> stat_report_;

    // Must subscribe topic
    const std::unordered_set<std::string> must_subscribe_topics_ = {
        senseAD::rscl::kSimClockChannel};

    std::shared_ptr<senseAD::service_discovery::SDServiceClient>
        sd_client_sptr_ =
            senseAD::service_discovery::SDServiceClient::SharedInstance();

    std::mutex recv_frame_buffer_mtx_;
    std::queue<std::shared_ptr<base::common::FrameMsg>> recv_frame_buffer_;
    std::mutex seq_mtx_;
    std::unordered_map<std::string, uint64_t> seq_map_;
    std::unordered_map<key_t, comm_cs::BucketGroup> bucket_groups_;

    std::atomic<uint16_t> topic_peer_map_sync_idx_{0};

    std::mutex topic_peer_sync_mtx_;
    /**
     * @brief sync flag indexed by topic_peer_map_sync_idx_ in each subscriber
     * key: idx maintained by each subscriber, value: topic peer map sync flag
     * Updater: Subscriber callback when sync flag update
     */
    std::unordered_map<uint16_t, uint32_t> topic_peer_map_sync_store_;

    std::mutex topic_peer_mtx_;
    /**
     * @brief peer map indexed by topic_peer_map_sync_idx_ in each subscriber
     * key: idx maintained by each subscriber, value: topic peer map
     * Updater: Subscriber callback when sync flag update
     */
    std::unordered_map<uint16_t, std::shared_ptr<std::unordered_set<key_t>>>
        topic_peer_map_target_store_;

    std::unordered_map<std::string, uint64_t> topics_tp_;
};

}  // namespace comm_cs
}  // namespace rscl
}  // namespace senseAD
