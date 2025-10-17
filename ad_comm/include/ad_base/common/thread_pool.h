/**
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung<fengzhongjie@senseauto.com>
 */

#pragma once

#include <sys/resource.h>
#include <unistd.h>
#include <cstdint>

#include <thread>
#include <string>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <future>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace base {
namespace common {

enum class QueueFullBehavior {
    REJECT_NEW,   // Reject new task
    DROP_OLDEST,  // Drop the oldest task
};

class ThreadPool {
 public:
    ThreadPool(const uint32_t thread_num,
               const uint32_t max_queue_size,
               const QueueFullBehavior behavior = QueueFullBehavior::REJECT_NEW,
               const std::string& threadname_prefix = "");
    ~ThreadPool();

    template <typename F, typename... Args>
    std::future<std::result_of_t<F(Args...)>> Enqueue(F&& f, Args&&... args);

    // Use this function if you never call get() for the return value
    template <typename F, typename... Args>
    bool EnqueueDetach(F&& f, Args&&... args);

    uint32_t ThreadNum() const;
    uint32_t TaskNum();
    inline uint32_t DiscardedTasks() const { return discarded_tasks_; }

    std::string ScheduleInfoStr();
    uint32_t GetTaskExecutions() const { return thread_executions_[0]; }

    void RusageCollectSwitch(bool on) { enable_rusage_ = on; }

 private:
    bool running_ = true;

    uint32_t thread_num_;
    uint32_t max_queue_size_;
    std::vector<std::thread> pool_;

    std::condition_variable cv_;

    std::mutex tasks_mtx_;
    std::queue<std::function<void()>> tasks_;

    QueueFullBehavior behavior_;
    std::atomic<uint32_t> discarded_tasks_{0};

    std::mutex info_mtx_;
    bool enable_rusage_ = false;
    std::vector<struct rusage> thread_usage_;
    std::vector<struct rusage> usage_before_;
    std::vector<struct rusage> usage_after_;
    std::vector<uint32_t> thread_executions_;
};

template <typename F, typename... Args>
std::future<std::result_of_t<F(Args...)>> ThreadPool::Enqueue(F&& f,
                                                              Args&&... args) {
    using return_type = std::result_of_t<F(Args...)>;
    std::lock_guard<std::mutex> lock(tasks_mtx_);
    if (tasks_.size() >= max_queue_size_) {
        switch (behavior_) {
            case QueueFullBehavior::REJECT_NEW:
                return std::future<return_type>();
            case QueueFullBehavior::DROP_OLDEST:
                tasks_.pop();
                ++discarded_tasks_;
                break;
        }
        AD_LERROR_EVERY(ThreadPool, 100)
            << "Task queue is full, discard the task";
        return std::future<return_type>();
    }
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();

    tasks_.emplace([task]() -> void { (*task)(); });
    cv_.notify_one();

    return res;
}

template <typename F, typename... Args>
bool ThreadPool::EnqueueDetach(F&& f, Args&&... args) {
    using return_type = std::result_of_t<F(Args...)>;
    std::lock_guard<std::mutex> lock(tasks_mtx_);
    if (tasks_.size() >= max_queue_size_) {
        AD_LERROR_EVERY(ThreadPool, 100)
            << "Task queue is full, discard the task";
        return false;
    }

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    tasks_.emplace([task]() -> void {
        std::future<return_type> res = task->get_future();
        (*task)();
        (void)res.get();
    });

    cv_.notify_one();
    return true;
}
}  // namespace common
}  // namespace base
}  // namespace senseAD
