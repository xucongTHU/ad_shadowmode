/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * gary <zhangyi15@senseauto.com>
 */

#pragma once

#include <string>
#include "ad_base/common/project_env.h"

namespace senseAD {
namespace base {
namespace common {

bool SetThreadName(const std::string& thread_name);

}  // namespace common
}  // namespace base
}  // namespace senseAD
