//
// Created by xucong on 25-5-9.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "debouncer.h"

namespace shadow {
namespace common {

Debouncer::Debouncer(int cycleTime, int duration) : m_timer(cycleTime, duration) {}

void Debouncer::setDuration(int duration) {
    m_timer.setDuration(duration);
}

bool Debouncer::getDebounceResult(const bool &val) {
    if (!val)
    {
        m_timer.reset();
        return false;
    }
    else
    {
        m_timer.tick();
        //only when val && timer elapsed, then the checkBool will be set
        bool checkBool = (val && m_timer.isElapsed());
        return checkBool;
    }
}

void Debouncer::resetTimer() {
    m_timer.reset();
}

} // namespace common
} // namespace shadow

