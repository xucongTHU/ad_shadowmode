/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once
#include <cstdint>
#include <string>
#include <unordered_set>

namespace senseAD {
namespace base {
namespace common {

std::unordered_set<std::string> GetIfName();
std::string GetMacAddr(const std::string &if_name);
std::unordered_set<std::string> GetMacAddr();
std::unordered_set<std::string> GetIpAddress();

bool ParseIpPort(const std::string &address, std::string *ip, uint32_t *port);
bool ParseProtocolIpPort(const std::string &address,
                         std::string *protocol,
                         std::string *ip,
                         uint32_t *port);

std::string GetHostIp();

}  // namespace common
}  // namespace base
}  // namespace senseAD
