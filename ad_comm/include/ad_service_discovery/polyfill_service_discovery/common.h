/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

namespace senseAD {
namespace service_discovery {

static constexpr const char* kServiceDiscoveryTopic =
    "/internal_sd/service_discovery_topic/10000";
static constexpr const char* kServiceQueryTopic =
    "/internal_sd/service_qurey_topic/10000";
static constexpr const char* kUpsertChannelMetaTopic =
    "/internal_sd/upsert_channel_meta_topic/10000";
static constexpr const char* kGetChannelMetaTopic =
    "/internal_sd/get_channel_meta_topic/10000";
static constexpr const char* kNotifySubscribeTopic =
    "/internal_sd/notify_subscribe_topic/10000";

static constexpr const char* kNotifyTopic = "polyfill_core_notify";

static constexpr const char* kSdListenerTopic = "discovery_listener";

// currently service discovery only work in self host
static constexpr const char* kTargetAddress = "127.0.0.1";
static constexpr uint32_t kSdCliSrvPort = 12173;

static constexpr uint32_t kNotifySubscribeTopicPort = 12177;
static constexpr uint32_t kNotifyTopicPort = 12178;
static constexpr uint32_t kSdListenerTopicPort = 12179;

}  // namespace service_discovery
}  // namespace senseAD
