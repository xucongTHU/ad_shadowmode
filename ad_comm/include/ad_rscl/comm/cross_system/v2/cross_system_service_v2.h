/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <memory>

#include "ad_base/common/shutdownable.h"
#include "ad_rscl/comm/node.h"
#include "ad_rscl/comm/cross_system/v2/common.h"
#include "ad_rscl/comm/cross_system/v2/stat_record.h"

namespace senseAD {
namespace rscl {
namespace comm_cs {

struct CrossSystemServiceV2Param {
    //! DEBUG ONLY FLAG
    // the topic from peer will publish with a prefix
    std::string hostname;
    // for topic that may have multiple publisher in multiple system
    std::set<std::string> common_topics;
    // if false, only apply to common_topic, otherwise, apply to all topic
    bool topic_outgoing_prefix_apply_all = false;

    AddrInfo host_info;

    // Preset peer
    std::vector<AddrInfo> peer_info;

    // TODO(kit) support it. used with dynamic peer
    base::optional<AddrInfo> discovery_info;
    // TODO(kit) support it. used with dynamic peer
    std::vector<AddrInfo> peer_whitelist;

    // Regex supported
    std::set<std::string> excluded_topics;
    // Only watch the topic within this list
    // Regex supported
    std::set<std::string> whitelist_topics;
    // Regex supported
    std::set<std::string> outgoing_only_topics;

    bool multiple_publisher_mode = true;

    StatReportConfig stat_report_config;

    CrossSystemServiceV2Param() = default;
    CrossSystemServiceV2Param(const CrossSystemServiceV2Param& other) {
        *this = other;
    }

    void operator=(const CrossSystemServiceV2Param& rhs) {
        hostname = rhs.hostname;
        common_topics = rhs.common_topics;
        topic_outgoing_prefix_apply_all = rhs.topic_outgoing_prefix_apply_all;

        host_info = rhs.host_info;
        peer_info = rhs.peer_info;
        discovery_info.reset();
        if (rhs.discovery_info) {
            discovery_info.emplace(rhs.discovery_info.value());
        }
        peer_whitelist = rhs.peer_whitelist;
        excluded_topics = rhs.excluded_topics;
        whitelist_topics = rhs.whitelist_topics;
        outgoing_only_topics = rhs.outgoing_only_topics;
        multiple_publisher_mode = rhs.multiple_publisher_mode;
        stat_report_config = rhs.stat_report_config;
    }
};

class CrossSystemServiceV2Impl : public ::Shutdownable {
 public:
    CrossSystemServiceV2Impl() : ::Shutdownable("CrossSystemServiceV2Impl") {}
    virtual ~CrossSystemServiceV2Impl() { Shutdown(); }

    /**
     * @brief Expected this node is dedicated for cross service
     *
     * @param node Node handle that dedicated for cross service
     * @param param Param used to control behaviour
     * @return boo Whether Init sucess or not
     */
    virtual bool Init(rscl::Node* node,
                      const CrossSystemServiceV2Param param) = 0;
    void ShutdownImpl() override{};
    virtual bool Start() = 0;
    virtual bool Stop() = 0;

    virtual StatRecord GetStatistics() const = 0;

    virtual const OverallState* GetOverallStatePtr() const = 0;
    virtual CrossSystemServiceV2Param param() const = 0;
};

class CrossSystemServiceV2 : public ::Shutdownable {
 public:
    static std::unique_ptr<CrossSystemServiceV2> New(
        rscl::Node* node,
        const CrossSystemServiceV2Param param = CrossSystemServiceV2Param());

    ~CrossSystemServiceV2();
    void ShutdownImpl() override;
    bool Start();
    bool Stop();
    StatRecord GetStatistics() const;

 private:
    explicit CrossSystemServiceV2(
        std::unique_ptr<CrossSystemServiceV2Impl> impl);

    std::unique_ptr<CrossSystemServiceV2Impl> impl_;
};
}  // namespace comm_cs
}  // namespace rscl
}  // namespace senseAD
