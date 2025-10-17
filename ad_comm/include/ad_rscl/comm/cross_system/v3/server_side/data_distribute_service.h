/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "ad_rscl/comm/node.h"
#include "ad_rscl/comm/cross_system/v3/core/common.h"
#include "ad_rscl/comm/cross_system/v3/core/system_state.h"
#include "ad_rscl/comm/cross_system/v3/core/protocol.h"
#include "ad_rscl/comm/cross_system/v3/core/stat_record.h"
#include "ad_base/common/socket.h"
#include "ad_base/common/thread_pool.h"

namespace senseAD {
namespace rscl {
namespace comm_cs3 {
using senseAD::base::common::ConnectionId;

struct DistributedStatPerTopic {
    TopicID local_topic_id;
    uint64_t last_topic_seq{0};

    uint64_t received_topic_msg_cnt{0};
    uint64_t received_topic_traffic_bytes{0};
    uint64_t received_topic_lost{0};
};

struct DistributedStatPerConnection {
    PeerID local_peer_id;
    uint64_t last_packet_seq{0};

    uint64_t received_peer_packet_cnt{0};
    uint64_t received_peer_traffic_bytes{0};
    uint64_t received_peer_packet_lost{0};
    uint64_t received_peer_max_latency{0};

    std::mutex mtx;

    std::unordered_map<TopicID, DistributedStatPerTopic> per_topic_stat;
    std::mutex per_topic_mtx;

    inline void Clear() {
        received_peer_packet_cnt = 0;
        received_peer_traffic_bytes = 0;
        received_peer_packet_lost = 0;
        received_peer_max_latency = 0;
        per_topic_stat.clear();
    }
};

class DataDistributeService {
 public:
    explicit DataDistributeService(const DataDistributeServiceConfig& config);
    virtual ~DataDistributeService();

    void Enable();
    void Shutdown();

    inline std::shared_ptr<StatRecord> GetSummarizedStatPtr() {
        return summarized_stat_;
    }

 private:
    // * ====== Tcp Message Handling ======
    void RecvLoop();
    void HandleMsg(std::shared_ptr<base::common::FrameMsg> raw_frame);

    void HandleReg(const base::common::FrameMsg& raw_frame,
                   const RootMessageHeader& packet_header,
                   const TopicRegMessage& reg_message);
    void BoardcastReReg();

    bool IsTopicAllowedCrossSystem(const std::string& topic) const;
    void HandleData(const std::shared_ptr<base::common::FrameMsg>& raw_frame,
                    const RootMessageHeader& packet_header,
                    const SendMessage& message_header,
                    const std::vector<MsgData>& datas,
                    const std::shared_ptr<rscl::comm::PublisherBase>& publisher,
                    std::shared_ptr<DistributedStatPerConnection> stat_record);

    void HandleDisconnectedCli(ConnectionId conn_id);

    std::shared_ptr<base::common::FrameMsg> GetFrameFromPool();
    void ReturnFrameToPool(
        const std::shared_ptr<base::common::FrameMsg>& frame);

    // * ====== Stat Updating ======
    void UpdatePeerStat(
        const base::common::FrameMsg& raw_frame,
        const RootMessageHeader& packet_header,
        std::shared_ptr<DistributedStatPerConnection> stat_record = nullptr);
    void SummaryStatAndPrint();

 private:
    virtual void PublishData(const MsgData& data,
                             rscl::comm::PublisherBase* pub);

    DataDistributeServiceConfig config_;
    bool enabled_{false};

    std::thread recv_thread_;
    std::unique_ptr<base::common::TcpFrameServer> host_ser_;
    const int kNMsgHandleTp{2};
    std::vector<std::shared_ptr<base::common::ThreadPool>> msg_handle_tp_;

    std::mutex frame_pools_mtx_;
    std::queue<std::shared_ptr<base::common::FrameMsg>> frame_pools_;

    // 1 TopicID = unique topic + msg type pair
    std::mutex per_conn_mtx_;
    std::unordered_map<ConnectionId,
                       std::shared_ptr<DistributedStatPerConnection>>
        distributed_stat_;
    std::shared_ptr<StatRecord> summarized_stat_;

    std::unique_ptr<senseAD::rscl::Node> distribute_node_;
    std::mutex pubs_map_mtx_;
    std::unordered_map<
        ConnectionId,
        std::unordered_map<TopicID, std::shared_ptr<rscl::comm::PublisherBase>>>
        distribute_pubs_;
};

}  // namespace comm_cs3
}  // namespace rscl
}  // namespace senseAD
