/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <set>

#include "ad_mw/ad_mw.h"

namespace senseAD {
namespace rscl {
constexpr const char* kProcessStateChannel =
    "/internal_swc/process_state/12000";
constexpr const char* kSimClockChannel = "/internal_time/sim_time/11000";
constexpr const char* kMonitorRequestChannel =
    "/internal_monitor/request/13000";
constexpr const char* kNodeStateChannel = "/internal_swc/node_state/12000";
constexpr const char* kComponentStateChannel =
    "/internal_swc/component_state/12000";
constexpr const char* kHeteroControllerChannel = "/controller/channel";

std::set<std::string> InternalAutoPublishTopics();
std::string PrefixedPeerChannelName(const std::string& peer_name,
                                    const std::string& channel_name);

void AssertChannelNameFormat(const std::string& channel_name,
                             const std::string& debug_log = "");

}  // namespace rscl
}  // namespace senseAD
