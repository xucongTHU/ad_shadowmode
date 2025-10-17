/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <memory>
#include <utility>
#include <thread>
#include <future>

namespace senseAD {
namespace mw_common {
namespace task {

template <typename F, typename... Args>
std::future<typename std::result_of<F(Args...)>::type> Async(F&& f,
                                                             Args&&... args) {
    return std::async(std::launch::async, std::forward<F>(f),
                      std::forward<Args>(args)...);
}

inline void Yield() { std::this_thread::yield(); }

template <typename Rep, typename Period>
void SleepFor(const std::chrono::duration<Rep, Period>& sleep_duration) {
    std::this_thread::sleep_for(sleep_duration);
}

}  // namespace task
}  // namespace mw_common
}  // namespace senseAD
