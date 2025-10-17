/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "ad_rscl/comm/cross_system/v3/core/common.h"
#include "ad_rscl/comm/cross_system/v3/core/protocol.h"
#include "ad_rscl/comm/cross_system/v3/core/stat_record.h"
#include "ad_rscl/comm/cross_system/v3/swc_side/remote_ep_client.h"
#include "ad_rscl/comm/inet_impl/inet_client.h"
#include "ad_rscl/task/task.h"
#include "ad_base/common/thread_pool.h"

namespace senseAD {
namespace rscl {
namespace comm_cs3 {

struct CrossSystemSwcConfig {
    bool separate_send_thread{true};
    // Regex supported
    std::set<std::string> excluded_topics;
    // Regex supported
    std::set<std::string> whitelist_topics;

    AddrInfo query_service_host_addr;
};

struct UniqueTopic {
    std::string topic_name;
    std::string msg_type;
};

/**
 * @brief Integrate all cross system swc logic
 * - 1. Auto Query From Core to get remote endpoint
 *  - on new publisher
 *  - periodicly
 * - 2. Create Remote Endpoint and TcpClients
 *  - after get the latest remote endpoints
 * - 3. Upsert self state to core
 *  - periodicly
 *
 */
class RemoteEndpointStore : public ::Shutdownable {
 public:
    explicit RemoteEndpointStore(const CrossSystemSwcConfig& config);
    virtual ~RemoteEndpointStore();

    // Call by Publisher Create Point
    std::shared_ptr<RemoteEndpoints> CreateRemoteEndpoints(
        const std::string& topic, const senseAD::serde::MsgMeta& msg_meta);

    inline size_t RemoteCount() const { return remote_clients_.size(); }

 private:
    void Init();
    void ShutdownImpl() override;

    void PollingUpdate();
    bool CheckCoreIsRestarted(const uint32_t core_pid,
                              const uint64_t core_start_time);

    void QueryRemoteState(const uint32_t query_timeout_ms = 1000);
    bool UpsertLocalState();
    void UpdateEndpointsFromQueryResult(
        const std::vector<TopicInfo>& remote_sub_topic_infos);

    key_t TopicAsKey(const std::string& topic_name,
                     const std::string& msg_type) const;
    bool IsTopicAllowedCrossSystem(const std::string& topic) const;
    bool UpdateCurrentProcessRuntimeStates(std::string ensure_exist_pub = "");
    void RemoveUselessEndpoints();
    void ReRegEndpointsOnRequested();

    void GatherTrafficStatAndReport();

    CrossSystemSwcConfig config_;
    bool running_{false};
    TopicIDMap topic_id_map_;
    PeerIDMap peer_id_map_;

 protected:
    //* ====================== For Endpoint Maintains ======================
    std::mutex query_mtx_;
    comm::InetClient<SwcCliToCoreReq, SwcCliToCoreResp>::Ptr query_cli_;
    std::unique_ptr<senseAD::rscl::task::SimpleTimerTaskImpl> periodic_task_{
        nullptr};
    uint64_t latest_query_state_version_{0};
    uint32_t core_pid_{0};
    uint64_t core_start_time_{0};

    std::mutex upsert_mtx_;
    bool last_upsert_success_{true};

    //* ====================== For Endpoint Topic List ======================
    std::weak_ptr<senseAD::rscl::comm::CommInstanceCount> comm_count_;
    uint64_t last_query_comm_count_vers_{0};
    std::mutex state_mtx_;
    RuntimeState cur_process_runtime_state_;

    //* ====================== For Data Transmit ======================
    std::mutex endpoints_mtx_;
    //! 1 unique topic+msg type get 1 endpoints
    //! Assumed will not have 3 publisher with same msg_type, but different
    //! descriptor
    std::unordered_map<key_t, std::shared_ptr<RemoteEndpoints>>
        topic_endpoints_;
    std::shared_ptr<senseAD::base::common::ThreadPool> execute_pool_;

    // All RemoteClient for 1 peer share the same tcp client
    std::unordered_map<key_t, std::shared_ptr<PeerTcpClient>> remote_clients_;

    //* ====================== Statistic ======================
    std::unique_ptr<StatRecord> stat_to_report_;
    uint64_t report_trigger_n_{0};
};

}  // namespace comm_cs3
}  // namespace rscl
}  // namespace senseAD
