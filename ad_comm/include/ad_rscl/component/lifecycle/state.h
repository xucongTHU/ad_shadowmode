/**
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>

namespace senseAD {

enum class StateStage {
    WAITING,
    INITING,
    READY,
    RUNNING,
    SUSPENDED,
    RESUMING,
    RESETTING,
    STOPPED_NORMAL,
    STOPPED_ABNORMAL,
    FAILED,
    UNKNOWN,
    MODE_SWITCHING,
};

enum class RunningMode { NOT_RUNNING, DRIVING, PILOT, PARKING };

struct State {
    StateStage stage = StateStage::WAITING;
    RunningMode mode = RunningMode::NOT_RUNNING;

    inline bool AbleToProc() const { return stage == StateStage::RUNNING; }
    inline std::string DebugString() const {
        return "StateStage: " + std::to_string(static_cast<int>(stage)) +
               "  RunningMode: " + std::to_string(static_cast<int>(mode));
    }
};

}  // namespace senseAD
