/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <vector>
#include <string>

#include "ad_base/std/optional.h"

namespace YAML {
class Node;
}

#define DEFINE_SAVE_LOAD_FN(...)              \
    template <class Archive>                  \
    void save(Archive& ar) const { /*NOLINT*/ \
        ar(__VA_ARGS__);                      \
    }                                         \
    template <class Archive>                  \
    void load(Archive& ar) { /*NOLINT*/       \
        ar(__VA_ARGS__);                      \
    }

namespace senseAD {
namespace diag {

// 0 to 127 is reserved for special supervision
typedef uint64_t SupervisionUniqueId;
constexpr SupervisionUniqueId kSupervisionUniqueIdGeneralComm =
    0x0000000000000001ull;
constexpr SupervisionUniqueId kSupervisionUniqueIdGeneralComponent =
    0x0000000000000002ull;
constexpr int kReservedUniqueIdRange = 127;

constexpr SupervisionUniqueId kUniqueIdForDtcSimpleSupervision = 3;

static_assert(
    kUniqueIdForDtcSimpleSupervision < kReservedUniqueIdRange,
    "Make sure the kUniqueIdForDtcSimpleSupervision < kReservedUniqueIdRange");

enum class SupervisionGroup : uint32_t {
    NONE = 0,

    AVAILABILITY = 1,
    PERFORMANCE = 2,
    RELIABILITY = 3
};
enum class SupervisionTypeL1 : uint32_t {
    NONE = 0,

    // Platform HI
    AVAI_BSW_ALIVE = 1,

    PERF_BSW_RESOURCE = 2,
    PERF_BSW_CM = 3,
    PERF_BSW_COMPUTE_COST = 4,
    PERF_BSW_NTP = 5,
    PERF_BSW_CAN = 6,
    PERF_BSW_NETWORK = 7,

    // SWC HI
    AVAI_SWC_ALIVE = 101,

    PERF_SWC_RESOURCE = 102,
    PERF_SWC_COMPUTE_COST = 103,
    PERF_SWC_CM = 104,

    PERF_SWC_LOCALIZATION = 105,
    PERF_SWC_HDMAP = 106,
    PERF_SWC_PERCEPTION = 107,
    PERF_SWC_SENSOR = 108,
    PERF_SWC_SENSOR_QUALITY = 109,
    PERF_SWC_CAN = 110,
    PERF_SWC_FUSHION = 111,
    PERF_SWC_CALIBRATION = 112,
    PERF_SWC_NODE = 113,
    PERF_SWC_PARAM = 114,
};

enum class ImportantLevel : uint32_t { DEBUG = 0, NORMAL = 1, CORE = 2 };

enum class SupervisionCppType : uint32_t {
    // Used for server side to receive all type. Not expect client use this
    ALL = 99,

    // Simple
    UNKNOWN = 0,
    SIMPLE = 1,
    NO_CHECK = 2,
    RANGE = 3,  // Unused
    THRESHOLD = 4,

    // Checkpoint
    LOGICAL = 5,
    ALIVE = 6,
    DEADLINE = 7,
    HEALTH_CHANNEL = 8,

    // Specified
    SYSTEM_PROFILER = 9,
    COMM = 10,
    COMPONENT = 11,

    PIPELINE = 12,
    DIAG = 13,
    SENSOR = 14,
    SENSOR_COM = 15,
    SENSOR_SYNC_TIME = 16,
    SENSOR_DIAG = 17,
    WORKSHOP = 18,
};

std::string ToString(const SupervisionGroup& g);
SupervisionGroup SupervisionGroupFromString(const std::string& s);
std::string ToString(const SupervisionTypeL1& g);
SupervisionTypeL1 SupervisionTypeL1FromString(const std::string& s);
std::string ToString(const ImportantLevel& g);
ImportantLevel ImportantLevelFromString(const std::string& s);
std::string ToString(const SupervisionCppType& g);
SupervisionCppType SupervisionCppTypeFromString(const std::string& s);

// Who am I
struct SupervisionEntity {
    SupervisionUniqueId unique_id;
    std::string name;

    SupervisionGroup group = SupervisionGroup::NONE;
    SupervisionTypeL1 type_l1 = SupervisionTypeL1::NONE;
    std::string description;

    ImportantLevel important_level = ImportantLevel::DEBUG;
    SupervisionCppType cpp_type = SupervisionCppType::UNKNOWN;

    DEFINE_SAVE_LOAD_FN(
        unique_id, name, group, type_l1, description, important_level, cpp_type)

    static SupervisionEntity FromSpecialSupervisionId(
        const SupervisionUniqueId special_id);

    static SupervisionEntity FromYaml(const YAML::Node& node);
    void ToYaml(YAML::Node* node) const;

    std::string DebugString() const;
};

// Which rule should I follow
struct SupervisionSetting {
    // Common setting
    SupervisionEntity entity;

    // Used for storing raw yaml and let the actual setting to parse
    std::shared_ptr<YAML::Node> param_yaml;

    static SupervisionSetting FromYaml(const YAML::Node& node);
    void ToYaml(YAML::Node* node) const;
    std::string DebugString() const;
};

// How well going on
struct SupervisionStatus {
    // Status at init
    std::string platform_name;
    std::string identifier;
    std::string domain_name;

    std::string process_name;
    std::string pid;
    std::string swc_name;

    // Status at Runtime
    enum class StatusLevel : uint32_t {
        NONE = 0,
        NORMAL = 1,
        HINTS = 2,
        WARNING = 3,
        CRITICAL = 4
    };
    StatusLevel status_level = StatusLevel::NONE;
    std::string problem_description;
    std::atomic<bool> enabled{true};

    uint64_t last_level_change_time_ns = 0;
    uint64_t last_update_time_ns = 0;

    void InitFromSupervisionSetting(const SupervisionSetting& setting);

    struct SupervisionStatusForReport {
        StatusLevel status_level;
        std::string problem_description;
        std::string process_name;
        std::string platform;
        std::string identifier;
        bool enabled;

        uint64_t last_level_change_time_ns = 0;
        uint64_t last_update_time_ns = 0;

        template <class Archive>
        void save(Archive& ar) const {  // NOLINT
            ar(status_level, problem_description, process_name, platform,
               identifier, enabled, last_level_change_time_ns,
               last_update_time_ns);
        }
        template <class Archive>
        void load(Archive& ar) {  // NOLINT
            ar(status_level, problem_description, process_name, platform,
               identifier, enabled, last_level_change_time_ns,
               last_update_time_ns);
        }

        std::string DebugString() const;
    };

    SupervisionStatusForReport GetReportablePart() const;
};

struct SupervisionReport {
    SupervisionUniqueId id;
    SupervisionStatus::SupervisionStatusForReport status;

    uint64_t check_time_ns = 0;
    std::string diagnosis_msg;
    uint64_t seq = 0;

    // enum class ContentType : uint32_t {
    //     NONE = 0,

    //     UNSTRUCTURED_JSON = 1,
    //     STRUCTURED_STRUCT = 2,
    // };
    // ContentType extra_info_type = ContentType::NONE;
    // std::string extra_info_strutname;
    // std::vector<char> extra_info_content;

    template <class Archive>
    void save(Archive& ar) const {  // NOLINT
        ar(id, status, check_time_ns, diagnosis_msg, seq);
        // ar(id, check_time_ns, diagnosis_msg, extra_info_type,
        //    extra_info_strutname, extra_info_content);
    }
    template <class Archive>
    void load(Archive& ar) {  // NOLINT
        ar(id, status, check_time_ns, diagnosis_msg, seq);
        // ar(id, check_time_ns, diagnosis_msg, extra_info_type,
        //    extra_info_strutname, extra_info_content);
    }
};

}  // namespace diag
}  // namespace senseAD
