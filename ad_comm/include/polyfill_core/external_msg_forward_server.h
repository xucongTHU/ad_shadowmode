/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 * Caiwenkang <caiwenkang@senseauto.com>
 */

#pragma once

#include <stdlib.h>
#include <unistd.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <string>
#include <iostream>

#include "ad_base/common/shutdownable.h"
#include "ad_rscl/ad_rscl.h"
#include "ad_rscl/polyfill_core/polyfill_core_server.h"
#include "ad_service_discovery/service_discovery/types.h"

using senseAD::service_discovery::SDServiceServer;

constexpr char kExternalTopicPrefix[] = "_ext/";

class ExternalMsgForwardServer : public ::Shutdownable {
 public:
    explicit ExternalMsgForwardServer(
        senseAD::rscl::comm::Node* node,
        const senseAD::ad_msg_bridge::MsgBridgeConfig& conf);
    ~ExternalMsgForwardServer() override;

    void Init();
    void ShutdownImpl() override;
    void AddForwardBridgeToRsclListener(const std::string& external_topic,
                                        const std::string& rscl_topic);
    void AddForwardRsclToBridgeListener(const std::string& rscl_topic,
                                        const std::string& external_topic);

 private:
    void PublishInternal(
        const std::string& topic,
        const std::shared_ptr<ReceivedMsg<senseAD::mw::RawMessage>>& msg);

    void PublishExternal(
        const std::string& topic,
        senseAD::rscl::comm::PublisherBase::Ptr pub,
        const std::shared_ptr<ReceivedMsg<senseAD::mw::RawMessage>>& msg);

    senseAD::rscl::comm::Node* node_;
    senseAD::ad_msg_bridge::MsgBridgeConfig bridge_conf_;

    std::mutex pub_mtx_;
    std::unordered_set<std::string> external_topics_;
    std::unordered_set<std::string> rscl_topics_;

    std::unordered_map<std::string, senseAD::rscl::comm::SubscriberBase::Ptr>
        ext_subs_;
    std::unordered_map<std::string, senseAD::serde::MsgMeta> ext_sub_metas_;
    std::unordered_map<std::string, senseAD::rscl::comm::PublisherBase::Ptr>
        inner_pubs_;

    std::unordered_map<std::string, senseAD::rscl::comm::SubscriberBase::Ptr>
        inner_subs_;
    std::unordered_map<std::string, senseAD::rscl::comm::PublisherBase::Ptr>
        ext_pubs_;

    std::unordered_map<std::string, uint64_t> channel_seq_;
    std::unordered_map<std::string, std::shared_ptr<std::vector<char>>>
        msg_datas_;
    std::unordered_map<std::string, std::unique_ptr<std::mutex>> datas_mtx_;
    std::mutex msg_datas_mtx_;
};
