/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <mutex>
#include <map>
#include <limits>
#include <memory>
#include <atomic>
#include <utility>

#include "concurrency/concurrentqueue.h"
#include "ad_base/std/optional.h"
#include "ad_serde/msgmeta.h"
#include "ad_base/common/thread_pool.h"

namespace senseAD {
namespace rscl {
namespace comm_cs {

typedef std::string key_t;

struct EndpointConfig {
    uint32_t send_thread_num = 4;
    bool non_blocking_send = false;
    bool send_thread_debug = false;
};

struct StatReportConfig {
    bool enable = false;
    uint32_t sample_interval = 50;
    uint32_t report_interval = 1000;
};

struct AddrInfo {
    std::string name;
    std::string ip;
    uint16_t port;

    // no need to serialize to cereal, just for internal use
    EndpointConfig config;

    // support multi-instance of CrossSystem class.
    inline key_t AsKey() const { return ip + ":" + std::to_string(port); }

    template <class Archive>
    void serialize(Archive& ar) {  // NOLINT
        ar(name, ip, port);
    }
};

struct TopicInfo {
    // a key/topic name used in the service exchange
    key_t topic;
    // the real topic name in host. prefix + host topic = topic
    key_t host_topic_name;
    std::string type_name;
    std::string descriptor;

    template <class Archive>
    void serialize(Archive& ar) {  // NOLINT
        ar(topic, host_topic_name, type_name, descriptor);
    }

    std::string DebugString() const;
};

struct RuntimeState {
    // host publishing
    std::unordered_map<key_t, TopicInfo> host_pub_topic;

    // host subscribing
    std::unordered_map<key_t, TopicInfo> host_sub_topic;

    template <class Archive>
    void serialize(Archive& ar) {  // NOLINT
        ar(host_pub_topic, host_sub_topic);
    }

    std::string DebugString(bool simple_mode = false) const;
};

using OutgoingTopicPeerMap =
    std::unordered_map<key_t, std::unordered_set<key_t>>;

struct OverallState {
 public:
    RuntimeState GetSelfState() const;

    std::unordered_map<key_t, RuntimeState> GetPartnerState() const;
    void SetPartnerState(const key_t& peer_key, const RuntimeState& val);
    void RemovePartnerState(const key_t& peer_key);

    std::unordered_map<key_t, TopicInfo> GetHostSubTopic() const;
    std::shared_ptr<std::unordered_map<key_t, TopicInfo>>
    GetHostSubTopicCopiedPtr() const;
    void SetHostSubTopic(const key_t& topic_key,
                         const TopicInfo& val,
                         const std::string& debug_msg = "");
    void RemoveHostSubTopic(const key_t& topic_key,
                            const std::string& debug_msg = "");

    void SetHostPubTopicInfo(const key_t& topic_key,
                             const TopicInfo& val,
                             const std::string& debug_msg = "");
    void UpdateHostPubTopicMsgMeta(const key_t& topic_key,
                                   const senseAD::serde::MsgMeta& meta);
    base::optional<TopicInfo> GetHostPubTopicInfo(const key_t& topic_key) const;
    void RemoveHostPubTopic(const key_t& topic_key,
                            const std::string& debug_msg = "");

    void SetOutgoingTopicPeerMap(const OutgoingTopicPeerMap& val);
    void RemovePeerFromOutgoingTopicPeerMap(const key_t& peer_key);
    OutgoingTopicPeerMap GetOutgoingTopicPeerMap() const;
    std::unordered_set<key_t> GetOutgoingTopicPeerMap(const key_t& key) const;

    uint32_t GetTopicPeerMapUpdateSyncCnt(void) const;

 private:
    std::unordered_map<key_t, RuntimeState> partner_state;

    OutgoingTopicPeerMap topic_peer_map;
    uint32_t topic_peer_map_update_sync = 0;
    const uint32_t max_u32_sync_cnt = std::numeric_limits<uint32_t>::max();

    RuntimeState self_state;
    // A Optimization to avoid copy of
    mutable std::shared_ptr<std::unordered_map<key_t, TopicInfo>>
        host_sub_topic_copied_ptr_{nullptr};

    mutable std::mutex mtx;
};

using SendDataSeqFunc = std::pair<uint64_t, std::function<void()>>;

struct SubDataBucket {
    explicit SubDataBucket(const std::string& ptopic)
        : topic(ptopic), invoke_cnt(0) {}

    moodycamel::ConcurrentQueue<SendDataSeqFunc> data_queue;
    std::atomic<bool> lock_status{false};
    std::string topic;
    uint64_t invoke_cnt;

    std::string DebugString() const;
};

class BucketGroup {
 public:
    explicit BucketGroup(const std::string& peer_ip)
        : target_peer_ip_(peer_ip) {}

    BucketGroup(BucketGroup&& other) noexcept
        : target_peer_ip_(std::move(other.target_peer_ip_)) {}

    void InitSendThreadPool(int32_t thread_num, int32_t task_num);
    void SwitchThreaPoolDebug(bool on);

    void PushSendTask(const std::string& topic, SendDataSeqFunc&& fn);

    std::string DebugString() const;

 private:
    void InvokeSendThread(const std::string& topic);

    std::shared_ptr<SubDataBucket> GetTargetBucketSafe(
        const std::string& topic);

    std::string target_peer_ip_;
    std::mutex bucket_mtx_;
    std::unordered_map<std::string, std::shared_ptr<SubDataBucket>> bucket_map_;
    std::unique_ptr<base::common::ThreadPool> send_pool_;
};

}  // namespace comm_cs
}  // namespace rscl
}  // namespace senseAD
