/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <string>

namespace senseAD {
namespace rscl {
namespace common_bridge {

// tcp://192.168.0.1:5432  --->  tcp://192.168.0.1:5433
std::string PortIncrement(const std::string& src_ip, int inc = 1);

std::string GetReceiveIp();

}  // namespace common_bridge
}  // namespace rscl
}  // namespace senseAD
