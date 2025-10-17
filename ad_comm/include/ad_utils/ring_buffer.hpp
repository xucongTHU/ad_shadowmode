/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Chen Xiaosong <chenxiaosong@senseauto.com>
 */

#pragma once

#include <condition_variable>
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <mutex>

namespace senseAD {
namespace ad_utils {

template<typename T>
class RingBuffer {
  size_t max_items_ = 0;
  typename std::vector<T>::size_type head_ = 0;
  typename std::vector<T>::size_type tail_ = 0;
  size_t overrun_counter_ = 0;
  std::vector<T> v_;
  std::shared_ptr<std::mutex> mutex_ = std::make_shared<std::mutex>();
  std::shared_ptr<std::mutex> full_mutex_ = std::make_shared<std::mutex>();
  std::shared_ptr<std::mutex> empty_mutex_ = std::make_shared<std::mutex>();
  std::shared_ptr<std::condition_variable> full_condition_ =
      std::make_shared<std::condition_variable>();
  std::shared_ptr<std::condition_variable> empty_condition_ =
      std::make_shared<std::condition_variable>();

 public:
  using value_type = T;

  explicit RingBuffer(size_t max_items = 1)
      : max_items_(max_items + 1)  // one item is reserved as marker for full q
        , v_(max_items_) {}

  RingBuffer(const RingBuffer&) = default;
  RingBuffer& operator=(const RingBuffer&) = default;

  // move cannot be default,
  // since we need to reset head_, tail_, etc to zero in the moved object
  RingBuffer(RingBuffer&& other) noexcept { copy_moveable(std::move(other)); }

  RingBuffer& operator=(RingBuffer&& other) noexcept {
    copy_moveable(std::move(other));
    return *this;
  }

  // push back, overrun (oldest) item if no room left and not wait
  void push_back(const T& item, bool wait) {
    if (max_items_ > 0) {
      if (full()) {
        if (wait) {
          std::unique_lock<std::mutex> lck(*full_mutex_);
          full_condition_->wait(lck, [&] { return !full(); });
        } else {
          pop();
          ++overrun_counter_;
        }
      }
      push(item);
      empty_condition_->notify_all();
    }
  }

  // Pop item from front.
  // If there are no elements in the container, throw exception.
  T pop_front(bool wait) {
    if (empty()) {
      if (wait) {
        std::unique_lock<std::mutex> lck(*empty_mutex_);
        empty_condition_->wait(lck, [&] { return !empty(); });
      } else {
        throw(std::string("ring buffer is empty for pop front"));
      }
    }
    auto item = pop();
    full_condition_->notify_all();
    return item;
  }

  bool empty() const { return tail_ == head_; }

  bool full() const {
    // head is ahead of the tail by 1
    if (max_items_ > 0) {
      return ((tail_ + 1) % max_items_) == head_;
    }
    return false;
  }

  size_t overrun_counter() const { return overrun_counter_; }

 private:
  void push(const T& item) {
    std::unique_lock<std::mutex> lck(*mutex_);
    v_[tail_] = item;
    tail_ = (tail_ + 1) % max_items_;
  }
  T pop() {
    std::unique_lock<std::mutex> lck(*mutex_);
    auto item = std::move(v_[head_]);
    head_ = (head_ + 1) % max_items_;
    return item;
  }
  // copy from other&& and reset it to disabled state
  void copy_moveable(RingBuffer&& other) noexcept {
    max_items_ = other.max_items_;
    head_ = other.head_;
    tail_ = other.tail_;
    overrun_counter_ = other.overrun_counter_;
    v_ = std::move(other.v_);

    // put &&other in disabled, but valid state
    other.max_items_ = 0;
    other.head_ = other.tail_ = 0;
    other.overrun_counter_ = 0;
  }
};

}  // namespace ad_utils
}  // namespace senseAD
