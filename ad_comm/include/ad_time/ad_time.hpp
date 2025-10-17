/*
 * Copyright (C) 2020 by SenseTime Group Limited. All rights reserved.
 * Guo Zhichong <guozhichong@sensetime.com>
 */

#pragma once

#include <climits>  // LONG_MAX
#include <cmath>    // std::floor/std::round
#include <string>

#include "ad_log/ad_log.hpp"
#include "ad_common/data_type/base.hpp"

#ifndef NDEBUG
#define TASSERT(condition, message)                                  \
    do {                                                             \
        if (!(condition)) {                                          \
            AD_LERROR(TIME)                                          \
                << "Assertion `" #condition "` failed: " << message; \
        }                                                            \
    } while (false)
#else
#include <cassert>  // assert

#define TASSERT(condition, message) \
    do {                            \
        assert((condition));        \
    } while (false)
#endif

namespace senseAD {

inline void NormalizeTime(uint32_t &sec32, uint32_t &nsec32) {  // NOLINT
    uint64_t nsec64 = nsec32;
    sec32 += (nsec64 / 1000000000L);
    nsec32 = (nsec64 % 1000000000L);
}

struct Time {
    Time() : sec(0), nsec(0) {}

    /*
     * @brief Construction from nanoseconds
     */
    explicit Time(const uint64_t &nanosec) { FromNSec(nanosec); }

    /*
     * @brief Construction from sec and nanoseconds
     */
    Time(const uint32_t &_sec, const uint32_t &_nsec) : sec(_sec), nsec(_nsec) {
        NormalizeTime(sec, nsec);
    }

    /*
     * @brief from nanoseconds
     */
    void FromNSec(const uint64_t &nanosec) {
        sec = static_cast<uint32_t>(nanosec / 1000000000L);
        nsec = static_cast<uint32_t>(nanosec % 1000000000L);
        NormalizeTime(sec, nsec);
    }

    /*
     * @brief from seconds
     */
    void FromSec(const double &time) {
        sec = static_cast<uint32_t>(std::floor(time));
        nsec = static_cast<uint32_t>(std::round((time - sec) * 1e9));
    }

    /*
     * @brief Convert to nanoseconds
     */
    uint64_t ToNSec() const {
        return static_cast<uint64_t>(sec) * 1000000000L +
               static_cast<uint64_t>(nsec);
    }

    /*
     * @brief Convert to seconds
     */
    double ToSec() const {
        return static_cast<double>(sec) + static_cast<double>(nsec) * 1e-9;
    }

    /*
     * @brief Convert to string "%Y-%m-%d-%H-%M-%S"
     */
    std::string ToString() const { return ToString(ToNSec()); }
    /*
     * @brief Convert to string "%Y-%m-%d-%H-%M-%S"
     */
    static std::string ToString(const uint64_t &ns) {
        timeval tv;
        tv.tv_sec = static_cast<decltype(tv.tv_sec)>(ns / 1e9);
        tv.tv_usec =
            static_cast<decltype(tv.tv_usec)>(ns / 1e3 - tv.tv_sec * 1e6);
        unsigned int ms = tv.tv_usec / 1000;
        time_t time(tv.tv_sec);
        struct tm stm;
        localtime_r(&time, &stm);
        char buffer[128];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &stm);
        char res[128];
        snprintf(res, sizeof(res), "%s.%03d", buffer, ms);
        return std::string(res);
    }

    Time operator+(const Time &rhs) const {
        uint64_t sec64 =
            static_cast<uint64_t>(this->sec) + static_cast<uint64_t>(rhs.sec);
        uint64_t nsec64 =
            static_cast<uint64_t>(this->nsec) + static_cast<uint64_t>(rhs.nsec);
        sec64 += (nsec64 / 1000000000L);
        nsec64 = (nsec64 % 1000000000L);
        TASSERT(sec64 < LONG_MAX, "Time out of range int32_t");
        TASSERT(nsec64 < LONG_MAX, "Time out of range int32_t");
        return Time(static_cast<uint32_t>(sec64),
                    static_cast<uint32_t>(nsec64));
    }

    Time &operator+=(const Time &rhs) {
        *this = *this + rhs;
        return *this;
    }

    Time operator-(const Time &rhs) const {
        TASSERT(this->sec >= rhs.sec,
                "Time subtraction require t0.sec >= t1.sec.");
        int64_t sec64 =
            static_cast<int64_t>(this->sec) - static_cast<int64_t>(rhs.sec);
        int64_t nsec64 =
            static_cast<int64_t>(this->nsec) - static_cast<int64_t>(rhs.nsec);
        if (sec64 <= 0) {
            TASSERT(nsec64 >= 0,
                    "Time subtraction requires t0.nsec >= t1.nsec "
                    " while t0.sec <= t1.sec");
        }
        if (nsec64 < 0) {
            TASSERT(sec64 > 0,
                    "Time subtraction requires t0.sec - t1.sec > 0"
                    " while t0.nsec < t1.nsec");
            sec64 -= 1;
            nsec64 += 1000000000L;
        }
        return Time(static_cast<uint32_t>(sec64),
                    static_cast<uint32_t>(nsec64));
    }

    Time &operator-=(const Time &rhs) {
        *this = *this - rhs;
        return *this;
    }

    bool operator>(const Time &rhs) const {
        if (this->sec > rhs.sec) {
            return true;
        } else if (this->sec < rhs.sec) {
            return false;
        } else {
            return this->nsec > rhs.nsec;
        }
    }

    bool operator>=(const Time &rhs) {
        if (this->sec > rhs.sec) {
            return true;
        } else if (this->sec < rhs.sec) {
            return false;
        } else {
            return this->nsec >= rhs.nsec;
        }
    }

    bool operator<(const Time &rhs) const {
        if (this->sec < rhs.sec) {
            return true;
        } else if (this->sec > rhs.sec) {
            return false;
        } else {
            return this->nsec < rhs.nsec;
        }
    }
    bool operator<=(const Time &rhs) const {
        if (this->sec < rhs.sec) {
            return true;
        } else if (this->sec > rhs.sec) {
            return false;
        } else {
            return this->nsec <= rhs.nsec;
        }
    }

    bool operator==(const Time &rhs) const {
        return ((this->sec == rhs.sec) && (this->nsec == rhs.nsec));
    }

    bool operator!=(const Time &rhs) const { return !((*this) == rhs); }

    uint32_t sec;
    uint32_t nsec;
};

inline std::ostream &operator<<(std::ostream &os, const Time &rhs) {
    os << rhs.sec << "." << rhs.nsec;
    return os;
}

namespace time {
template <class T>
using duration = std::chrono::duration<T>;
using nanoseconds = std::chrono::nanoseconds;
using microseconds = std::chrono::microseconds;
using milliseconds = std::chrono::milliseconds;
using seconds = std::chrono::seconds;
using minutes = std::chrono::minutes;
using hours = std::chrono::hours;
using timepoint = std::chrono::time_point<std::chrono::system_clock>;

/*
 * @brief Return current time, maybe a virtual clock when in double speed
 * running
 */
Time Now();
// For backward compatiable
Time NativeNow();

//! **** DEPRECATED. Useless at now ****
class ClockInterface {
 public:
    virtual ~ClockInterface() {}

    virtual Time Now() = 0;
};
inline void SetGlobalClock(ClockInterface *) {}
//***************************************

}  // namespace time

}  // namespace senseAD
