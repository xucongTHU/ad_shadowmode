/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstdint>

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <thread>
#include <deque>
#include <queue>
#include <tuple>
#include <condition_variable>

#include "ad_rscl/ad_rscl.h"
#include "ad_bag/bag_writer.h"
#include "ad_bag/bag_flags.h"
#include "rsclbag/player/msg_transform/msg_transform.h"

using senseAD::bag::BagWriteExtraInfo;
using senseAD::bag::BagWriter;
using senseAD::bag::BagWriterAttribute;
using senseAD::bag::ChannelInfo;
using senseAD::rscl::comm::RawMessage;
using senseAD::rscl::time::Clock;
using senseAD::rscl::time::ClockMode;

struct RecorderOptions {
    std::set<std::string> excluded_channels;
    std::set<std::string> included_channels;
    size_t bagsplit_bytes = 0;
    size_t bagsplit_msgcnt = 0;
    std::string bridge_config;
    std::string compress;
    std::string platform;
    std::string vehicle;
    std::string user_define;
    std::unordered_map<std::string, std::string> remap_channels;
    bool use_msg_header_time{false};
    std::string msg_transform_so;
};

class Recorder {
 public:
    Recorder(const std::string &out_bag_basename,
             const RecorderOptions &options = RecorderOptions());
    ~Recorder();

    bool Start();
    bool Stop();

 private:
    bool RegisterChannel(const std::string &channel_name);
    void NewMessageCallback(const std::shared_ptr<ReceivedMsg<RawMessage>> &msg,
                            std::shared_ptr<ChannelInfo>);
    void ShowProgress() const;
    void MessageIoThread();

    void AddSubscriber();
    void InitExtraBagInfo();

    std::string out_bag_basename_;
    RecorderOptions options_;
    std::shared_ptr<rscl::Node> node_;
    senseAD::service_discovery::ServiceDiscovery *service_discovery_ = nullptr;

    std::shared_timed_mutex start_stop_mtx_;
    bool running_ = false;

    std::mutex register_mtx_;
    std::unordered_map<std::string, std::shared_ptr<ChannelInfo>>
        registered_channels_;
    std::unordered_map<std::string, int> failed_msg_type_cnts_;
    std::vector<rscl::SubscriberBase::Ptr> subs_;
    std::unique_ptr<BagWriter> writer_;
    std::queue<std::shared_ptr<ChannelInfo>> channel_q_;
    std::thread subs_thread_;
    std::condition_variable sub_cv_;

    std::atomic<uint64_t> message_count_{0};
    std::atomic<uint64_t> dropped_count_{0};
    uint64_t message_time_ = 0;

    std::mutex msg_mtx_;
    std::deque<std::tuple<uint64_t,
                          std::shared_ptr<ReceivedMsg<RawMessage>>,
                          std::shared_ptr<ChannelInfo>>>
        msg_q_;
    std::thread tui_thread_;
    std::thread io_thread_;
    std::unique_ptr<MsgTransformer> msg_transform_;
};
