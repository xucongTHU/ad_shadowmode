/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <string>

#include "ad_base/common/macros.h"
#include "ad_base/common/listener.h"

namespace senseAD {
namespace base {
namespace time {

enum class ClockType {
    // System time, directly from the system clock
    SYSTEM_TIME,
    // Mock time or Our custom time source if we set.
    // Otherwise, fallback to system time
    RSCL_TIME
};

/**
 * @brief The internal clock mode priority
 *
 */
enum class ClockMode { SYSTEM_TIME, MOCK_TIME, CUSTOM_TIME };

/**
 * @brief ClockSource Interface
 *
 */
class ClockSource {
 public:
    virtual ~ClockSource() = default;
    virtual uint64_t NowInNanosecond() const = 0;
};

/**
 * @brief A Simple Mock Clock Implementation
 *
 */
class SimClockSource : public ClockSource {
 public:
    virtual ~SimClockSource() = default;
    uint64_t NowInNanosecond() const override;
    static void SetNow(const uint64_t& ns);

    /**
     * @brief Add a listener that will be trigger when sim time updated
     *
     * @param cb The listener function
     * @return common::ListenerHandle::Ptr A handle to auto unregister the
     * callback when it destruct
     */
    static common::ListenerHandle::Ptr AddOnSimTimeUpdateListener(
        std::function<void(const uint64_t)> cb);

    static common::ListenerHandle::Ptr AddOnSimTimeBackListener(
        std::function<void(const uint64_t, uint64_t)> cb);

    // Just for backward-compatiablity
    static std::string kSimClockChannel;

 private:
    std::atomic<uint64_t> now_ns_;

    std::mutex update_mtx_;
    std::mutex back_mtx_;
    std::unordered_map<uint64_t, std::function<void(uint64_t)>>
        time_update_cbs_;
    std::unordered_map<uint64_t, std::function<void(uint64_t, uint64_t)>>
        time_back_cbs_;

    DECLARE_AD_SINGLETON(SimClockSource)
};

/**
 * @brief Clock to get time. Not expect user call this directly.
 * User should call Time in general case.
 *
 */
class Clock {
 public:
    static uint64_t NowInNanosecond(const ClockType type);
    static uint64_t NowInNanosecond(const ClockType type,
                                    ClockMode* clock_mode);
    static uint64_t MonoNowInNanosecond();
    static void SetMode(ClockMode mode);
    static void SetClockSource(std::unique_ptr<ClockSource> source);
    static ClockMode GetMode();

    /**
     * @brief Add a listener that will be trigger when clock mode changed
     *
     * @param cb The listener function
     * @return common::ListenerHandle::Ptr A handle to auto unregister the
     * callback when it destruct
     */
    static common::ListenerHandle::Ptr AddOnClockChangeListener(
        std::function<void(const ClockMode)> cb);

 private:
    void TriggerOnClockChange();

    ClockMode mode_ = ClockMode::SYSTEM_TIME;
    std::unique_ptr<ClockSource> custom_clock_;

    std::mutex change_mtx_;
    std::unordered_map<uint64_t, std::function<void(ClockMode)>> cbs_;

    DECLARE_AD_SINGLETON(Clock)
};

/**
 * @brief Exception when using the custom clock in wrong way
 *
 */
class CustomClockException : public std::exception {
 public:
    explicit CustomClockException(std::string const& message) : msg_(message) {}
    virtual char const* what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

}  // namespace time
}  // namespace base
}  // namespace senseAD
