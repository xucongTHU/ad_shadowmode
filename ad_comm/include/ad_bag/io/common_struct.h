/**
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file common_struct.h
 * @brief Store common struct for both reader and writer while not belong to the
 * io scheme
 */

#pragma once

#include <vector>
#include <memory>

#include "ad_bag/schema/schema.h"

namespace senseAD {
namespace bag {

struct BagExtraInfoData {
    bool is_valid = false;
    ExtraBagInfo extra_bag_info;
    std::vector<std::unique_ptr<UserDefineItem>> user_defines;
};

}  // namespace bag
}  // namespace senseAD
