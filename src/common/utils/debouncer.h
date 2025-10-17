//
// Created by xucong on 25-5-9.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_DEBOUNCER_H
#define SHADOW_MODE_DEBOUNCER_H

#include <iostream>

namespace shadow {
namespace common {

class TimeSeries {
public:
    TimeSeries() = default;
    TimeSeries(int cycleTime, int duration) : m_cycleTime(cycleTime), m_duration(duration), m_currentTime(0) {}
    void tick() {
        if (m_currentTime < m_duration)
        {
            m_currentTime += m_cycleTime;
        }
    };
    bool isElapsed() const { return m_currentTime >= m_duration; };
    void setDuration(int duration) { m_duration = duration; }
    void reset() { m_currentTime = 0; }

private:
    int m_cycleTime;
    int m_duration;
    int m_currentTime;
};

class TimerT20 : public TimeSeries {
public:
    TimerT20(int duration) : TimeSeries(20, duration) { }
};

class TimerT10 : public TimeSeries {
public:
    TimerT10(int duration) : TimeSeries(10, duration) {}
};


class Debouncer {
public:
    explicit Debouncer(int cycleTime, int duration);
    bool getDebounceResult(const bool &val);
    void setDuration(const int val);
    void resetTimer();

private:
    TimeSeries m_timer;

};

} // namespace common
} // namespace shadow


#endif //SHADOW_MODE_DEBOUNCER_H
