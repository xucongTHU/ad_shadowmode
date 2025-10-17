/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <string>

#include "ad_rscl/comm/cross_system/v3/core/common.h"
#include "ad_rscl/comm/cross_system/v3/core/system_state.h"
#include "ad_rscl/comm/cross_system/v3/core/protocol.h"
#include "ad_rscl/comm/cross_system/v3/core/stat_record.h"
#include "ad_base/common/socket.h"
#include "ad_base/common/thread_pool.h"
#include "ad_rscl/comm/types.h"
#include "ad_rscl/comm/inet_impl/inet_client.h"
#include "ad_rscl/comm/inet_impl/inet_service.h"
#include "ad_rscl/task/task.h"

namespace senseAD {
namespace rscl {
namespace comm_cs3 {

using senseAD::base::common::ConnectionId;

class RemoteEndpointCoordinatorQueryService;
class RemoteEndpointCoordinatorSummarizationService;

/**
 * @brief Provide topo query and summaization service
 * - Each core own a RemoteEndpointCoordinator to exchange latest topo
 * - Each user node own a RemoteEndpointCoordinator Client to get the overall
 * topo from core
 *
 */
class RemoteEndpointCoordinator {
 public:
    explicit RemoteEndpointCoordinator(const CoordinatorConfig& config);
    virtual ~RemoteEndpointCoordinator();

    virtual void Enable();
    virtual void Shutdown();

    std::string StateDebugString() const;
    std::string StatReportDebugString() const;

    inline std::shared_ptr<StatRecord> GetSummarizedStatPtr() {
        return summarized_stat_;
    }

 private:
    bool enabled_{false};

 protected:
    //======================================
    //* Shared for child
    //======================================
    CoordinatorConfig config_;

    //======================================
    //* All topo info
    //======================================
    SystemLevelState system_state_;

    //======================================
    //* For sync topo between SoCs
    //======================================
    std::unique_ptr<RemoteEndpointCoordinatorSummarizationService>
        summarization_service_;

    //======================================
    //* For serving the user node topo query
    //======================================
    std::unique_ptr<RemoteEndpointCoordinatorQueryService> query_service_;

    //======================================
    //* SWC Stat Info
    //======================================
    mutable std::mutex stats_mtx_;
    std::shared_ptr<StatRecord> summarized_stat_;

    friend class RemoteEndpointCoordinatorQueryService;
    friend class RemoteEndpointCoordinatorSummarizationService;
};

/**
 * @brief Provide topo query detail impl
 * - Summary local Endpoint
 * - Provide Remote Endpoint info to swc
 */
class RemoteEndpointCoordinatorQueryService {
 public:
    explicit RemoteEndpointCoordinatorQueryService(
        RemoteEndpointCoordinator* parent);
    ~RemoteEndpointCoordinatorQueryService();

    void Enable();
    void Shutdown();

 private:
    void ServiceHandle(
        const ConnectionId conn_id,
        const std::shared_ptr<ReceivedMsg<SwcCliToCoreReq>>& req,
        std::shared_ptr<SendMsg<SwcCliToCoreResp>>& resp);  // NOLINT
    void RemoveOutdatedStates();
    void BatchUpdateStats();
    void PrintStat();

    void HandleUpsertState(
        const ConnectionId conn_id,
        const std::shared_ptr<ReceivedMsg<SwcCliToCoreReq>>& req,
        std::shared_ptr<SendMsg<SwcCliToCoreResp>>& resp);  // NOLINT
    void HandleRemoveState(
        const ConnectionId conn_id,
        const std::shared_ptr<ReceivedMsg<SwcCliToCoreReq>>& req,
        std::shared_ptr<SendMsg<SwcCliToCoreResp>>& resp);  // NOLINT
    void HandleQueryRemoteEndpoint(
        const std::shared_ptr<ReceivedMsg<SwcCliToCoreReq>>& req,
        std::shared_ptr<SendMsg<SwcCliToCoreResp>>& resp);  // NOLINT
    void HandleQueryStateVersion(
        const std::shared_ptr<ReceivedMsg<SwcCliToCoreReq>>& req,
        std::shared_ptr<SendMsg<SwcCliToCoreResp>>& resp);  // NOLINT
    void HandleReportStat(
        const ConnectionId conn_id,
        const std::shared_ptr<ReceivedMsg<SwcCliToCoreReq>>& req,
        std::shared_ptr<SendMsg<SwcCliToCoreResp>>& resp);  // NOLINT

    void FillSelfStateReqResp(SendMsg<SwcCliToCoreResp>* msg) const;

    RemoteEndpointCoordinator* parent_;
    bool enabled_{false};
    std::mutex mtx_;

    AddrInfo ser_addr_;

    comm::InetService<SwcCliToCoreReq, SwcCliToCoreResp>::Ptr ser_;

    std::mutex conn_state_mtx_;
    std::unordered_map<comm::ConnectionId, uint64_t> conn_state_version_;

    std::unique_ptr<senseAD::rscl::task::SimpleTimerTaskImpl> periodic_task_;

    //======================================
    //* SWC Stat Info
    //======================================
    std::mutex reported_stats_mtx_;
    std::unordered_map<comm::ConnectionId,
                       std::shared_ptr<SendMsg<SwcCliToCoreReq>>>
        reported_stats_;
    uint32_t reported_stats_update_n_{0};

    uint32_t self_pid_{0};
    uint64_t self_start_time_{0};
};

/**
 * @brief Provide summaization service detail impl
 * - Summary Remote Endpoint
 */
class RemoteEndpointCoordinatorSummarizationService {
 public:
    RemoteEndpointCoordinatorSummarizationService(
        RemoteEndpointCoordinator* parent);
    ~RemoteEndpointCoordinatorSummarizationService();

    void Enable();
    void Shutdown();

 private:
    void ServiceHandle(
        const std::string& cli_key,
        const std::string& cli_ip,
        const ConnectionId conn_id,
        const std::shared_ptr<ReceivedMsg<CoreToCoreReq>>& req,
        std::shared_ptr<SendMsg<CoreToCoreResp>>& resp);  // NOLINT
    void BoardcastSelfStateToPeers(bool exit_state = false);
    void RemoveDisconnectedClients();
    void UpdateStateFromPeerData(const std::string& cli_key,
                                 const std::string& cli_ip,
                                 const uint64_t cli_state_version,
                                 RuntimeState state);

    void MaintainSubscribeSD();

    template <typename T>
    void FillSelfStateReqResp(SendMsg<T>* msg) const {
        auto builder = msg->MsgBuilder();

        auto self_state = parent_->system_state_.GetSelfState();
        auto addr_builder = builder.getSelfAddr();
        auto state_builder = builder.getSelfSysState();
        builder.setStateVersion(self_state_version_);
        ser_addr_.To(&addr_builder);
        self_state.To(&state_builder);
    }
    template <typename T>
    void FillExitStateReqResp(SendMsg<T>* msg) const {
        auto builder = msg->MsgBuilder();

        auto addr_builder = builder.getSelfAddr();
        builder.setStateVersion(self_state_version_ + 100);
        ser_addr_.To(&addr_builder);
    }

    RemoteEndpointCoordinator* parent_;

    bool enabled_{false};
    std::mutex mtx_;

    AddrInfo ser_addr_;
    std::vector<AddrInfo> peer_addrs_;
    comm::InetService<CoreToCoreReq, CoreToCoreResp>::Ptr ser_;

    std::mutex peer_cli_mtx_;
    std::unordered_map<key_t,
                       comm::InetClient<CoreToCoreReq, CoreToCoreResp>::Ptr>
        peer_clis_;
    std::mutex peer_state_mtx_;
    std::unordered_map<key_t, uint64_t> peer_state_version_;
    std::unordered_map<key_t, comm::ConnectionId> peer_conn_id_;

    std::atomic<uint64_t> self_state_version_;

    std::unique_ptr<senseAD::rscl::task::SimpleTimerTaskImpl> periodic_task_;
    bool need_resync_{false};

    std::mutex pubinfo_mtx_;
    std::unordered_map<std::string, TopicInfo> maintaining_fake_pubinfo_;

    std::shared_ptr<senseAD::service_discovery::SDServiceClient>
        sd_client_sptr_;
};

}  // namespace comm_cs3
}  // namespace rscl
}  // namespace senseAD
