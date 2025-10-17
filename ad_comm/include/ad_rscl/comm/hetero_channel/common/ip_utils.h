/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <string>

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

// tcp://192.168.0.1:5432  --->  tcp://192.168.0.1:5433
std::string PortIncrement(const std::string& src_ip, int inc = 1);

}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
