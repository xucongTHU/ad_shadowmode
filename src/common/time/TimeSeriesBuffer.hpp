//
// Created by xucong on 25-2-12.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef TIME_SERIES_BUFFER_HPP
#define TIME_SERIES_BUFFER_HPP

#include <boost/circular_buffer.hpp>
#include <boost/core/noncopyable.hpp>
#include <functional>
#include "common/time/Timer.h"

namespace shadow {
namespace common {

template <typename _T>
class TimeSeriesBuffer : private boost::noncopyable {
 public:
  using GetTime = std::function<Time(_T)>;
  TimeSeriesBuffer(const size_t& capacity, GetTime&& func)
      : buffer_(capacity), getTime(std::forward<GetTime>(func)) {}
  ~TimeSeriesBuffer() = default;

  inline bool push(const _T& data) {
    if (!buffer_.empty() && getTime(buffer_.back()) > getTime(data)) {
      return false;
    }
    buffer_.push_back(data);
    return true;
  }

  inline bool pop() {
    if (buffer_.empty()) {
      return false;
    }
    buffer_.pop_front();
    return true;
  }

  // find left and right for interpolation, where left <= data <= right
  // bool between(const _T& data, _T* left, _T* right) const {
  bool between(const Time& time, _T* const left, _T* const right) const {  // PRQA S 6006
    auto iter =
        std::lower_bound(buffer_.begin(), buffer_.end(), time,
                         [this](const _T& data, const Time& time) { return getTime(data) < time; });
    assert(iter == buffer_.end() || time <= getTime(*iter));
    if (iter == buffer_.end()) {
      return false;
    }
    if (time == getTime(*iter)) {
      if (std::distance(iter, buffer_.end()) > 1) {
        *left = *iter;
        *right = *(iter + 1);
        return true;
      } else if (std::distance(buffer_.begin(), iter) > 0) {
        *left = *(iter - 1);
        *right = *iter;
        return true;
      }
      return false;
    }
    if (std::distance(buffer_.begin(), iter) > 0) {
      *left = *(iter - 1);
      *right = *iter;
      return true;
    }
    return false;
  }

  inline size_t size() const noexcept { return buffer_.size(); }

  inline _T& at(const size_t& index) { return buffer_.at(index); }

  inline const _T& at(size_t index) const { return buffer_.at(index); }

  inline bool empty() const noexcept { return buffer_.empty(); }

  inline void clear() noexcept { buffer_.clear(); }

  _T& front() { return buffer_.front(); }

  _T& back() { return buffer_.back(); }

  const _T& front() const { return buffer_.front(); }

  const _T& back() const { return buffer_.back(); }

  Time frontTime() const { return getTime(buffer_.front()); }

  Time backTime() const { return getTime(buffer_.back()); }

 private:
  boost::circular_buffer<_T> buffer_;
  GetTime getTime;
};

}  // namespace common
}  // namespace shadow

#endif //TIME_SERIES_BUFFER_HPP
