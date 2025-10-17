/*
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@sensetime.com>
 */

#pragma once

#ifdef USE_NVTX
#define CUDA_ENABLED
#endif

#include <cstdint>
#include <chrono>
#include <vector>
#include <array>
#include <mutex>
#include <memory>
#include <map>
#include <atomic>
#include <string>
#include <functional>
#include <utility>
#include <unordered_map>
#ifdef CUDA_ENABLED
#include <cuda_runtime.h>
#endif

namespace senseAD {
namespace profiler {

class BaseTimer {
 public:
    virtual ~BaseTimer() = default;
    /**
     * @brief Start the Timer
     */
    virtual int64_t Start() = 0;
    /**
     * @brief Start the Timer
     */
    virtual int64_t Observe() const = 0;
    /**
     * @brief Return the time interval started from the Start()
     * Unit: ms
     */
    virtual double CountDuration() const = 0;
};

/**
 * @brief Timer Class for time interval measurement
 */
class Timer : public BaseTimer {
 public:
    explicit Timer(int tid = -1);
    virtual ~Timer() = default;
    int64_t Start() final;
    int64_t Observe() const final;
    double CountDuration() const final;

 private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint begin_time_;
    int tid_;
};

/**
 * @brief Timer Class for cpu time interval measurement
 */
class CpuTimer : public BaseTimer {
 public:
    explicit CpuTimer(int tid = -1);
    int64_t Start() final;
    int64_t Observe() const final;
    double CountDuration() const final;

 private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    int64_t begin_time_;
    int tid_;
};

#ifdef CUDA_ENABLED
class CudaStreamTimer : public BaseTimer {
 public:
    virtual ~CudaStreamTimer() = default;
    explicit CudaStreamTimer(cudaStream_t stream);

    int64_t Start() final;
    /**
     * !Important: It will call sync internally
     */
    double CountDuration() const final;

 private:
    cudaStream_t target_stream_;
    cudaEvent_t begin_, end_;
};
#endif

/**
 * @brief Memory Usage Info. Unit: KB
 */
struct RssInfo {
    // Virtual memory
    double vm_size;
    // Actual used memory
    double resident;
    double shared;
};

#ifndef __QNXNTO__
class ResourceProfiler {
 public:
    /**
     * @brief Get the Current Memory Usage Info
     *
     * @return RssInfo
     */
    static RssInfo GetCurrentRss();
    /**
     * @brief Get the Processor ID for current thread
     *
     * @return uint8_t
     */
    static uint8_t GetCpuId();
};
#endif

// Unit: ns
struct ProfilingStat {
 public:
    double min_ms = 0;
    double max_ms = 0;
    double mean_ms = 0;
    double variance = 0;
    uint64_t sample_cnt = 0;

    void AddSample(double measure_ms);
    void Print() const;
};

}  // namespace profiler
}  // namespace senseAD

//******* Below macro is useless, just keep for compatiable *******
#define TIME_PROFILE_RANGE(name)
#define TIME_PROFILE_PUSH(name)
#define TIME_PROFILE_PUSH_UNIQUE()
#define TIME_PROFILE_POP_0_ARGS()
#define TIME_PROFILE_POP_1_ARGS(name)
#define TIME_PROFILE_POP(...)
#define TIME_PROFILE_RANGE_STR(name)
#define TIME_PROFILE_PUSH_STR(name)
#define TIME_PROFILE_POP_STR(...)
#define MSG_PROFILE_PUSH(name, msg)
#define MSG_PROFILE_POP(name)
#define MSG_PROFILE_TAG(name, msg)

#define TIME_PROFILE_CUDA_RANGE(name)
#define TIME_PROFILE_CUDA_PUSH(name)
#define TIME_PROFILE_CUDA_PUSH_UNIQUE()
//*******************************************************************

// https://github.com/facebook/folly/tree/main/folly/tracing
