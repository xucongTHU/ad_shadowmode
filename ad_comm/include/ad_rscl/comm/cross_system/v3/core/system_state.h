/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
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

#include "ad_base/common/rpc.h"
#include "ad_rscl/comm/cross_system/v3/core/common.h"

namespace senseAD {
namespace rscl {
namespace comm_cs3 {

using senseAD::base::common::ConnectionId;

struct RuntimeState {
    //! key: topic, val: topic info
    // Used for let other machince local endpoints
    std::unordered_map<key_t, std::vector<TopicInfo>> host_sub_topic_infos;
    // Used for pre create publisher for core
    std::unordered_map<key_t, std::vector<TopicInfo>> host_pub_topic_infos;

    std::string DebugString(bool simple_mode = false) const;

    void UpdateSubEndpointIp(const std::string& dst_ip);

    static RuntimeState From(
        const senseAD::rscl::cs3::idl::RuntimeState::Reader& reader);

    void To(senseAD::rscl::cs3::idl::RuntimeState::Builder* builder) const;
    void Clear();
};

struct SystemLevelState {
 public:
    explicit SystemLevelState(const AddrInfo& host_dispatcher_addr);
    virtual ~SystemLevelState() = default;

    RuntimeState GetSelfState() const;

    //* Sync State from Remote
    std::unordered_map<key_t, RuntimeState> GetPartnerState(
        uint32_t* remote_state_version = nullptr) const;
    uint32_t GetPartnerStateVersion() const;

    std::vector<TopicInfo> AllPartnerPublishingTopicInfo();

    void SetPartnerState(const key_t& peer_key, const RuntimeState& val);
    void RemovePartnerState(const key_t& peer_key);

    //* Update local state
    void UpsertRuntimeState(const ConnectionId conn, const RuntimeState& val);
    void RemoveRuntimeState(const ConnectionId conn);

    void RegOnPeerUpdateFn(const std::function<void()>& fn);
    void RegOnSelfUpdateFn(const std::function<void()>& fn);

    std::string DebugString() const;

 private:
    AddrInfo host_dispatcher_addr_;
    mutable std::mutex mtx_;
    std::unordered_map<key_t, RuntimeState> partner_state_;
    std::unordered_map<ConnectionId, RuntimeState> state_per_connection_;
    mutable std::unique_ptr<RuntimeState> self_state_cache_;

    mutable std::mutex on_update_fn_mtx_;
    std::function<void()> on_self_update_fn_;
    std::function<void()> on_peer_update_fn_;

    std::atomic<uint32_t> remote_state_version_{0};
};

}  // namespace comm_cs3
}  // namespace rscl
}  // namespace senseAD
