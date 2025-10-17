/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

namespace senseAD {
namespace base {
namespace common {

constexpr const char* kDefaultTimeZone = "";
constexpr char kProjectSenseautoRsclConfigDir[] =
    "";

constexpr char kProjectSenseautoRsclDir[] = "";

static u_char *const kProcessPrefix = (u_char *)"rscl-";

const std::string kThreadNamePrefix = "";  // NOLINT

constexpr char kExpectedPolyfillCorePath[] = "/opt/senseauto_active/senseauto-rscl/bin/rscl_polyfill_core";

constexpr char kMsgBridgeDefaultConfPath[] = "";
}  // namespace common
}  // namespace base
}  // namespace senseAD
