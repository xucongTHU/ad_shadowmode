/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <signal.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <csignal>

#include "ad_mw/visibility_control.h"
#include "ad_mw/proto/runtime_conf.pb.h"
#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace mw {

using KeyValue = std::unordered_map<std::string, std::string>;
using KeyValues = std::unordered_map<std::string, std::vector<std::string>>;

struct RuntimeParam {
    std::string process_name;
    std::string version;
};

struct RuntimeConfig {
    // project config
    senseAD::mw::proto::RuntimeConfig pj_cfg;
    // dynamic init
    RuntimeParam rt_param;
};

enum class State {
    STATE_UNINITIALIZED = 0,
    STATE_INITIALIZED,
    STATE_SHUTTING_DOWN,
    STATE_SHUTDOWN,
};
ADMW_PUBLIC void Init(int argc,
                      char* argv[],
                      const RuntimeConfig* conf = nullptr);
ADMW_PUBLIC void Shutdown();
ADMW_PUBLIC State GetState();
ADMW_PUBLIC void InitExternalIPC();
ADMW_PUBLIC void DeinitExternalIPC();
ADMW_PUBLIC KeyValue GetKeyValue();
ADMW_PUBLIC KeyValues GetKeyValues();

enum class MwSignal : int {
    kCyberNodeNameDuplicate = SIGUSR2,
};

}  // namespace mw
}  // namespace senseAD
