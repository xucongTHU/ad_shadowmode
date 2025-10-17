/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ad_mw/task.h"
#include "ad_mw/time.h"
#include "ad_mw_cyber/comm_handle.h"
#include "cyber/cyber.h"

namespace senseAD {
namespace mw {
namespace task {

template <typename F, typename... Args>
std::future<typename std::result_of<F(Args...)>::type> Async(F&& f,
                                                             Args&&... args) {
    return apollo::cyber::Async(std::forward<F>(f),
                                std::forward<Args>(args)...);
}

inline void Yield() { return apollo::cyber::Yield(); }

template <typename Rep, typename Period>
void SleepFor(const std::chrono::duration<Rep, Period>& sleep_duration) {
    return apollo::cyber::SleepFor(sleep_duration);
}

struct MessageSyncedCallbackTaskInnerHandle {
    std::string task_name;
    std::vector<apollo::cyber::data::VisitorConfig> config_list;
};

#define DECLARE_MessageSyncedCallbackTask_FN(...)                           \
    static std::unique_ptr<MessageSyncedCallbackTask<__VA_ARGS__>> New(     \
        const std::string& task_name,                                       \
        const std::vector<senseAD::mw::comm::SubscriberBase*>& subscribers, \
        const CallbackFunc& cb) {                                           \
        return std::make_unique<MessageSyncedCallbackTask<__VA_ARGS__>>(    \
            task_name, subscribers, cb);                                    \
    }                                                                       \
    MessageSyncedCallbackTask(                                              \
        const std::string& task_name,                                       \
        const std::vector<senseAD::mw::comm::SubscriberBase*>& subscribers, \
        const CallbackFunc& cb);                                            \
    ~MessageSyncedCallbackTask() { Stop(); }                                \
    void Start();                                                           \
    void Stop()

inline MessageSyncedCallbackTaskInnerHandle*
ToMessageSyncedCallbackTaskInnerHandle(void* ptr) {
    return reinterpret_cast<MessageSyncedCallbackTaskInnerHandle*>(ptr);
}

template <typename M0>
class MessageSyncedCallbackTask<M0, NullType, NullType, NullType>
    : public WrapperdPtrInnerHandleU {
 public:
    using CallbackFunc = std::function<void(const std::shared_ptr<M0>&)>;

    DECLARE_MessageSyncedCallbackTask_FN(M0);

 private:
    CallbackFunc cb_;
};

template <typename M0, typename M1>
class MessageSyncedCallbackTask<M0, M1, NullType, NullType>
    : public WrapperdPtrInnerHandleU {
 public:
    using CallbackFunc = std::function<void(const std::shared_ptr<M0>&,
                                            const std::shared_ptr<M1>&)>;

    DECLARE_MessageSyncedCallbackTask_FN(M0, M1);

 private:
    CallbackFunc cb_;
};

template <typename M0, typename M1, typename M2>
class MessageSyncedCallbackTask<M0, M1, M2, NullType>
    : public WrapperdPtrInnerHandleU {
 public:
    using CallbackFunc = std::function<void(const std::shared_ptr<M0>&,
                                            const std::shared_ptr<M1>&,
                                            const std::shared_ptr<M2>&)>;

    DECLARE_MessageSyncedCallbackTask_FN(M0, M1, M2);

 private:
    CallbackFunc cb_;
};

}  // namespace task
}  // namespace mw
}  // namespace senseAD

#include "ad_mw_cyber/task_borrowed_rawmessage.h"
