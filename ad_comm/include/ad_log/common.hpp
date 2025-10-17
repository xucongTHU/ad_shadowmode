/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * LiangYu <liangyu@sensetime.com>
 */
#pragma once
#include <unistd.h>
#include <sys/types.h>

#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <functional>

namespace senseAD {

using LogFunc = std::function<void(const char *)>;

struct LogConfig {
    bool color = true;
    bool simple_file = true;
    bool rotating_file = false;
    bool daily_file = false;
    bool async = false;
    bool create_dir = false;
    uint64_t max_size = 1024;
    uint32_t max_files = 3;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t async_thread_nums = 1;
    uint32_t async_queue_length = 1024;
    uint8_t async_overflow_policy = 0;
    uint8_t tostderr = 0;
    std::string severity = "info";
    std::string path = "/tmp";
    std::string pattern = "%D %X [%l%$] %v";
    bool append_mode = false;
};

std::string TimePidString();
template <typename T>
inline T GetEnv(const char *name, T default_value) {
    char *p = getenv(name);
    if (p) {
        std::string str(p);
        std::istringstream iss(str);
        T ret;
        iss >> ret;
        return ret;
    } else {
        return default_value;
    }
}

template <typename T,
          typename U = typename std::enable_if<
              std::is_const<T>::value,
              typename std::remove_const<T>::type>::type>
inline U GetEnv(const char *name, U default_value) {
    char *p = getenv(name);
    if (p) {
        std::string str(p);
        std::istringstream iss(str);
        U ret;
        iss >> ret;
        return ret;
    } else {
        return default_value;
    }
}

class StartLogTime {
 public:
    StartLogTime(const StartLogTime &) = delete;
    StartLogTime &operator=(const StartLogTime &) = delete;
    static StartLogTime &Instance();
    std::string GetStartTime() { return startTime_; }

 private:
    void SetStartTime(std::string startTime) { this->startTime_ = startTime; }
    StartLogTime();
    std::string startTime_;
};

}  // namespace senseAD
