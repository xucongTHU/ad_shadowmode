/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * LiangYu <liangyu@sensetime.com>
 */
#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "ad_log/configuration.hpp"
#if ENABLE_FAW_LOG

#include "framework/runtime.h"
#include "ad_log/common.hpp"

namespace senseAD {

template <typename T>
void LogHelper(faw::ad::log::LogStream &os, const T &arg) {  // NOLINT
    os << arg;
}

template <typename T, typename... Args>
void LogHelper(faw::ad::log::LogStream &os,  // NOLINT
               const T &arg,
               const Args &... args) {
    os << arg;
    LogHelper(os, args...);
}

class FawLogManager {
 public:
    FawLogManager(const FawLogManager &) = delete;
    FawLogManager &operator=(const FawLogManager &) = delete;

    static FawLogManager &Instance();
    static std::shared_ptr<FawLogManager> SharedInstance();

    static inline std::string FawLogLevel2RsclLogLevel(
        faw::ad::log::LogLevel lv) {
        static std::unordered_map<uint8_t, std::string> level_map_ = {
            {static_cast<uint8_t>(faw::ad::log::LogLevel::kVerbose), "trace"},
            {static_cast<uint8_t>(faw::ad::log::LogLevel::kDebug), "debug"},
            {static_cast<uint8_t>(faw::ad::log::LogLevel::kInfo), "info"},
            {static_cast<uint8_t>(faw::ad::log::LogLevel::kWarn), "warn"},
            {static_cast<uint8_t>(faw::ad::log::LogLevel::kError), "error"},
            {static_cast<uint8_t>(faw::ad::log::LogLevel::kFatal), "critical"},
            {static_cast<uint8_t>(faw::ad::log::LogLevel::kOff), "off"}};
        return level_map_[static_cast<uint8_t>(lv)];
    }
    static inline faw::ad::log::LogLevel RsclLogLevel2FawLogLevel(
        const char *lv) {
        static std::unordered_map<std::string, faw::ad::log::LogLevel>
            level_map_ = {{"trace", faw::ad::log::LogLevel::kVerbose},
                          {"debug", faw::ad::log::LogLevel::kDebug},
                          {"info", faw::ad::log::LogLevel::kInfo},
                          {"warn", faw::ad::log::LogLevel::kWarn},
                          {"error", faw::ad::log::LogLevel::kError},
                          {"critical", faw::ad::log::LogLevel::kFatal},
                          {"off", faw::ad::log::LogLevel::kOff}};
        return level_map_[lv];
    }
    void RegistLogger(faw::ad::framework::LoggerT &logger);  // NOLINT
    ~FawLogManager();

 public:
    template <typename... Args>
    void TRACE(const Args &... args) {
        if (logger_ == nullptr) return;
        auto logger = logger_->LogVerbose();
        LogHelper(logger, args...);
    }

    template <typename... Args>
    void DEBUG(const Args &... args) {
        if (logger_ == nullptr) return;
        auto logger = logger_->LogDebug();
        LogHelper(logger, args...);
    }

    template <typename... Args>
    void INFO(const Args &... args) {
        if (logger_ == nullptr) return;
        auto logger = logger_->LogInfo();
        LogHelper(logger, args...);
    }

    template <typename... Args>
    void WARN(const Args &... args) {
        if (logger_ == nullptr) return;
        auto logger = logger_->LogWarn();
        LogHelper(logger, args...);
    }

    template <typename... Args>
    void ERROR(const Args &... args) {
        if (logger_ == nullptr) return;
        auto logger = logger_->LogError();
        LogHelper(logger, args...);
    }

    template <typename... Args>
    void CRITICAL(const Args &... args) {
        if (logger_ == nullptr) return;
        auto logger = logger_->LogFatal();
        LogHelper(logger, args...);
    }

    void Flush();

    void Reset();

    void SetLogger(const std::string &);

    inline bool ShouldLogging(int severity) { return severity >= level_; }

 private:
    FawLogManager();

 private:
    LogConfig config_;
    int level_ = ADLOG_LEVEL_ERROR;
    faw::ad::framework::LoggerT *logger_;
    std::unordered_map<std::string, int> level_map_ = {
        {"trace", ADLOG_LEVEL_TRACE}, {"debug", ADLOG_LEVEL_DEBUG},
        {"info", ADLOG_LEVEL_INFO},   {"warn", ADLOG_LEVEL_WARN},
        {"error", ADLOG_LEVEL_ERROR}, {"critical", ADLOG_LEVEL_CRITICAL},
        {"off", ADLOG_LEVEL_OFF}};
};

}  // namespace senseAD

#endif
