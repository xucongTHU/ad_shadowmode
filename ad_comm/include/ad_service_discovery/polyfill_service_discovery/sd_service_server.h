/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once

#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <functional>
#include <string>
#include <vector>
#include <memory>

#include "ad_base/common/rpc.h"
#include "ad_base/common/shutdownable.h"
#include "ad_mw/types.h"
#include "ad_mw/comm.h"
#include "ad_service_discovery/polyfill_transport/transport_impl.h"
#include "ad_service_discovery/polyfill_service_discovery/common.h"
#include "ad_service_discovery/polyfill_service_discovery/request_response.h"
#include "ad_service_discovery/service_discovery/types.h"
#include "ad_serde/ad_serde.h"
#include "ad_serde/msgmeta.h"

namespace senseAD {
namespace service_discovery {

using MsgMeta = senseAD::serde::MsgMeta;
using senseAD::base::common::ConnectionId;

struct NodeInfo {
    SrvDiscEvent node_info;

    std::unordered_map<std::string, SrvDiscEvent> pub_discovery;
    std::unordered_map<std::string, SrvDiscEvent> sub_discovery;
    std::unordered_map<std::string, SrvDiscEvent> cli_discovery;
    std::unordered_map<std::string, SrvDiscEvent> srv_discovery;
};

class SDServiceServer : public ::Shutdownable {
 public:
    SDServiceServer();
    virtual ~SDServiceServer();

    using UpsertDiscoveryMetaListenerCallbackFunc =
        std::function<void(const SrvDiscEvent& meta)>;
    bool Init();
    void ShutdownImpl() override;
    // TODO(zhoupenghui) The API have problem, need fix (we may not use sd
    // polyfill)
    void RegisteUpsertDiscoveryMetaListener(
        const UpsertDiscoveryMetaListenerCallbackFunc& cb);

    std::unordered_map<std::string, NodeInfo> GetDiscoveryMeta() const {
        return node_info_map_;
    }

 private:
    enum ListenerAction {
        NONE,
        UPSERT_DISCOVERY_META,
        GET_DISCOVERY_META,
        UPSERT_CHANNEL_META,
        GET_CHANNEL_META,
        NOTIFY_SUBSCRIBE,
        SEND_DISCOVERY_META
    };
    void UpsertDiscoveryMetaMsg(const UpsertDiscoveryMetaRequest& req,
                                UpsertDiscoveryMetaResponse* resp);
    void GetDiscoveryMetaMsg(const GetDiscoveryMetaRequest& req,
                             GetDiscoveryMetaResponse* resp);

    void SendDiscoveryMeta(const UpsertDiscoveryMetaRequest& req);
    void BuilderDiscovery(GetDiscoveryMetaResponse* resp);
    void QueryDiscoveryInfo(
        std::vector<SrvDiscEvent>* info,
        const std::unordered_map<std::string, SrvDiscEvent>& info_map);
    void QueryDiscoveryInfoByNodeName(std::vector<SrvDiscEvent>* info,
                                      const std::string& name = "");
    void QueryDiscoveryInfoByChannlName(std::vector<SrvDiscEvent>* info,
                                        const std::string& name);
    void QueryDiscoveryInfoByServiceName(std::vector<SrvDiscEvent>* info,
                                         const std::string& name);

 private:
    void EraseAllForNode(const std::string& node_name);
    void PrintDebugGraph();
    bool CheckChannelMetaMsg(const std::string& channel_name,
                             const MsgMeta& meta);

 private:
    std::unordered_map<base::common::ConnectionId,
                       std::unordered_set<std::string>>
        cid_node_name_map_;
    std::unordered_map<std::string, NodeInfo> node_info_map_;
    std::unordered_map<std::string, MsgMeta> channel_meta_map_;
    std::unordered_map<int,
                       std::vector<UpsertDiscoveryMetaListenerCallbackFunc>>
        listener_callbacks_;

    using SurveyPtr = std::shared_ptr<transport::Survey>;

    std::mutex node_info_mt_;
    std::mutex sub_service_mt_;
    std::mutex channel_mt_;

    std::unique_ptr<base::common::RpcServer> rpc_server_;
    std::thread notify_thread_;
    std::shared_ptr<transport::Publisher> notify_pub_;
    SurveyPtr sd_listener_survey_;

    base::common::RpcStatus RpcCallDispatch(const ConnectionId cid,
                                            const char* in_data_ptr,
                                            const size_t in_data_size,
                                            std::vector<char>* out_data);

    bool is_running_{false};
    std::ofstream stat_file_;

    static const int kPrintDebugGraphInterval = 10;
    uint64_t last_print_debuggraph_{0};
};

}  // namespace service_discovery
}  // namespace senseAD
