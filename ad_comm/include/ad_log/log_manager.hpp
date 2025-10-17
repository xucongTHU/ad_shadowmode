/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * LiangYu <liangyu@sensetime.com>
 */

#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <atomic>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <sstream>

#include "ad_log/configuration.hpp"
#include "ad_log/common.hpp"

#include "spdlog/spdlog.h"
#define LOG_SIZE_UNIT_MB 1024 * 1024

#ifdef ENABLE_GAC_LOG
#include "ad_log/gac_log_manager.hpp"

namespace senseAD {
using LogManager = GacLogManager;
using FileLogManager = GacLogManager;
}  // namespace senseAD

#else
namespace senseAD {

class LogManager;

class CerrStreamBuf : public std::streambuf {
 public:
    explicit CerrStreamBuf(LogManager *mgr);
    virtual ~CerrStreamBuf();

 protected:
    int_type overflow(int_type ch) override;
    int sync() override;

    LogManager *mgr_;
    std::stringstream buffer_;
    std::streambuf *old_cerr_buf_;
    std::mutex mtx_;
};

class LogManager {
 public:
    LogManager(const LogManager &) = delete;
    LogManager &operator=(const LogManager &) = delete;

    inline static LogManager &Instance() {
        static LogManager *ptr = SharedInstance().get();
        return *ptr;
    }
    static std::shared_ptr<LogManager> SharedInstance();
    ~LogManager();

 public:
    template <typename... Args>
    void TRACE(const Args &... args) {
        logger_->trace(args...);
    }

    template <typename... Args>
    void DEBUG(const Args &... args) {
        logger_->debug(args...);
    }

    template <typename... Args>
    void INFO(const Args &... args) {
        logger_->info(args...);
    }

    template <typename... Args>
    void WARN(const Args &... args) {
        logger_->warn(args...);
    }

    template <typename... Args>
    void ERROR(const Args &... args) {
        logger_->error(args...);
    }

    template <typename... Args>
    void CRITICAL(const Args &... args) {
        logger_->critical(args...);
    }

    inline bool ShouldLogging(int severity) { return severity >= level_; }

    void Flush();

    void SetLogger(const std::string &time_pid, bool keep_old_file = false);

    void Reset();
    void SwitchToSyncMode();

    std::string GetLevel();
    bool SetLevel(const std::string &level);
    LogFunc GetLogFunc(const std::string &level);

    void DisableCerrRedirect() { cerrbuf_ = nullptr; }

    void SetCustomLogConf(const LogConfig &custom_conf);
    inline LogConfig GetCurLogConf(void) { return config_; }
    void DebugString(void);

 private:
    LogManager();

    template <typename Derived, typename... Ts>
    std::shared_ptr<spdlog::logger> SetupLogger(Ts &&... args) {
        auto logger = std::make_shared<Derived>(std::forward<Ts>(args)...);
        return logger;
    }

 private:
    std::shared_ptr<spdlog::logger> logger_ = nullptr;
    std::unordered_map<std::string, LogFunc> log_funcs_;

    LogConfig config_;
    int level_ = ADLOG_LEVEL_ERROR;
    std::unordered_map<std::string, int> level_map_ = {
        {"trace", ADLOG_LEVEL_TRACE}, {"debug", ADLOG_LEVEL_DEBUG},
        {"info", ADLOG_LEVEL_INFO},   {"warn", ADLOG_LEVEL_WARN},
        {"error", ADLOG_LEVEL_ERROR}, {"critical", ADLOG_LEVEL_CRITICAL},
        {"off", ADLOG_LEVEL_OFF}};
    std::unique_ptr<CerrStreamBuf> cerrbuf_;
};

}  // namespace senseAD

namespace senseAD {
class FileLogManager {
 public:
    FileLogManager(const FileLogManager &) = delete;
    FileLogManager &operator=(const FileLogManager &) = delete;

    static FileLogManager &Instance();
    static std::shared_ptr<FileLogManager> SharedInstance();
    ~FileLogManager();

 public:
    template <typename... Args>
    void TRACE(const Args &... args) {
        logger_->trace(args...);
    }

    template <typename... Args>
    void DEBUG(const Args &... args) {
        logger_->debug(args...);
    }

    template <typename... Args>
    void INFO(const Args &... args) {
        logger_->info(args...);
    }

    template <typename... Args>
    void WARN(const Args &... args) {
        logger_->warn(args...);
    }

    template <typename... Args>
    void ERROR(const Args &... args) {
        logger_->error(args...);
    }

    template <typename... Args>
    void CRITICAL(const Args &... args) {
        logger_->critical(args...);
    }

    void Flush();

    void SetLogger(const std::string &);

    void Reset();

    inline bool ShouldLogging(int severity) { return severity >= level_; }

    LogFunc GetLogFunc(const std::string &level);

    void SetCustomLogConf(const LogConfig &custom_conf);
    inline LogConfig GetCurLogConf(void) { return config_; }
    void DebugString(void);

 private:
    FileLogManager();
    std::unordered_map<std::string, LogFunc> log_funcs_;

    template <typename Derived, typename... Ts>
    std::shared_ptr<spdlog::logger> SetupLogger(Ts &&... args) {
        auto logger = std::make_shared<Derived>(std::forward<Ts>(args)...);
        return logger;
    }

 private:
    std::shared_ptr<spdlog::logger> logger_ = nullptr;
    LogConfig config_;
    int level_ = ADLOG_LEVEL_ERROR;
    std::unordered_map<std::string, int> level_map_ = {
        {"trace", ADLOG_LEVEL_TRACE}, {"debug", ADLOG_LEVEL_DEBUG},
        {"info", ADLOG_LEVEL_INFO},   {"warn", ADLOG_LEVEL_WARN},
        {"error", ADLOG_LEVEL_ERROR}, {"critical", ADLOG_LEVEL_CRITICAL},
        {"off", ADLOG_LEVEL_OFF}};
};

}  // namespace senseAD

#endif

namespace senseAD {

class LoggerManager {
 public:
    inline static void LogTRACE(const char *msg) {
        static LogFunc log_func = GetLogFunc("TRACE");
        log_func(msg);
    }
    inline static void LogDEBUG(const char *msg) {
        static LogFunc log_func = GetLogFunc("DEBUG");
        log_func(msg);
    }
    inline static void LogINFO(const char *msg) {
        static LogFunc log_func = GetLogFunc("INFO");
        log_func(msg);
    }
    inline static void LogWARN(const char *msg) {
        static LogFunc log_func = GetLogFunc("WARN");
        log_func(msg);
    }
    inline static void LogERROR(const char *msg) {
        static LogFunc log_func = GetLogFunc("ERROR");
        log_func(msg);
    }
    inline static void LogCRITICAL(const char *msg) {
        static LogFunc log_func = GetLogFunc("CRITICAL");
        log_func(msg);
    }

    static std::shared_ptr<void> LoggerHolder();

 private:
    static std::string GetLogType();
    static LogFunc GetLogFunc(const std::string &level);
};

}  // namespace senseAD
