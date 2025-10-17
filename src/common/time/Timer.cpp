//
// Created by xucong on 25-2-12.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include <iomanip>
#include <iostream>
#include <sstream>

#include "common/time/Timer.h"
#include "common/logger/Logger.h"

namespace shadow {
namespace common {

// String to identify log entries originating from this file.
static const char* LOG_TAG = "time";

Performance::Performance(const std::string& str) {
  description_ << str;
  is_printf_ = 1;
}

Performance::Performance(const std::string& str, const bool& is_printf) {
  description_ << str;
  is_printf_ = is_printf;
}

}  // namespace common
}  // namespace shadow
