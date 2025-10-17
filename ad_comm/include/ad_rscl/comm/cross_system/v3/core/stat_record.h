/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <map>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>
#include <memory>

#include "ad_base/common/socket.h"
#include "ad_rscl/comm/cross_system/v3/core/protocol.h"
#include "ad_rscl/comm/types.h"
#include "ad_rscl/comm/node.h"
#include "ad_rscl/comm/publisher_base.h"
#include "ad_rscl/task/task.h"
#include "ad_rscl/idl/cross_system_stat.capnp.h"

namespace senseAD {
namespace rscl {
namespace comm_cs3 {

using senseAD::base::common::ConnectionId;
class StatRecord;

//* =============================================================
//*   For Report
//* =============================================================

struct DistributeServiceStatReportInfo {
    // KB
    double received_total_traffic = 0;
    uint64_t received_total_cnt = 0;
    uint64_t lost_total_cnt = 0;
    uint64_t max_received_latency = 0;
};

struct OutgoingTrafficInfo {
    // KB
    double sent_total_traffic = 0;
    uint64_t sent_total_cnt = 0;
    uint64_t dropped_total_cnt = 0;
};

struct StatReportInfo {
    DistributeServiceStatReportInfo distri_serv_info;
    OutgoingTrafficInfo outgoing_traffic_info;

    std::string DebugString() const;
};

class StatRecordReporter {
 public:
    explicit StatRecordReporter(
        rscl::Node* node,
        uint32_t report_interval,
        std::vector<std::shared_ptr<StatRecord>> stat_records);
    virtual ~StatRecordReporter();

    StatRecordReporter(const StatRecordReporter&) = delete;
    StatRecordReporter& operator=(const StatRecordReporter&) = delete;

    void ReportStatInfo();

 private:
    std::vector<std::shared_ptr<StatRecord>> stat_records_;
    senseAD::rscl::comm::PublisherBase::Ptr cs_stat_pub_;
    std::unique_ptr<senseAD::rscl::task::SimpleTimerTaskImpl> periodic_task_;

    StatReportInfo last_stat_info_;
};

//* =============================================================
//*   The Actual Statistic Part
//* =============================================================

struct StatNetworkUsage {
    uint64_t total_byte_part = 0;
    uint64_t total_kbyte_part = 0;

    inline double KB() const {
        return total_kbyte_part + total_byte_part / 1024.0;
    }
    inline uint64_t Byte() const {
        return total_kbyte_part * 1024 + total_byte_part;
    }
    inline void AddByte(const uint64_t byte) {
        total_byte_part += byte;
        total_kbyte_part += static_cast<uint64_t>(total_byte_part / 1024);
        total_byte_part %= 1024;
    }
    inline void AddKB(const uint64_t kb) { total_kbyte_part += kb; }

    inline void Clear() {
        total_byte_part = 0;
        total_kbyte_part = 0;
    }
    StatNetworkUsage operator+(const StatNetworkUsage& other) const;
    StatNetworkUsage& operator+=(const StatNetworkUsage& other) &;
};

//* =============================================================
//*   Brief stat info for Rscl Graph tool
//* =============================================================
struct TrafficStatRecord {
    uint64_t last_update_unix_ns = 0;

    std::map<std::string, uint64_t> received_topic_msg_cnt;
    std::map<std::string, uint64_t> sent_topic_msg_cnt;
    std::unordered_map<std::string, StatNetworkUsage> received_peer_traffic;
    std::unordered_map<std::string, StatNetworkUsage> received_topic_traffic;
    std::unordered_map<std::string, StatNetworkUsage> sent_peer_traffic;
    std::unordered_map<std::string, StatNetworkUsage> sent_topic_traffic;
};
//* =============================================================

struct StatRecordPerTopic {
    //* Received Msg Stat
    uint64_t received_topic_msg_cnt{0};
    StatNetworkUsage received_topic_traffic;
    uint64_t received_topic_lost{0};

    //* Sent Msg Stat
    uint64_t sent_topic_msg_cnt{0};
    uint64_t sent_topic_dropped{0};
    StatNetworkUsage sent_topic_traffic;

    std::mutex sub_mtx;
};

struct StatRecordPerPeer {
    //* Received Msg Stat
    uint64_t received_peer_packet_lost{0};
    StatNetworkUsage received_peer_traffic;
    uint64_t received_peer_max_latency{0};

    //* Sent Msg Stat
    StatNetworkUsage sent_peer_traffic;

    std::mutex sub_mtx;
};

class StatRecord {
 public:
    StatRecord();
    StatRecord(PeerIDMap* peer_id_map, TopicIDMap* topic_id_map);
    ~StatRecord();

    //! Assumed now_ns will be strictly incremental

    //* Received Msg Stat
    //* Expected Reported By: Data Distribute Service
    void IncreReceivedPeerTraffic(const uint64_t now_ns,
                                  const PeerID& peer_key,
                                  const uint64_t new_received_cnt,
                                  const uint64_t new_received_bytes,
                                  const uint64_t new_lost,
                                  const uint64_t latest_latency);
    void IncreReceivedTopicTraffic(const uint64_t now_ns,
                                   const TopicID& topic,
                                   const uint64_t new_received_cnt,
                                   const uint64_t new_received_bytes,
                                   const uint64_t new_lost);

    //* Sent Msg Stat
    //* Expected Reported By: RemoteEpStore
    void IncreSentTraffic(const uint64_t now_ns,
                          const PeerID& peer_key,
                          const TopicID& topic,
                          const uint64_t new_sent_cnt,
                          const uint64_t new_dropped,
                          const uint64_t new_sent_bytes);

    uint64_t GetMaxAndResetReceivedLatency();

    //* Topo Stat
    void SetStateSnapshotStr(const std::string& str);
    //* Inner Debug Stat
    void SetDataEnqueueInfo(const std::string& str);

    void AddToStatReportInfo(StatReportInfo* p_stat_info);

    // For Rscl Graph Tool
    void AddToTrafficStatRecord(TrafficStatRecord* p_traffic_stat_record);

    void LogPrint() const;
    std::string DebugString() const;

    void To(senseAD::rscl::cs3::idl::StatReport::Builder* builder) const;
    void Reset();
    void ResetAndReadFromIdlReports(
        const std::vector<senseAD::rscl::cs3::idl::StatReport::Reader>&
            stat_reports);

    inline PeerIDMap* peer_id_map() { return peer_id_map_; }
    inline TopicIDMap* topic_id_map() { return topic_id_map_; }

 public:
    std::atomic<uint64_t> first_unix_ns{0};
    std::atomic<uint64_t> last_update_unix_ns{0};

    //* Overall Part
    //* - Received Msg Stat
    std::atomic<uint64_t> received_total_msg_cnt{0};
    std::atomic<uint64_t> received_total_lost{0};

    //* - Sent Msg Stat
    std::atomic<uint64_t> sent_total_msg_cnt{0};
    std::atomic<uint64_t> sent_total_dropped{0};

    //* PerPeer Part
    std::unordered_map<PeerID, std::unique_ptr<StatRecordPerPeer>>
        per_peer_stat{};
    //* PerTopic Part
    std::unordered_map<TopicID, std::unique_ptr<StatRecordPerTopic>>
        per_topic_stat{};

    //* Extra Part
    std::string state_snapshot_str{};
    std::string data_enqueue_info{};

 private:
    bool have_provided_id_map_{true};
    mutable std::mutex topic_mtx_;
    mutable std::mutex peer_mtx_;

    PeerIDMap* peer_id_map_{nullptr};
    TopicIDMap* topic_id_map_{nullptr};
};

}  // namespace comm_cs3
}  // namespace rscl
}  // namespace senseAD
