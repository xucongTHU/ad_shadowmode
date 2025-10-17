/**
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung<fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstdint>

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <deque>

namespace senseAD {
namespace base {
namespace common {

template <typename T>
class BoundedQueue {
 public:
    explicit BoundedQueue(size_t max_size) : max_size_(max_size) {}

    bool push(const T& value) {  // NOLINT
        bool ret = true;
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.size() >= max_size_) {
            queue_.pop_front();  // Remove the oldest element
            ret = false;
        }
        queue_.push_back(value);
        return ret;
    }

    bool pop(T& value) {  // NOLINT
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        value = queue_.front();
        queue_.pop_front();
        return true;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    inline size_t max_size() const { return max_size_; }

 private:
    std::deque<T> queue_;
    size_t max_size_;
    mutable std::mutex mutex_;
};

}  // namespace common
}  // namespace base
}  // namespace senseAD
