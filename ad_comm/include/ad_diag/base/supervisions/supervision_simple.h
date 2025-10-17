/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "ad_diag/base/supervisions/supervision.h"

namespace senseAD {
namespace diag {

/***************************************************
 * !Check Point That Not Use Checkpoint            *
 ***************************************************/
struct SimpleSupervisionReport {
    SupervisionReport base;

    bool is_ok = false;

    // Not serialize the base part since we will send it seperately
    DEFINE_SAVE_LOAD_FN(is_ok)
};

// Let user to directly tell us ok or not
class SimpleSupervision : public SupervisionBase {
 public:
    using LogicFn = std::function<bool(std::string *)>;

    explicit SimpleSupervision(const SupervisionUniqueId uid);
    explicit SimpleSupervision(const SupervisionSetting &custom_setting);
    ~SimpleSupervision() override;

    void Report(const bool is_ok, const std::string &diagnosis_msg = "");
    void SetLogic(LogicFn fn);
    void UnsetLogic();
    // Execute the logic function to get the required argument
    void Report();

    constexpr static SupervisionCppType Type = SupervisionCppType::SIMPLE;

 private:
    SimpleSupervisionReport GenReport(const bool is_ok,
                                      const std::string &diagnosis_msg) const;

    LogicFn logic_;
};

typedef SupervisionEndpoint<SimpleSupervisionReport, SimpleSupervision::Type>
    SimpleSupervisionEndpoint;

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

struct PipelineCost {
    uint64_t instance_id;
    uint32_t index;
    uint32_t sub_index;
    std::string name;
    std::string msg;
    int64_t init_timestamp;
    int64_t timestamp;
    int64_t time_cost;
    int64_t time_cost_avg = 0;
    int64_t time_cost_max = 0;
    int64_t time_cost_min = 0;

    DEFINE_SAVE_LOAD_FN(instance_id,
                        index,
                        sub_index,
                        name,
                        msg,
                        init_timestamp,
                        timestamp,
                        time_cost,
                        time_cost_avg,
                        time_cost_max,
                        time_cost_min)
};

struct DiagInfo {
    uint64_t ts;
    int code;
    int status;
    int event_id;
    int level;
    std::string msg;
    std::string expected;
    std::string actual;
    std::string process;
    std::string module;
    std::string class_type;
    std::string sub_class_type;
    bool is_internal = true;

    DEFINE_SAVE_LOAD_FN(ts,
                        code,
                        status,
                        event_id,
                        level,
                        msg,
                        expected,
                        actual,
                        process,
                        module,
                        class_type,
                        sub_class_type,
                        is_internal)
};

struct NoCheckSupervisionReport {
    SupervisionReport base;

    // Put the monitor report here
    std::vector<char> monitor_data;
    DEFINE_SAVE_LOAD_FN(monitor_data)
};

// Not checking, used to pass data to monitor
class NoCheckSupervision : public SupervisionBase {
 public:
    explicit NoCheckSupervision(const SupervisionUniqueId uid);
    explicit NoCheckSupervision(const SupervisionSetting &custom_setting);
    ~NoCheckSupervision() override;

    void Report(const std::vector<char> &monitor_data);

    constexpr static SupervisionCppType Type = SupervisionCppType::NO_CHECK;

 private:
};

typedef SupervisionEndpoint<NoCheckSupervisionReport, NoCheckSupervision::Type>
    NoCheckSupervisionEndpoint;

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

struct ThresholdSupervisionReport : public SupervisionReport {
    SupervisionReport base;

    float supervise_val = 0.0;
    DEFINE_SAVE_LOAD_FN(supervise_val)
};

struct ThresholdSupervisionSetting {
    // NORMAL
    // (threshold[0]) HINTS
    // (threshold[1]) WARNING
    // (threshold[2]) CRITICAL
    float level_threshold[3];

    static ThresholdSupervisionSetting FromYaml(const YAML::Node &node);
    void ToYaml(YAML::Node *node) const;
};

// check the output value using range as condition
class ThresholdSupervision : public SupervisionBase {
 public:
    using LogicFn = std::function<float(std::string *)>;

    explicit ThresholdSupervision(const SupervisionUniqueId uid);
    explicit ThresholdSupervision(const SupervisionSetting &custom_setting);

    void Report(const float supervise_val,
                const std::string &diagnosis_msg = "");

    void SetLogic(LogicFn fn);
    void UnsetLogic();
    // Execute the logic function to get the required argument
    void Report();

    constexpr static SupervisionCppType Type = SupervisionCppType::THRESHOLD;

 private:
    void StatusUpdateLogic(const float supervise_val,
                           const std::string &diagnosis_msg);

    // Load from yaml
    ThresholdSupervisionSetting specific_setting_;

    LogicFn logic_;
};

typedef SupervisionEndpoint<ThresholdSupervisionReport,
                            ThresholdSupervision::Type>
    ThresholdSupervisionEndpoint;

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

struct PipelineSupervisionReport : public SupervisionReport {
    SupervisionReport base;

    PipelineCost cost;
    DEFINE_SAVE_LOAD_FN(cost)
};

struct PipelineSupervisionSetting {
    float level_threshold[3];

    static PipelineSupervisionSetting FromYaml(const YAML::Node &node);
    void ToYaml(YAML::Node *node) const;
};

// check the output value using range as condition
class PipelineSupervision : public SupervisionBase {
 public:
    explicit PipelineSupervision(const SupervisionUniqueId uid);
    explicit PipelineSupervision(const SupervisionSetting &custom_setting);

    void Report(const PipelineCost &cost,
                const std::string &diagnosis_msg = "");
    constexpr static SupervisionCppType Type = SupervisionCppType::PIPELINE;

 private:
};

typedef SupervisionEndpoint<PipelineSupervisionReport,
                            PipelineSupervision::Type>
    PipelineSupervisionEndpoint;

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
struct DiagSupervisionReport : public SupervisionReport {
    SupervisionReport base;

    DiagInfo diag;
    DEFINE_SAVE_LOAD_FN(diag)
};

class DiagSupervision : public SupervisionBase {
 public:
    explicit DiagSupervision(const SupervisionUniqueId uid);
    explicit DiagSupervision(const SupervisionSetting &custom_setting,
                             const std::string &node_name = "");

    void Report(const DiagInfo &cost, const std::string &diagnosis_msg = "");
    constexpr static SupervisionCppType Type = SupervisionCppType::DIAG;

 private:
};

typedef SupervisionEndpoint<DiagSupervisionReport, DiagSupervision::Type>
    DiagSupervisionEndpoint;

}  // namespace diag
}  // namespace senseAD
