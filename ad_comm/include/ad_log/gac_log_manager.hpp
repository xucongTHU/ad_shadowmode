/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * LiangYu <liangyu@sensetime.com>
 */
#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <sstream>

#include "ad_log/configuration.hpp"
#include "ad_log/common.hpp"

#ifdef ENABLE_GAC_LOG
#include "gaclog/gac_log.h"
#include "gaclog/gac_log_cout.h"

namespace senseAD {

template <typename T>
void LogHelper(std::stringstream &os, const T &arg) {  // NOLINT
    os << arg;
}

template <typename T, typename... Args>
void LogHelper(std::stringstream &os,  // NOLINT
               const T &arg,
               const Args &... args) {
    os << arg;
    LogHelper(os, args...);
}

class GacLogManager {
 public:
    GacLogManager(const GacLogManager &) = delete;
    GacLogManager &operator=(const GacLogManager &) = delete;

    static GacLogManager &Instance();
    static std::shared_ptr<GacLogManager> SharedInstance();
    ~GacLogManager();

    inline void SwitchToSyncMode() {}

 public:
    template <typename... Args>
    void TRACE(const Args &... args) {
        std::stringstream ss;
        LogHelper(ss, args...);
        GVERBOSE << ss.str();
    }

    template <typename... Args>
    void DEBUG(const Args &... args) {
        std::stringstream ss;
        LogHelper(ss, args...);
        GDEBUG << ss.str();
    }

    template <typename... Args>
    void INFO(const Args &... args) {
        std::stringstream ss;
        LogHelper(ss, args...);
        GINFO << ss.str();
    }

    template <typename... Args>
    void WARN(const Args &... args) {
        std::stringstream ss;
        LogHelper(ss, args...);
        GWARN << ss.str();
    }

    template <typename... Args>
    void ERROR(const Args &... args) {
        std::stringstream ss;
        LogHelper(ss, args...);
        GERROR << ss.str();
    }

    template <typename... Args>
    void CRITICAL(const Args &... args) {
        std::stringstream ss;
        LogHelper(ss, args...);
        GERROR << ss.str();
    }
    void Flush();

    void Reset();

    void SetLogger(const std::string &);

    template <typename LOGLEVEL>
    inline bool ShouldLogging(LOGLEVEL severity) {
        return static_cast<int>(severity) >= static_cast<int>(level_);
    }

    std::string GetLevel();

    bool SetLevel(const std::string &);

    inline void SetCustomLogConf(const LogConfig &) {}
    inline LogConfig GetCurLogConf(void) { return config_; }

 private:
    GacLogManager();

 private:
    static GacLogManager *dangle_ptr_;
    LogConfig config_;
    std::string gslog_conf_path_;
    int level_ = ADLOG_LEVEL_ERROR;
    std::unordered_map<std::string, GAC_LOG_LEVEL> level_map_ = {
        {"trace", ADLOG_LEVEL_VERBOSE}, {"debug", ADLOG_LEVEL_DEBUG},
        {"info", ADLOG_LEVEL_INFO},     {"warn", ADLOG_LEVEL_WARN},
        {"error", ADLOG_LEVEL_ERROR},   {"critical", ADLOG_LEVEL_FATAL},
        {"off", ADLOG_LEVEL_OFF}};
};

#endif

}  // namespace senseAD
