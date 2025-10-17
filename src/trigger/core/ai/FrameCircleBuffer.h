/*******************************************************************
* Copyright (c) 2025 T3CAIC. All rights reserved.
*
* @file FrameCircleBuffer.h
* @brief 基于boost的循环buffer
*
* @author maqiang
* @date 2025-06
*******************************************************************/
#pragma once

#include <boost/circular_buffer.hpp>
#include <boost/core/noncopyable.hpp>
#include <functional>

#include "common/time/Timer.h"

namespace shadow::trigger {

template <typename _T>
class FrameCircleBuffer : private boost::noncopyable {
public:
  using GetTime = std::function<common::Time(_T)>;

  /**
   * @brief 构造函数
   * @param capacity buffer的最大容量
   * @param func 获取时间戳函数
   */
  FrameCircleBuffer(const size_t& capacity, GetTime&& func)
      : buffer_(capacity), getTime(std::forward<GetTime>(func)) {}
  ~FrameCircleBuffer() = default;

  /**
   * @brief 数据添加到buffer
   * @param data 数据
   * @return 是否成功
   */
  inline bool push(const _T& data) {
    if (!buffer_.empty() && getTime(buffer_.back()) > getTime(data)) {
      return false;
    }
    buffer_.push_back(data);
    return true;
  }

  /**
   * @brief 取最早的数据
   * @return 是否成功
   */
  inline bool pop() {
    if (buffer_.empty()) {
      return false;
    }
    buffer_.pop_front();
    return true;
  }

  /**
   * @brief 获取循环buffer的大小
   * @return 返回当前buffer的大小
   */
  inline size_t size() const noexcept { return buffer_.size(); }

  /**
   * @brief 获取第index个数据
   * @param index index
   * @return 数据
   */
  inline _T& at(const size_t& index) { return buffer_.at(index); }

  /**
   * @brief 获取第index个数据
   * @param index index
   * @return 数据
   */
  inline const _T& at(size_t index) const { return buffer_.at(index); }

  /**
   * @brief buffer是否为空
   * @return 是否空
   */
  inline bool empty() const noexcept { return buffer_.empty(); }

  /**
   * @brief 清空循环buffer
   */
  inline void clear() noexcept { buffer_.clear(); }

  /**
   * @brief 获取buffer中最早添加的数据
   * @return 数据
   */
  _T& front() { return buffer_.front(); }
  const _T& front() const { return buffer_.front(); }

  /**
   * @brief 获取buffer中最晚添加的数据
   * @return 数据
   */
  _T& back() { return buffer_.back(); }
  const _T& back() const { return buffer_.back(); }

  /**
   * @brief 获取buffer中最早添加的数据的时间戳
   * @return 时间戳
   */
  common::Time frontTime() const { return getTime(buffer_.front()); }

  /**
   * @brief 获取buffer中最晚添加的数据的时间戳
   * @return 时间戳
   */
  common::Time backTime() const { return getTime(buffer_.back()); }

 private:
  boost::circular_buffer<_T> buffer_;  // 循环buffer
  GetTime getTime;                     // 获取时间戳函数
};

}  // namespace shadow::trigger