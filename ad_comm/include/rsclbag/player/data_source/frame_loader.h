/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <limits>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <unordered_map>

#include "ad_bag/multibag_reader.h"
#include "ad_serde/ad_serde.h"
#include "rsclbag/player/data_source/hevc_loader.h"

using senseAD::bag::MultiBagReader;
using senseAD::bag::MultiBagReaderAttribute;
using senseAD::bag::ReadedMessage;
using senseAD::serde::RawMessage;

struct Channel {
    std::string channel_name;
    std::string msg_type;
    std::string msg_descriptor;
};

using Channels = std::vector<Channel>;

class LoadedMessage {
 public:
    uint64_t seq;

    std::shared_ptr<SendMsg<RawMessage>> content;
    uint32_t msg_channel_id;
    std::string msg_channel;
    // timestamp to be play
    uint64_t msg_timestamp_ns;

    int loop_index{0};
};

struct FrameLoaderAttr {
    std::set<std::string> data_paths;

    bool looping = false;
    bool looping_with_continous_timestamp = false;
    std::string msg_transform_lib = "";

    double relative_start_sec = 0.0;
    double duration_sec = std::numeric_limits<double>::max();

    uint64_t begin_time_ns = 0;
    uint64_t end_time_ns = std::numeric_limits<uint64_t>::max();

    std::set<std::string> excluded_channels;
    std::set<std::string> included_channels;
};

class MsgTransformer;
class FrameLoader {
 public:
    FrameLoader(const FrameLoaderAttr& attr,
                std::shared_ptr<MultiBagReader> bag_reader = nullptr);
    ~FrameLoader();

    bool IsDataSourceValid();
    Channels GetChannels() const;
    uint64_t GetFirstFrameTimeNs() const;
    uint64_t GetLastFrameTimeNs() const;
    int32_t GetGmtOffsetSec() const;

    std::shared_ptr<LoadedMessage> LoadNextFrame();
    bool IsNoMoreData() const;

 private:
    void InitReader(std::shared_ptr<MultiBagReader> bag_reader = nullptr);
    void InitExternLoader();
    bool ReadMsgFromDataSource();

    FrameLoaderAttr attr_;

    bool have_any_data_{false};
    uint64_t latest_seq_{1};
    int loop_index_{0};

    std::shared_ptr<MultiBagReader> bag_reader_;
    ReadedMessage read_msg_;

    Channels channels_;
    std::unordered_map<std::string, uint32_t> channel_id_map_;
    uint64_t first_frame_time_ns_{0};
    uint64_t last_frame_time_ns_{0};
    uint64_t one_loop_total_ns_{0};

    bool is_end_{false};

    std::unique_ptr<MsgTransformer> transformer_;

    std::shared_ptr<MultiHevcLoader> extern_loader_ = nullptr;
    ReadedMessage bag_msg_cache_ = {};
    bool bag_cache_ret = false;
};
