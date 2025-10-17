/**
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file write_strategy.h
 * @brief Extract some common write strategy for both writer and editer to use
 */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>

#include "ad_bag/schema/schema.h"
#include "ad_bag/bag_flags.h"

namespace senseAD {
namespace bag {

bool WriteExtraBagInfo(FILE* fptr,
                       const uint64_t extra_bag_info_offset,
                       const ExtraBagInfo& extra_bag_info,
                       const std::vector<UserDefineItem>& user_defines);

bool WriteEndAndCloseFile(FILE* fptr);

}  // namespace bag
}  // namespace senseAD
