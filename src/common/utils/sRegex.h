//
// Created by xucong on 25-5-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_SREGEX_H
#define SHADOW_MODE_SREGEX_H

#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <boost/regex.hpp>

#include "nlohmann/json.hpp"

namespace shadow {
namespace common {
using json = nlohmann::json;
using namespace boost;

bool IsMatch(const std::string &path, const std::string &regexPattern);

} // namespace common
} // namespace shadow

#endif //SHADOW_MODE_SREGEX_H
