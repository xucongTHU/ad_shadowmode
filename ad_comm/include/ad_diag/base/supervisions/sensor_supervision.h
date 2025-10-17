/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupengghui <zhoupengghui@senseauto.com>
 */

#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "ad_diag/base/supervisions/supervision.h"

namespace senseAD {
namespace diag {

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

struct SensorSupervisionReport {
    SupervisionReport base;

    bool is_ok = false;
    std::string identifier;
    std::string sub_class_type;
    std::string class_type;
    std::string obj;

    // Not serialize the base part since we will send it seperately
    DEFINE_SAVE_LOAD_FN(is_ok, identifier, sub_class_type, class_type, obj)
};

struct SensorItemInfo {
    bool is_ok;
    int stat = 0;
    SupervisionStatus::StatusLevel level = SupervisionStatus::StatusLevel::NONE;
    std::string diagnosis_msg;
    std::string identifier;
    std::string
        sub_class_type;  // ins_rtk_exception, ins_status_exception, lidar
    std::string class_type;
    std::string obj;
};

class SensorSupervision : public SupervisionBase {
 public:
    explicit SensorSupervision(const SupervisionUniqueId uid);
    explicit SensorSupervision(const SupervisionSetting &custom_setting);
    ~SensorSupervision() override;

    void Report(const SensorItemInfo &parm);
    constexpr static SupervisionCppType Type = SupervisionCppType::SENSOR;
};

typedef SupervisionEndpoint<SensorSupervisionReport, SensorSupervision::Type>
    SensorSupervisionEndpoint;

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

struct SensorCommSupervisionReport {
    SupervisionReport base;

    bool is_ok;
    int stat = 0;
    std::string identifier;
    std::string sub_class_type;
    std::string class_type;

    // Not serialize the base part since we will send it seperately
    DEFINE_SAVE_LOAD_FN(is_ok, stat, identifier, sub_class_type, class_type)
};

class SensorCommSupervision : public SupervisionBase {
 public:
    explicit SensorCommSupervision(const SupervisionUniqueId uid);
    explicit SensorCommSupervision(const SupervisionSetting &custom_setting);
    ~SensorCommSupervision() override;

    void Report(const SensorItemInfo &parm);
    constexpr static SupervisionCppType Type = SupervisionCppType::SENSOR_COM;

 private:
    SensorCommSupervisionReport GenReport() const;

    std::vector<uint8_t> sensor_status_;
};

typedef SupervisionEndpoint<SensorCommSupervisionReport,
                            SensorCommSupervision::Type>
    SensorCommSupervisionEndpoint;

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
enum SensorDeviceType {
    kDeviceUnknown = -1,
    kDeviceCamera = 0,
    kDeviceRadar = 1,
    kDeviceLidar = 2,
    kDeviceImu = 3,
    kDeviceCan = 4,
    kDeviceUss = 5,
    kDeviceIns = 6,
    kDeviceGnss = 7,
    kDeviceDualAntHeading = 8,
};

enum SensorDevReportLevel {
    kDeviceNormal = 0,
    kDeviceHints = 1,
    kDeviceWarning = 2,
    kDeviceError = 3,
    kDeviceCritical = 4,
};
struct SensorSyncTimeSupervisionReport : public SupervisionReport {
    SupervisionReport base;

    SensorDevReportLevel report_level;
    SensorDeviceType device_type;
    std::string device_name;

    DEFINE_SAVE_LOAD_FN(report_level, device_type, device_name)
};

class SensorSyncTimeSupervision : public SupervisionBase {
 public:
    explicit SensorSyncTimeSupervision(const SupervisionUniqueId uid);
    explicit SensorSyncTimeSupervision(
        const SupervisionSetting &custom_setting);

    void Report(const SensorDevReportLevel report_level,
                const SensorDeviceType device_type,
                const std::string &device_name,
                const std::string &diagnosis_msg = "");

    constexpr static SupervisionCppType Type =
        SupervisionCppType::SENSOR_SYNC_TIME;

 private:
};

typedef SupervisionEndpoint<SensorSyncTimeSupervisionReport,
                            SensorSyncTimeSupervision::Type>
    SensorSyncTimeSupervisionEndpoint;

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

struct Sensor {
    enum class Level {
        INFO = 0,
        WARN = 1,
        ERROR = 2,
    };
    std::string id;
    Level level;
    std::string name;
    std::string comment;

    DEFINE_SAVE_LOAD_FN(id, level, name, comment)
};

struct Alarm {
    enum class Level {
        INFO = 0,
        WARN = 1,
        ERROR = 2,
    };
    uint64_t timestamp = 0;
    std::string id;
    Level level;
    std::string major;
    std::string minor;
    std::string value;
    std::string object;
    uint64_t occur_timestamp = 0;
    std::string solution;

    DEFINE_SAVE_LOAD_FN(timestamp,
                        id,
                        level,
                        major,
                        minor,
                        value,
                        object,
                        occur_timestamp,
                        solution)
};

struct SensorDiagAlarm {
    std::vector<Sensor> sensor_list;
    std::vector<Alarm> alarm_list;

    DEFINE_SAVE_LOAD_FN(sensor_list, alarm_list)
};

struct SensorDiagSupervisionReport {
    SupervisionReport base;
    SensorDiagAlarm alarm;

    // Not serialize the base part since we will send it seperately
    DEFINE_SAVE_LOAD_FN(alarm)
};

class SensorDiagSupervision : public SupervisionBase {
 public:
    explicit SensorDiagSupervision(const SupervisionUniqueId uid);
    explicit SensorDiagSupervision(const SupervisionSetting &custom_setting);
    ~SensorDiagSupervision() override;

    void Report(const SensorDiagAlarm &alarm);
    constexpr static SupervisionCppType Type = SupervisionCppType::SENSOR_DIAG;

 private:
    SensorDiagSupervisionReport GenReport(const SensorDiagAlarm &alarm) const;
};

typedef SupervisionEndpoint<SensorDiagSupervisionReport,
                            SensorDiagSupervision::Type>
    SensorDiagSupervisionEndpoint;

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

}  // namespace diag
}  // namespace senseAD
