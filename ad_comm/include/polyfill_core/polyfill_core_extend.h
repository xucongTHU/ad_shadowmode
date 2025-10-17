/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "ad_rscl/polyfill_core/polyfill_core_server.h"

#include "polyfill_core/external_msg_forward_server.h"
#include "polyfill_core/simple_shm.h"
#include "rscl_graph/rscl_graph_manager.h"
#include "http_server/http_server.h"
// #include "ad_rscl/comm/cross_system/v2/cross_system_service_v2.h"
#include "ad_rscl/comm/cross_system/v3/server_side/data_distribute_service.h"
#include "ad_rscl/comm/cross_system/v3/server_side/remote_endpoint_coordinator.h"
#include "ad_rscl/idl/runtime_state.capnp.h"
#include "ad_rscl/task/task.h"

using senseAD::rscl::idl::ComponentStateInfo;
using senseAD::rscl::task::TimerTask;
using senseAD::rscl::task::TimerTaskParam;

class PolyFillCoreServerExtend
    : public senseAD::rscl::core::PolyFillCoreServer {
 public:
    PolyFillCoreServerExtend();
    PolyFillCoreServerExtend(senseAD::rscl::comm::Node *core_node_internal,
                             senseAD::rscl::comm::Node *core_node);
    void InitNode(senseAD::rscl::comm::Node *core_node_internal,
                  senseAD::rscl::comm::Node *core_node);
    ~PolyFillCoreServerExtend();

    void EnableCrossSystemService();
    void DisableCrossSystemService();

    void EnableStateForwardServer(
        const std::vector<std::string> &forward_topics);
    void DisableStateForwardServer();

    void EnableExternalMsgForwardServer(
        const senseAD::ad_msg_bridge::MsgBridgeConfig &conf);
    void DisableExternalMsgForwardServer();

    void EnableRsclGraphManager();
    void DisableRsclGraphManager();

    void EnableRsclGraphFrontend();

    void InitAll() override;
    void ShutdownImpl() override;

    void InitServiceDiscoveryServer();

 private:
    void SendComponentStatus();

    senseAD::rscl::comm::Node *core_node_internal_;
    senseAD::rscl::comm::Node *core_node_;

    // std::shared_ptr<senseAD::rscl::comm_cs::CrossSystemServiceV2>
    //     cross_system_service_;
    std::unique_ptr<senseAD::rscl::comm_cs3::DataDistributeService>
        distribute_ser_;
    std::unique_ptr<senseAD::rscl::comm_cs3::RemoteEndpointCoordinator>
        coordinator_ser_;
    std::unique_ptr<senseAD::rscl::comm_cs3::StatRecordReporter>
        cs3_stat_reporter_;

    std::shared_ptr<ExternalMsgForwardServer> external_msg_forward_server_;

    senseAD::rscl::comm::PublisherBase::Ptr state_pub_;
    uint64_t pid_ = 0;
    std::unique_ptr<TimerTask> status_timer_;

    std::once_flag service_discovery_server_init_flag_;

    std::unique_ptr<senseAD::rscl::graph_tool::RsclGraphManager>
        rscl_graph_manager_;
    std::unique_ptr<simple_http_server::HttpServer> graph_server_;

    std::shared_ptr<senseAD::sensor::SimpleSharedMem> shared_mem_;
};
