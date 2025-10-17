/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Gongshusheng <gongshusheng@senseauto.com>
 */
#pragma once
#include <memory>
#include <string>
#include <functional>

namespace senseAD {
namespace ad_task {

// Note:初始设计Task接口为单例+静态函数,但是没法处理任务生命周期问题
// 所以改成普通对象,管理任务生命周期
class Task : public std::enable_shared_from_this<Task> {
 public:
    using TaskFunc = std::function<void()>;
    enum class TimerMode { Once, Repeat };

    explicit Task(const std::string &mode = "default",
                  const std::string &config = "");

    ~Task();

    // for task
    bool AddShortTask(const TaskFunc &func, const std::string &func_name = "");

    int32_t AddPersistentTask(const TaskFunc &func,
                              const std::string &func_name = "");
    void Join(uint32_t thread_index);

    // for timer
    int32_t CreateTimer(uint32_t timeout_ms,
                        const TaskFunc &func,
                        const TimerMode &mode = TimerMode::Repeat);
    void ResetTimer(uint32_t index,
                    uint32_t timeout_ms,
                    const TaskFunc &func,
                    const TimerMode &mode = TimerMode::Repeat);
    void DeleteTimer(uint32_t index);

    // for underlay impl
    void *GetNativeHandle();
};
}  // namespace ad_task

}  // namespace senseAD
