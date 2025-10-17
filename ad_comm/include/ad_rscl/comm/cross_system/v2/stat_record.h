/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <map>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>
#include <memory>

#include "ad_rscl/comm/cross_system/v2/protocol.h"
#include "ad_rscl/comm/types.h"
#include "ad_rscl/comm/node.h"
#include "ad_rscl/comm/publisher_base.h"
#include "ad_rscl/task/task.h"
#include "ad_rscl/idl/cross_system_stat.capnp.h"

namespace senseAD {
namespace rscl {
namespace comm_cs {

#define kCrossSystemStatChannel "/stat/cross_system_perform"

struct StatNetworkUsage {
    uint64_t total_byte_part = 0;
    uint64_t total_kbyte_part = 0;

    double KB() const;
    void AddByte(const uint64_t byte);
    StatNetworkUsage operator+(const StatNetworkUsage& other) const;
    StatNetworkUsage& operator+=(const StatNetworkUsage& other) &;
};

struct StatReportInfo {
    double received_total_traffic = 0;
    double sent_total_traffic = 0;
    uint64_t received_total_cnt = 0;
    uint64_t sent_total_cnt = 0;
    uint64_t lost_total_cnt = 0;
    uint64_t max_latency = 0;
};

struct StatRecord {
    uint64_t first_unix_ns = 0;
    uint64_t last_update_unix_ns = 0;

    uint64_t received_total_msg_cnt = 0;
    uint64_t sent_total_msg_cnt = 0;

    std::map<std::string, uint64_t> received_topic_msg_cnt;
    std::map<std::string, uint64_t> sent_topic_msg_cnt;

    // { key: topic + send unique node id
    std::unordered_map<std::string, uint64_t> sent_topic_msg_last_seq;
    std::map<std::string, uint64_t> frame_lost_topic_msg_cnt;
    // }

    std::map<std::string, uint64_t> received_peer_packet_lost;
    std::map<std::string, uint64_t> received_peer_packet_last_seq;

    uint64_t received_peer_topic_lost_total = 0;
    std::map<std::string, uint64_t> received_peer_topic_lost;
    std::map<std::string, uint64_t> received_peer_topic_last_seq;

    uint64_t max_latency = 0;

    std::unordered_map<std::string, StatNetworkUsage> received_peer_traffic;
    std::unordered_map<std::string, StatNetworkUsage> received_topic_traffic;
    std::unordered_map<std::string, StatNetworkUsage> sent_peer_traffic;
    std::unordered_map<std::string, StatNetworkUsage> sent_topic_traffic;

    std::map<std::string, StatNetworkUsage> published_topic_traffic;

    std::string state_snapshot_str;
    std::string data_enqueue_info;

    StatRecord operator+(const StatRecord& other) const;
    StatRecord& operator+=(const StatRecord& other) &;

    void IncreReceivedPeerTraffic(const std::string& peer_key,
                                  const uint64_t received_byte,
                                  const uint64_t packet_seq,
                                  const RootMessage& debug_m);
    void IncreReceivedTopicTraffic(const std::string& topic,
                                   const uint64_t received_byte,
                                   const std::string& source_node_id = "",
                                   const uint64_t source_seq = 0,
                                   const uint64_t source_stamp = 0);

    void IncreSentPeerTraffic(const std::string& peer_key,
                              const uint64_t received_byte);
    void IncreSentTopicTraffic(const std::string& topic,
                               const uint64_t received_byte,
                               const std::string& source_node_id = "",
                               const uint64_t source_seq = 0);
    void IncrePublishedTopicTraffic(const std::string& topic,
                                    const uint64_t received_byte);
    void SetStateSnapshotStr(const std::string& str);
    void SetDataEnqueueInfo(const std::string& str);
    void TriggerLatencySample(void);
    void GetStatReportInfo(StatReportInfo* p_stat_info);

    std::string DebugString() const;
    bool sampling_status{false};
};

class StatRecordMonitor {
 public:
    StatRecordMonitor() = default;
    ~StatRecordMonitor() = default;

    inline StatRecord GetTotalHistoryRecord() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return total_record_;
    }

    void Init();

    void IncreReceivedPeerTraffic(const std::string& peer_key,
                                  const uint64_t received_byte,
                                  const uint64_t packet_seq,
                                  const RootMessage& debug_m);
    void IncreReceivedTopicTraffic(const std::string& topic,
                                   const uint64_t received_byte,
                                   const std::string& source_node_id = "",
                                   const uint64_t source_seq = 0,
                                   const uint64_t source_stamp = 0);

    void IncreSentPeerTraffic(const std::string& peer_key,
                              const uint64_t received_byte);
    void IncreSentTopicTraffic(const std::string& topic,
                               const uint64_t received_byte,
                               const std::string& source_node_id = "",
                               const uint64_t source_seq = 0);
    void IncrePublishedTopicTraffic(const std::string& topic,
                                    const uint64_t received_byte);
    void SetStateSnapshotStr(const std::string& str);
    void SetDataEnqueueInfo(const std::string& str);
    void TriggerLatencySample(void);
    void GetStatReportInfo(StatReportInfo* p_stat_info);

 private:
    StatRecord total_record_;

    mutable std::mutex mtx_;
};

class StatRecordReporter {
 public:
    explicit StatRecordReporter(rscl::Node* node,
                                uint32_t report_interval,
                                uint32_t sample_interval,
                                std::shared_ptr<StatRecordMonitor> stat_record);

    StatRecordReporter(const StatRecordReporter&) = delete;
    StatRecordReporter& operator=(const StatRecordReporter&) = delete;

    void TriggerLatencySample(void);
    void ReportStatInfo(void);

 private:
    std::shared_ptr<StatRecordMonitor> stat_record_;
    senseAD::rscl::comm::PublisherBase::Ptr cs_stat_pub_;
    std::unique_ptr<senseAD::rscl::task::SimpleTimerTaskImpl> periodic_task_;
};

}  // namespace comm_cs
}  // namespace rscl
}  // namespace senseAD
