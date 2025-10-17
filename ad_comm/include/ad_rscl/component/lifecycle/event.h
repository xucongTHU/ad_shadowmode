/**
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

namespace senseAD {

enum class Event {
    SYSTEM_STARTUP,
    INIT_DONE,
    START_ALLOWED,
    TICK,
    SUSPEND,
    RESET,
    SWITCH_TO_DRIVIING_MODE,
    SWITCH_TO_PILOT_MODE,
    SWITCH_TO_PARKING_MODE,
    RESUME_RUNNING,
    ERROR,
    SYSTEM_EXIT
};

}  // namespace senseAD
