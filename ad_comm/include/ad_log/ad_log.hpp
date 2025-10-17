
/*
 * Copyright (C) 2017 by SenseTime Group Limited. All rights reserved.
 * Sun Gefei <sungefei@sensetime.com>
 * Chen Kaige <chenkaige@sensetime.com>
 * Guo Zhichong <guozhichong@sensetime.com>
 */

#pragma once

#include <unistd.h>
#include <sys/time.h>
#include <chrono>
#include <iostream>
#include <string>
#include <type_traits>

#include "ad_log/configuration.hpp"
#include "ad_log/log_interface.hpp"

#ifdef HAF_LOG
#include "adsfi_log/logger.hpp"
#endif

#ifdef ENABLE_GAC_LOG
#include "gaclog/gac_log.h"
#include "gaclog/gac_log_cout.h"
#endif

#ifndef JOIN_STRING
#define _JOIN_STRING(A, B) A##B
#define JOIN_STRING(A, B) _JOIN_STRING(A, B)
#endif

#define AD_LOG_EVERY_SEC(LOG_FUNC, SEC)                                        \
    static std::size_t JOIN_STRING(count, __LINE__) = 0;                       \
    static auto JOIN_STRING(last_time, __LINE__) =                             \
        std::chrono::system_clock::time_point(std::chrono::seconds(0));        \
    auto JOIN_STRING(now, __LINE__) = std::chrono::system_clock::now();        \
    auto JOIN_STRING(timeout, __LINE__) =                                      \
        (JOIN_STRING(now, __LINE__) >=                                         \
         JOIN_STRING(last_time, __LINE__) + std::chrono::seconds(SEC));        \
    auto JOIN_STRING(count_shadow, __LINE__) = ++JOIN_STRING(count, __LINE__); \
    if (JOIN_STRING(timeout, __LINE__)) {                                      \
        JOIN_STRING(last_time, __LINE__) = std::chrono::system_clock::now();   \
        JOIN_STRING(count, __LINE__) = 0;                                      \
    }                                                                          \
    if (JOIN_STRING(timeout, __LINE__))                                        \
    LOG_FUNC << JOIN_STRING(count_shadow, __LINE__) << " "

#define AD_LOG_EVERY_N(LOG_FUNC, N)                      \
    static std::size_t JOIN_STRING(count, __LINE__) = 0; \
    ++JOIN_STRING(count, __LINE__);                      \
    if (JOIN_STRING(count, __LINE__) > (N)) {            \
        JOIN_STRING(count, __LINE__) -= (N);             \
    }                                                    \
    if (JOIN_STRING(count, __LINE__) == 1) LOG_FUNC

#ifdef HAF_LOG
#define AD_LTRACE(tag) HAF_LOG_VERBOSE_SDK(tag)
#define AD_LDEBUG(tag) HAF_LOG_DEBUG_SDK(tag)
#define ADAS_LDEBUG(tag) HAF_LOG_DEBUG_SDK(tag)
#define AD_LINFO(tag) HAF_LOG_INFO_SDK(tag)
#define AD_LERROR(tag) HAF_LOG_ERROR_SDK(tag)
#define AD_LWARN(tag) HAF_LOG_WARN_SDK(tag)
#define AD_LFATAL(tag) HAF_LOG_FATAL_SDK(tag)

#define AD_FTRACE(tag) HAF_LOG_VERBOSE_SDK(tag)
#define AD_FDEBUG(tag) HAF_LOG_DEBUG_SDK(tag)
#define AD_FINFO(tag) HAF_LOG_INFO_SDK(tag)
#define AD_FERROR(tag) HAF_LOG_ERROR_SDK(tag)
#define AD_FWARN(tag) HAF_LOG_WARN_SDK(tag)
#define AD_FFATAL(tag) HAF_LOG_FATAL_SDK(tag)

#define LOG_TRACE(tag) HAF_LOG_VERBOSE_SDK(tag)
#define LOG_DEBUG(tag) HAF_LOG_DEBUG_SDK(tag)
#define LOG_INFO(tag) HAF_LOG_INFO_SDK(tag)
#define LOG_ERROR(tag) HAF_LOG_ERROR_SDK(tag)
#define LOG_WARN(tag) HAF_LOG_WARN_SDK(tag)
#define LOG_FATAL(tag) HAF_LOG_FATAL_SDK(tag)

#define IFR_LTRACE(tag) HAF_LOG_VERBOSE_SDK(tag)
#define IFR_LDEBUG(tag) HAF_LOG_DEBUG_SDK(tag)
#define IFR_LINFO(tag) HAF_LOG_INFO_SDK(tag)
#define IFR_LERROR(tag) HAF_LOG_ERROR_SDK(tag)
#define IFR_LWARN(tag) HAF_LOG_WARN_SDK(tag)
#define IFR_LFATAL(tag) HAF_LOG_FATAL_SDK(tag)

#elif defined(ENABLE_GAC_LOG)

#define AD_LTRACE(tag)      GVERBOSE << " [" << (#tag) << "] "
#define AD_LDEBUG(tag)      GDEBUG << " [" << (#tag) << "] "
#define ADAS_LDEBUG(tag)    GDEBUG << " [" << (#tag) << "] "
#define AD_LINFO(tag)       GINFO << " [" << (#tag) << "] "
#define AD_LERROR(tag)      GERROR << " [" << (#tag) << "] "
#define AD_LWARN(tag)       GWARN << " [" << (#tag) << "] "
#define AD_LFATAL(tag)      GERROR << " [" << (#tag) << "] "

#define AD_FTRACE(tag)      GVERBOSE << " [" << (#tag) << "] "
#define AD_FDEBUG(tag)      GDEBUG << " [" << (#tag) << "] "
#define AD_FINFO(tag)       GINFO << " [" << (#tag) << "] "
#define AD_FERROR(tag)      GERROR << " [" << (#tag) << "] "
#define AD_FWARN(tag)       GWARN << " [" << (#tag) << "] "
#define AD_FFATAL(tag)      GERROR << " [" << (#tag) << "] "

#define LOG_TRACE(tag)      GVERBOSE << " [" << (#tag) << "] "
#define LOG_DEBUG(tag)      GDEBUG << " [" << (#tag) << "] "
#define LOG_INFO(tag)       GINFO << " [" << (#tag) << "] "
#define LOG_ERROR(tag)      GERROR << " [" << (#tag) << "] "
#define LOG_WARN(tag)       GWARN << " [" << (#tag) << "] "
#define LOG_FATAL(tag)      GERROR << " [" << (#tag) << "] "

#define IFR_LTRACE(tag)     GVERBOSE << " [" << (#tag) << "] "
#define IFR_LDEBUG(tag)     GDEBUG << " [" << (#tag) << "] "
#define IFR_LINFO(tag)      GINFO << " [" << (#tag) << "] "
#define IFR_LERROR(tag)     GERROR << " [" << (#tag) << "] "
#define IFR_LWARN(tag)      GWARN << " [" << (#tag) << "] "
#define IFR_LFATAL(tag)     GERROR << " [" << (#tag) << "] "

#else

#define AD_LTRACE(tag) LOGS_TRACE(tag)
#define AD_LDEBUG(tag) LOGS_DEBUG(tag)
#define ADAS_LDEBUG(tag) LOGS_DEBUG(tag)
#define AD_LINFO(tag) LOGS_INFO(tag)
#define AD_LERROR(tag) LOGS_ERROR(tag)
#define AD_LWARN(tag) LOGS_WARN(tag)
#define AD_LFATAL(tag) LOGS_CRITICAL(tag)
// with the prefix of AD_F  it will log into background log file
#define AD_FTRACE(tag) FLOGS_TRACE(tag)
#define AD_FDEBUG(tag) FLOGS_DEBUG(tag)
#define AD_FINFO(tag) FLOGS_INFO(tag)
#define AD_FERROR(tag) FLOGS_ERROR(tag)
#define AD_FWARN(tag) FLOGS_WARN(tag)
#define AD_FFATAL(tag) FLOGS_CRITICAL(tag)
#endif

// Every N
#define AD_LTRACE_EVERY(tag, n) AD_LOG_EVERY_N(AD_LTRACE(tag), n)
#define AD_LDEBUG_EVERY(tag, n) AD_LOG_EVERY_N(AD_LDEBUG(tag), n)
#define AD_LINFO_EVERY(tag, n) AD_LOG_EVERY_N(AD_LINFO(tag), n)
#define AD_LERROR_EVERY(tag, n) AD_LOG_EVERY_N(AD_LERROR(tag), n)
#define AD_LWARN_EVERY(tag, n) AD_LOG_EVERY_N(AD_LWARN(tag), n)
#define AD_LFATAL_EVERY(tag, n) AD_LOG_EVERY_N(AD_LFATAL(tag), n)
#define AD_FTRACE_EVERY(tag, n) AD_LOG_EVERY_N(AD_FTRACE(tag), n)
#define AD_FDEBUG_EVERY(tag, n) AD_LOG_EVERY_N(AD_FDEBUG(tag), n)
#define AD_FINFO_EVERY(tag, n) AD_LOG_EVERY_N(AD_FINFO(tag), n)
#define AD_FERROR_EVERY(tag, n) AD_LOG_EVERY_N(AD_FERROR(tag), n)
#define AD_FWARN_EVERY(tag, n) AD_LOG_EVERY_N(AD_FWARN(tag), n)
#define AD_FFATAL_EVERY(tag, n) AD_LOG_EVERY_N(AD_FFATAL(tag), n)

#define AD_LTRACE_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_LTRACE(tag), n)
#define AD_LDEBUG_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_LDEBUG(tag), n)
#define AD_LINFO_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_LINFO(tag), n)
#define AD_LERROR_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_LERROR(tag), n)
#define AD_LWARN_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_LWARN(tag), n)
#define AD_FTRACE_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_FTRACE(tag), n)
#define AD_FDEBUG_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_FDEBUG(tag), n)
#define AD_FINFO_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_FINFO(tag), n)
#define AD_FERROR_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_FERROR(tag), n)
#define AD_FWARN_EVERY_SEC(tag, n) AD_LOG_EVERY_SEC(AD_FWARN(tag), n)

#define START_FUNCTION(NAME)                                     \
    timeval M_PROCEDURE_##NAME##_START_TIME;                     \
    do {                                                         \
        LOGS_WARN(TIME) << "[" << #NAME << "] started.";         \
        gettimeofday(&M_PROCEDURE_##NAME##_START_TIME, nullptr); \
    } while (0)
#define FINISH_FUNCTION(NAME)                                              \
    do {                                                                   \
        LOGS_WARN(TIME) << "[" << #NAME << "] finished.";                  \
        timeval M_PROCEDURE_##NAME##_FINISH_TIME;                          \
        gettimeofday(&M_PROCEDURE_##NAME##_FINISH_TIME, nullptr);          \
        LOGS_WARN(TIME) << "[" << #NAME << "] totally time "               \
                        << (M_PROCEDURE_##NAME##_FINISH_TIME.tv_sec -      \
                            M_PROCEDURE_##NAME##_START_TIME.tv_sec) *      \
                                   1000.0 +                                \
                               (M_PROCEDURE_##NAME##_FINISH_TIME.tv_usec - \
                                M_PROCEDURE_##NAME##_START_TIME.tv_usec) / \
                                   1000.0                                  \
                        << " ms.";                                         \
    } while (0)

#define INIT_FRAME_TIMER(NAME)      \
    double M_TOTAL_TIME_##NAME = 0; \
    int M_TOTAL_COUNT_##NAME = 0;

#define START_FRAME_TIMER(NAME)                             \
    timeval M_FRAME_START_TIME_##NAME;                      \
    do {                                                    \
        LOGS_WARN(TIME) << "\"" << #NAME << "\": started."; \
        gettimeofday(&M_FRAME_START_TIME_##NAME, nullptr);  \
    } while (0)

#define FINISH_FRAME_TIMER(NAME, COUNT)                               \
    do {                                                              \
        LOGS_WARN(TIME) << "\"" << #NAME << "\": finished.";          \
        timeval M_FRAME_FINISH_TIME_##NAME;                           \
        gettimeofday(&M_FRAME_FINISH_TIME_##NAME, nullptr);           \
        double M_FRAME_TIME_COST_##NAME =                             \
            (M_FRAME_FINISH_TIME_##NAME.tv_sec -                      \
             M_FRAME_START_TIME_##NAME.tv_sec) *                      \
                1000.0 +                                              \
            (M_FRAME_FINISH_TIME_##NAME.tv_usec -                     \
             M_FRAME_START_TIME_##NAME.tv_usec) /                     \
                1000.0;                                               \
        M_TOTAL_TIME_##NAME += M_FRAME_TIME_COST_##NAME;              \
        M_TOTAL_COUNT_##NAME += COUNT;                                \
        LOGS_WARN(TIME) << "\"" << #NAME << "\": totally time "       \
                        << M_FRAME_TIME_COST_##NAME << " ms.";        \
        LOGS_WARN(TIME) << "\"" << #NAME << "\": average time "       \
                        << M_TOTAL_TIME_##NAME / M_TOTAL_COUNT_##NAME \
                        << " ms.";                                    \
    } while (0)

#define LOGF_TIME_NS(tpc, msg, stat)                                           \
    AD_FINFO(tpc##_##stat) << (uint64_t)(senseAD::time::Now().ToNSec()) << " " \
                           << msg->header.stamp.toNSec();

#define LOGF_TIME_SEND_NO_MSG(tpc, timestamp)                                  \
    AD_FINFO(tpc##_##SEND) << (uint64_t)(senseAD::time::Now().ToNSec()) << " " \
                           << timestamp;

#define LOGF_TIME_RECV_NS(tpc, msg) LOGF_TIME_NS(tpc, msg, RECV)
#define LOGF_TIME_SEND_NS(tpc, msg) LOGF_TIME_NS(tpc, msg, SEND)

namespace senseAD {

class LoggingHelper {
 public:
    static void InitLogging(const std::string log_dir = "./",
                            const char* prog_name = "",
                            const std::string symbol_link_file_basename = "");
    static size_t GetThreadId(void);

 private:
    static LoggingHelper* Instance();

    void Init(const std::string log_dir,
              const char* prog_name,
              const std::string symbol_link_file_basename);
};

}  // namespace senseAD
