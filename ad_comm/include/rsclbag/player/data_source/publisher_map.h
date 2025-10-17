/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "ad_rscl/ad_rscl.h"
#include "rsclbag/player/data_source/frame_loader.h"

class PublisherMap {
 public:
    PublisherMap(rscl::Node* node,
                 const Channels& channels,
                 const bool disable_cross_system = false);
    PublisherMap(
        rscl::Node* node,
        const Channels& channels,
        const std::unordered_map<std::string, std::string>& remap_channels,
        const bool disable_cross_system = false);
    ~PublisherMap() = default;

    inline rscl::Publisher<RawMessage>* GetPublisher(
        const std::string& channel) const {
        return pubs_.at(channel).get();
    }

 private:
    void Init(
        rscl::Node* node,
        const Channels& channels,
        const std::unordered_map<std::string, std::string>& remap_channels,
        const bool disable_cross_system);

    std::unordered_map<std::string, rscl::Publisher<RawMessage>::Ptr> pubs_;
};
