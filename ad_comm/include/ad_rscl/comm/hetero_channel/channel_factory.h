/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "ad_rscl/global_config.h"
#include "ad_rscl/comm/hetero_channel/basic_type.h"
#include "ad_serde/msgmeta.h"
#include "ad_rscl/idl/hetero_channel_config.pb.h"
#include "ad_service_discovery/service_discovery/service_discovery.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

using senseAD::rscl::idl::HeteroChannelConfig;
using senseAD::rscl::idl::SupportedHeteroChannel;

class ChannelFactory {
 public:
    std::vector<std::shared_ptr<PublisherBase>> CreatePublisher(
        const std::string& node_name, const std::string& topic);
    std::vector<std::shared_ptr<SubscriberBase>> CreateSubscriber(
        const std::string& node_name,
        const std::string& topic,
        const SubscriberBase::CallbackFunc& fn) const;

    bool GetMsgMeta(const std::string& topic,
                    senseAD::serde::MsgMeta* meta) const;

 private:
    std::vector<std::shared_ptr<PublisherBase>> CreatePublisher(
        const std::string& node_name,
        const std::string& topic,
        const HeteroChannelConfig& conf);

    // @TODO(kit) Not used in SubscriberBase yet
    std::vector<std::shared_ptr<SubscriberBase>> CreateSubscriber(
        const std::string& node_name,
        const std::string& topic,
        const SubscriberBase::CallbackFunc& fn,
        const HeteroChannelConfig& conf) const;

    std::unordered_map<std::string, senseAD::serde::MsgMeta> msg_metas_;

    std::shared_ptr<senseAD::service_discovery::ServiceDiscovery> sd_;
    DECLARE_AD_SINGLETON(ChannelFactory)
};

}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
