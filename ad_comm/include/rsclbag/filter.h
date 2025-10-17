/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstdint>

#include <vector>
#include <string>
#include <set>
#include <limits>
#include <unordered_map>

struct BagFilterOptions {
    uint64_t begin_time_ns = 0;
    uint64_t end_time_ns = std::numeric_limits<uint64_t>::max();

    std::set<std::string> excluded_channels;
    std::set<std::string> included_channels;
    std::unordered_map<std::string, std::string> remap_channels;
    std::string out_compress;
};

class BagFilter {
 public:
    explicit BagFilter(const BagFilterOptions &options = BagFilterOptions());
    ~BagFilter() = default;

    bool Filter(const std::vector<std::string> in_bags,
                const std::string &out_bag_name) const;

 private:
    BagFilterOptions options_;
};
