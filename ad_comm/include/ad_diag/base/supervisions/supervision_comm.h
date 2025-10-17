/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cmath>
#include <string>
#include <list>
#include <unordered_map>

#include "ad_base/std/utils.h"
#include "ad_diag/base/supervisions/supervision.h"

namespace senseAD {
namespace diag {

enum class CommSide : uint8_t {
    PUB = 0,
    SUB = 1,
    CLI = 2,
    SERV = 3,
    FORWARD = 4
};
enum class CommErrIssue { HZ_ERR, GAP_ERR, SIZE_ERR };

struct CommStat {
    float frame_ratio = 0;
    float max_gap_ns = 0;
    float min_gap_ns = 0;
    // In KB
    double avg_size_kb = 0;

    bool is_key_frame = false;

    uint64_t frame_ts = 0;

    DEFINE_SAVE_LOAD_FN(frame_ratio,
                        max_gap_ns,
                        min_gap_ns,
                        avg_size_kb,
                        is_key_frame,
                        frame_ts)

    std::string DebugString() const;
};

struct CommSupervisionReport {
    SupervisionReport base;

    std::string node_name;
    std::string topic_name;
    CommSide comm_side;
    bool is_sensor = false;

    CommStat stat;
    std::unordered_map<CommErrIssue, std::string, senseAD::base::EnumClassHash>
        err_reason;

    // Not serialize the base part since we will send it seperately
    DEFINE_SAVE_LOAD_FN(
        node_name, topic_name, comm_side, is_sensor, stat, err_reason)
};

struct CommSupervisionSetting {
    float min_hz = -1;
    float max_hz = -1;
    float warning_hz = -1;
    float hints_hz = -1;

    double max_avg_size_kb = -1;

    float min_gap_ns = -1;
    float max_gap_ns = -1;
    float warning_gap_ns = -1;
    float hints_gap_ns = -1;

    bool is_key_frame = false;

    std::string DebugString() const;
    static CommSupervisionSetting FromYaml(const YAML::Node &node);
    void ToYaml(YAML::Node *node) const;
};

// Let user to directly tell us ok or not
class CommSupervision : public SupervisionBase {
 public:
    using StatGetFn = std::function<CommStat()>;

    explicit CommSupervision(const std::string &node_name,
                             const std::string &topic_name,
                             const CommSide comm_side);
    ~CommSupervision() override;

    void SetStatGetFn(StatGetFn fn);
    void UnsetStatGetFn();
    // Execute the logic function to get the required argument
    void Report(bool is_sensor = false);

    constexpr static SupervisionCppType Type = SupervisionCppType::COMM;

 private:
    CommSupervisionReport CheckLogic(const CommStat &stat);
    CommStat BuildCommStat();

    std::string node_name_;
    std::string topic_name_;
    CommSide comm_side_;

    std::list<uint64_t> hz_vec_;
    uint64_t last_frame_ts_ = 0;
    StatGetFn stat_get_fn_;
    CommSupervisionSetting specific_setting_;

    bool need_check_ = false;
};

typedef SupervisionEndpoint<CommSupervisionReport, CommSupervision::Type>
    CommSupervisionEndpoint;

}  // namespace diag
}  // namespace senseAD
