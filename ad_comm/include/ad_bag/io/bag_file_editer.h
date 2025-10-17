/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file bag_file_editer.h
 * @brief Edit the data into the rsclbag file.
 */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>

#include "ad_bag/schema/schema.h"
#include "ad_bag/bag_flags.h"
#include "ad_bag/io/common_struct.h"

namespace senseAD {
namespace bag {

/**
 * @brief Class for writing raw data format into single bag file
 * with low-level api
 *
 */
class BagFileEditer {
 public:
    explicit BagFileEditer(const std::string& bag_name);
    ~BagFileEditer();

    /**
     * @brief Write extra bag info into bag
     *
     */
    bool OverwriteExtraBagInfo(const ExtraBagInfo& extra_bag_info,
                               const std::vector<UserDefineItem>& user_defines);

    /**
     * @brief Write magic number in the end of bag
     */
    bool WriteEndAndCloseFile();

 private:
    std::string bag_name_;
    FILE* fptr_;
    bool closed_{false};

    // For global header
    BagHeader bag_header_;
};

}  // namespace bag
}  // namespace senseAD
