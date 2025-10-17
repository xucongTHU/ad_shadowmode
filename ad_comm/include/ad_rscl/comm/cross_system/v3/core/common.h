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

#include "ad_base/std/optional.h"
#include "ad_serde/msgmeta.h"
#include "ad_base/common/thread_pool.h"
#include "ad_rscl/idl/cross_system_v3.capnp.h"

namespace senseAD {
namespace rscl {
namespace comm_cs3 {

//* =============================================================
//*   Utilites
//* =============================================================

typedef std::string key_t;
using TopicID = size_t;

/**
 * @brief Using a compressed id to represent a topic
 * Speed up all hash operation that need to use topic name as key
 */
class TopicIDMap {
 public:
    inline TopicID GetId(const std::string& topic_name) {
        std::lock_guard<std::mutex> lg(mtx_);
        auto itr = topic_ids_.find(topic_name);
        if (itr != topic_ids_.end()) {
            return itr->second;
        }
        TopicID id{topics_.size()};
        topic_ids_[topic_name] = id;
        topics_.emplace_back(topic_name);
        return id;
    }

    inline base::optional<std::string> GetName(TopicID id) const {
        std::lock_guard<std::mutex> lg(mtx_);
        if (topics_.size() < id) {
            return {};
        }
        return {topics_.at(id)};
    }

    inline std::vector<std::string> GetAllNames() const {
        std::lock_guard<std::mutex> lg(mtx_);
        return topics_;
    }

 private:
    mutable std::mutex mtx_;
    std::unordered_map<std::string, TopicID> topic_ids_;
    std::vector<std::string> topics_;
};
using PeerID = TopicID;
using PeerIDMap = TopicIDMap;

//* =============================================================
//*   Core Info
//* =============================================================

/**
 * @brief Describe a remote server address
 *
 */
struct AddrInfo {
    std::string name{};
    std::string ip{};
    uint16_t port{0};

    inline bool Empty() const { return port == 0; }
    // support multi-instance of CrossSystem class.
    inline key_t AsKey() const { return ip + ":" + std::to_string(port); }

    static AddrInfo From(
        const senseAD::rscl::cs3::idl::AddrInfo::Reader& reader);

    void To(senseAD::rscl::cs3::idl::AddrInfo::Builder* builder) const;
};

struct TopicInfo {
    // a key/topic name used in the service exchange
    key_t topic;

    std::string type_name;
    std::string descriptor;

    // The subscriber process tcp client
    AddrInfo local_endpoint;

    std::string DebugString() const;

    static TopicInfo From(
        const senseAD::rscl::cs3::idl::TopicInfo::Reader& reader);

    void To(senseAD::rscl::cs3::idl::TopicInfo::Builder* builder) const;
};

//* =============================================================
//*   Config
//* =============================================================

struct CoordinatorConfig {
    std::vector<AddrInfo> peer_summarization_service_addrs;

    AddrInfo dispatcher_service_host_addr;

    AddrInfo query_service_host_addr;
    AddrInfo summarization_service_host_addr;

    // ***** unittest only *****
    bool disable_query_service{false};
    bool disable_summarization_service{false};
    std::vector<std::string> must_receive_topics;
    // **************************
};

struct DataDistributeServiceConfig {
    AddrInfo dispatcher_service_host_addr;

    // If set, only allow received data from those peer ip
    std::vector<std::string> peer_whitelist;

    // Regex supported
    std::unordered_set<std::string> excluded_topics;
    // Regex supported
    std::unordered_set<std::string> whitelist_topics;
};

struct EndpointConfig {
    uint32_t send_thread_num = 4;
    bool send_thread_debug = false;
};

struct StatReportConfig {
    bool enable = false;
    uint32_t sample_interval = 50;
    uint32_t report_interval = 1000;
};

}  // namespace comm_cs3
}  // namespace rscl
}  // namespace senseAD
