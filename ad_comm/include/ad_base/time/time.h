/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <time.h>
#include <stdio.h>

#include "ad_base/time/clock.h"
#include "ad_base/time/rate.h"

namespace senseAD {
namespace base {

namespace time {

/**
 * @brief Class for user to get time
 *
 */
class Time {
 public:
    static const Time MAX;
    static const Time MIN;
    Time() = default;
    explicit Time(uint64_t nanoseconds);
    explicit Time(int64_t nanoseconds);
    explicit Time(int nanoseconds);
    explicit Time(double seconds);
    Time(const Time& other);
    Time& operator=(const Time& other);

    /**
     * @brief Get a Time Instance that store the current time
     *
     * @param clock_type which clock source is selected?
     * @return Time
     */
    static Time Now(ClockType clock_type = ClockType::RSCL_TIME);
    static Time Now(ClockMode* clock_mode,
                    ClockType clock_type = ClockType::RSCL_TIME);
    /**
     * @brief Get a Time Instance that store the current time from steady clock
     *
     * @return Time
     */
    static Time SteadyNow();

    /**
     * @brief Sleep until target time
     * !Important, the time should be system time and not from steady clock
     *
     * @param time
     */
    static void SleepUntil(const Time& time);

    double ToSecond() const;
    uint64_t ToMillisecond() const;
    uint64_t ToMicrosecond() const;
    uint64_t ToNanosecond() const;
    bool IsZero() const;

 private:
    uint64_t nanoseconds_ = 0;
};

// ms level resolution
inline uint64_t CoarseSteadyClockNs() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

// ms level resolution
inline uint64_t CoarseSystemClockNs() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME_COARSE, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

}  // namespace time
}  // namespace base
}  // namespace senseAD
