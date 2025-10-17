/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <chrono>
#include <future>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include "ad_mw/comm.h"
#include "ad_mw/visibility_control.h"
#include "ad_mw/wrapped_handle.h"

namespace senseAD {
namespace mw {
namespace task {

/**
 * @brief Run a function without blocking current thread
 *
 * @tparam F function signature
 * @tparam Args function arugment type
 * @param f function pointer
 * @param args function provide argument
 * @return future
 */
template <typename F, typename... Args>
ADMW_PUBLIC std::future<typename std::result_of<F(Args...)>::type> Async(
    F&& f, Args&&... args);

/**
 * @brief Give out current cpu control
 *
 */
ADMW_PUBLIC void Yield();

template <typename Rep, typename Period>
ADMW_PUBLIC void SleepFor(
    const std::chrono::duration<Rep, Period>& sleep_duration);

class ADMW_PUBLIC TimerTask {
 public:
    /**
     * @brief Create a new periodic task
     *
     * @param period_ms Interval between each execution
     * @param callback The Task
     * @param oneshot Call only once?
     * @return std::unique_ptr<TimerTask> If not support, will return nullptr
     */
    static std::unique_ptr<TimerTask> New(uint32_t period_ms,
                                          std::function<void()> callback,
                                          bool oneshot,
                                          const std::string& thread_name = "");
    TimerTask() = default;
    virtual ~TimerTask();
    /**
     * @brief Start the task
     *
     */
    inline virtual void Start() {
        if (impl_) impl_->Start();
        started_ = true;
    }
    /**
     * @brief Stop the task
     *
     */
    inline virtual void Stop() {
        if (impl_) impl_->Stop();
        started_ = false;
    }

    inline bool Started() const { return started_; }

    inline virtual void Shutdown() {
        if (impl_) impl_->Shutdown();
        started_ = false;
    }

 private:
    bool started_ = true;
    std::unique_ptr<TimerTask> impl_;
};

/**
 * @brief Sync multi channel message
 *
 * @tparam M0 either BorrowedRawMessage or NullType
 * @tparam M1 either BorrowedRawMessage or NullType
 * @tparam M2 either BorrowedRawMessage or NullType
 * @tparam M3 either BorrowedRawMessage or NullType
 */
template <typename M0 = NullType,
          typename M1 = NullType,
          typename M2 = NullType,
          typename M3 = NullType>
class ADMW_PUBLIC MessageSyncedCallbackTask : public WrapperdPtrInnerHandleU {
 public:
    using CallbackFunc = std::function<void(const std::shared_ptr<M0>&,
                                            const std::shared_ptr<M1>&,
                                            const std::shared_ptr<M2>&,
                                            const std::shared_ptr<M3>&)>;

    static std::unique_ptr<MessageSyncedCallbackTask<M0, M1, M2, M3>> New(
        const std::string& task_name,
        const std::vector<senseAD::mw::comm::SubscriberBase*>& subscribers,
        const CallbackFunc& cb);

    [[deprecated("Will be remove in future.")]] MessageSyncedCallbackTask(
        const std::string& task_name,
        const std::vector<senseAD::mw::comm::SubscriberBase*>& subscribers,
        const CallbackFunc& cb);
    ~MessageSyncedCallbackTask() = default;
    /**
     * @brief Start the task
     *
     */
    void Start();

    /**
     * @brief Stop the task
     *
     */
    void Stop();

 private:
    CallbackFunc cb_;
};

class TaskContext : public std::enable_shared_from_this<TaskContext> {
 public:
    TaskContext() : id(reinterpret_cast<uint64_t>(this)) {}
    explicit TaskContext(uint64_t identify) : id(identify) {}

    ~TaskContext() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& func : cancel_funcs_) {
            func();
        }
        cancel_funcs_.clear();
    }

    void AddCancelFunction(std::function<void()> cancel_func) {
        std::lock_guard<std::mutex> lock(mutex_);
        cancel_funcs_.push_back(std::move(cancel_func));
    }

    uint64_t id;

 private:
    std::vector<std::function<void()>> cancel_funcs_;
    mutable std::mutex mutex_;
};

class ADMW_PUBLIC Task {
 public:
    using TaskFunc = std::function<void()>;
    enum class TimerMode { Once, Repeat };
    virtual ~Task() = default;
    virtual std::string Name() = 0;
    virtual bool Init(const std::string& config = "") = 0;
    virtual bool IsInit() const = 0;
    virtual bool DeInit() = 0;

    virtual bool AddShortTask(const TaskFunc& func,
                              const std::string& func_name = "",
                              uint64_t producer = 0) = 0;
    virtual int32_t AddPersistentTask(const TaskFunc& func,
                                      const std::string& func_name = "") = 0;
    virtual void Join(uint32_t thread_index) = 0;
    virtual int32_t CreateTimer(uint32_t timeout_ms,
                                const TaskFunc& func,
                                const TimerMode& mode) = 0;
    virtual void ResetTimer(uint32_t index,
                            uint32_t timeout_ms,
                            const TaskFunc& func,
                            const TimerMode& mode) = 0;
    virtual void DeleteTimer(uint32_t index) = 0;
    virtual bool ClearTask(uint64_t producer) = 0;
    virtual bool ClearTask() = 0;
    virtual void* GetNativeHandle() = 0;

    //  protected:
};

class ADMW_PUBLIC TaskFactory {
 public:
    using Creator = std::function<std::shared_ptr<Task>()>;

    static void Register(const std::string& name, Creator creator) {
        GetRegistry()[name] = creator;
    }

    static std::shared_ptr<Task> Create(const std::string& name) {
        auto it = GetRegistry().find(name);
        if (it != GetRegistry().end()) {
            return it->second();
        }
        return {};
    }

 private:
    static std::unordered_map<std::string, Creator>& GetRegistry() {
        static std::unordered_map<std::string, Creator> registry;
        return registry;
    }
};

template <typename T>
class TaskRegistrar {
 public:
    explicit TaskRegistrar(const std::string& name) {
        TaskFactory::Register(name, []() -> std::shared_ptr<Task> {
            return std::make_shared<T>();
        });
    }
};

#define REGISTER_TASK_IMPL(name, clazz)            \
    static senseAD::mw::task::TaskRegistrar<clazz> \
        _task_impl_registrar_##name_##clazz(name);

}  // namespace task
}  // namespace mw
}  // namespace senseAD
