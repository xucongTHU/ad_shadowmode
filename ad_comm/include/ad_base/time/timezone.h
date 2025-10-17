/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once
#include <stdint.h>
#include <string>

namespace senseAD {
namespace base {
namespace time {

/**
 * @brief Return the offset second to GMT 0
 * value should between -13 hour to +13 hour
 * @return int32_t offset in second unit
 */
int32_t GMTOffsetSecond();

/**
 * @brief Return the time zone name
 *
 * @return std::string
 */
std::string MachineTimeZoneString();

std::string ProcessTimeZoneString();

std::string DatetimeStringWithTimeZone(const uint64_t linux_tp_sec,
                                       const int32_t gmt_offset_sec);

uint64_t DatetimeToLinuxTimeSec(
    const std::string& datetime_str,
    const int32_t gmt_offset_sec,
    const std::string& str_format = "%Y-%m-%d %H:%M:%S");
}  // namespace time
}  // namespace base
}  // namespace senseAD
