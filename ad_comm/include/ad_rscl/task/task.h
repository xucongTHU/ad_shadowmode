/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <chrono>
#include <future>
#include <memory>
#include <utility>
#include <mutex>
#include <string>

#include "ad_mw/ad_mw.h"
#include "ad_rscl/time/time.h"
#include "ad_base/common/shutdownable.h"
#include "ad_rscl/common/interface.h"

namespace senseAD {
namespace rscl {
namespace task {

using senseAD::rscl::common::Suspendable;

/**
 * @brief Run a task asynchronously without blocking current thread
 *
 * @tparam F The task Signature
 * @tparam Args Arguments Signature
 * @param f the instance of the task function pointer
 * @param args Arguments for the task
 * @return std::future<typename std::result_of<F(Args...)>::type> The handle to
 * receive the task result
 */
template <typename F, typename... Args>
static auto Async(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
    return senseAD::mw::task::Async(std::forward<F>(f),
                                    std::forward<Args>(args)...);
}

/**
 * @brief Give out the cpu control
 *
 */
static inline void Yield() { senseAD::mw::task::Yield(); }

/**
 * @brief Sleep for a period of time
 *
 * @tparam Rep
 * @tparam Period
 * @param sleep_duration sleep period
 */
template <typename Rep, typename Period>
static void SleepFor(const std::chrono::duration<Rep, Period>& sleep_duration) {
    senseAD::mw::task::SleepFor(sleep_duration);
}

class TimerTaskParam {
 public:
    /**
     * @brief Construct a new TimerTaskParam object
     *
     * @param period_ms The period
     * @param callback callback function
     * @param sys_only True of run only once
     */
    TimerTaskParam(uint32_t period_ms,
                   std::function<void()> callback,
                   bool sys_only = false)
        : period_ms_(period_ms),
          callback_(callback),
          sys_only_(sys_only),
          thread_name_set_(false) {}

    /**
     * @brief Construct a new TimerTaskParam object
     *
     * @param period_ms The period
     * @param callback callback function pointer
     * @param obj callback function object
     * @param sys_only True of run only once
     */
    template <typename T>
    TimerTaskParam(uint32_t period_ms,
                   void (T::*callback)(),
                   T* obj,
                   bool sys_only = false)
        : TimerTaskParam(period_ms, std::bind(callback, obj), sys_only) {}

    /**
     * @brief Construct a new TimerTaskParam object
     *
     * @param period_ms The period
     * @param callback const callback function pointer
     * @param obj callback function object
     * @param sys_only True of run only once
     */
    template <typename T>
    TimerTaskParam(uint32_t period_ms,
                   void (T::*callback)() const,
                   T* obj,
                   bool sys_only = false)
        : TimerTaskParam(period_ms, std::bind(callback, obj), sys_only) {}

    void SetThreadNameIfSupport(const std::string& thread_name);
    void SetRateErrorHandler(std::function<void()>);

    /**
     * @brief Set rate sleep error log display with tag, no tag no log
     */
    void SetRateErrorLogTag(const std::string& rate_tag);

    uint32_t GetPeriodMs() const;

    std::function<void()> GetCallback() const;
    std::function<void()> GetRateErrorHandler() const;

    bool GetSysOnly() const;

    bool GetThreadNameSet() const;

    std::string GetThreadName() const;

    std::string GetRateErrorTag() const;

 private:
    uint32_t period_ms_;
    std::function<void()> callback_;
    std::function<void()> rate_error_handler_;
    bool sys_only_;
    bool thread_name_set_;
    std::string thread_name_{};
    std::string rate_error_tag_{};
};

class ManualTask : public ::Shutdownable, Suspendable {
 public:
    explicit ManualTask(const TimerTaskParam& param);
    // For compatiable
    ManualTask(uint32_t period_ms,
               std::function<void()> task,
               const std::string& thread_name = "");
    ~ManualTask() override;

    void ShutdownImpl() override;
    void Enable();
    void Disable();
    void Trigger(bool force = false);

    bool Suspend() override;
    bool Resume() override;

 private:
    void Init();

    TimerTaskParam task_param_;

    std::mutex m_;
    bool enabled_{false};
    std::atomic<int32_t> trigger_count_{0};
    std::condition_variable cv_;
    std::thread inner_thread_;

    ListenerHandle::Ptr sim_time_change_listener_handle_;
    uint64_t last_trigger_ns_ = 0;

    std::atomic<bool> task_shutdown_{false};
    // For suspend/resume. Avoid Start() when Resume() if it is not runnning
    // when suspend
    bool suspended_{false};
};

/**
 * @brief Create a periodic task that run in background
 *
 */
class TimerTask : public ::Shutdownable, Suspendable {
 public:
    /**
     * @brief Construct a new Timer Task object
     *
     * @param param TimerTaskParam object
     */
    explicit TimerTask(const TimerTaskParam& param);
    TimerTask(uint32_t period_ms,
              std::function<void()> callback,
              bool sys_only = false)
        : TimerTask(TimerTaskParam(period_ms, callback, sys_only)) {}

    template <typename T>
    TimerTask(uint32_t period_ms,
              void (T::*callback)(),
              T* obj,
              bool sys_only = false)
        : TimerTask(
              TimerTaskParam(period_ms, std::bind(callback, obj), sys_only)) {}

    template <typename T>
    TimerTask(uint32_t period_ms,
              void (T::*callback)() const,
              T* obj,
              bool sys_only = false)
        : TimerTask(
              TimerTaskParam(period_ms, std::bind(callback, obj), sys_only)) {}

    ~TimerTask() override;
    void ShutdownImpl() override;
    void Start();
    void Stop();
    void ManualTrigger();

    bool Suspend() override;
    bool Resume() override;

 private:
    void HandleClockModeChange(const senseAD::rscl::time::ClockMode mode);
    void InitNonSysImpl();

    TimerTaskParam param_;

    std::mutex mode_mtx_;
    bool enabled_ = false;
    bool sys_mode_ = true;

    // Only for Sys Mode
    std::unique_ptr<senseAD::mw::task::TimerTask> sys_impl_ = nullptr;
    // Only For Non Sys mode
    std::unique_ptr<ManualTask> nonsys_impl_ = nullptr;

    ListenerHandle::Ptr clock_change_listener_handle_{nullptr};

    // For suspend/resume. Avoid Start() when Resume() if it is not runnning
    // when suspend
    bool suspended_{false};
};

/**
 * @brief An simple implementation of timer task when the underlying mw have no
 * specical support
 *
 */
class SimpleTimerTaskImpl final : public senseAD::mw::task::TimerTask,
                                  public ::Shutdownable,
                                  public Suspendable {
 public:
    // do we really need oneshot?
    SimpleTimerTaskImpl(const TimerTaskParam& param, bool oneshot);

    // deprecated, saved for backwards compatable
    SimpleTimerTaskImpl(uint32_t period_ms,
                        std::function<void()> callback,
                        bool oneshot,
                        const std::string& thread_name = "");
    ~SimpleTimerTaskImpl() final;

    void Shutdown() override;
    void ShutdownImpl() override;
    void Start() final;
    void Stop() final;

    bool Suspend() override;
    bool Resume() override;

 private:
    bool oneshot_;
    TimerTaskParam task_param_;

    bool running_ = false;
    std::thread timer_thread_;

    // For suspend/resume. Avoid Start() when Resume() if it is not runnning
    // when suspend
    bool suspended_{false};
};

}  // namespace task
}  // namespace rscl
}  // namespace senseAD
