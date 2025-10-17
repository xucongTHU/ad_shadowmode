
/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * gongshusheng <gongshusheng@senseauto.com>
 */

#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>
#include "ad_diag/base/supervisions/supervision.h"

namespace senseAD {
namespace diag {
//-----------------------------------

struct WorkshopInfo {
    struct Data {
        uint64_t count;
        Data() : count(0) {}
        Data(const Data& other) : count(other.count) {}
        Data& operator=(const Data& other) {
            count = other.count;
            return *this;
        }
        explicit Data(uint64_t count) : count(count) {}

        DEFINE_SAVE_LOAD_FN(count)
    };

    struct Time {
        uint64_t min;
        uint64_t max;
        uint64_t avg;
        Time() : min(0), max(0), avg(0) {}
        Time(const Time& other)
            : min(other.min), max(other.max), avg(other.avg) {}
        Time& operator=(const Time& other) {
            min = other.min;
            max = other.max;
            avg = other.avg;
            return *this;
        }
        Time(uint64_t min, uint64_t max, uint64_t avg)
            : min(min), max(max), avg(avg) {}

        DEFINE_SAVE_LOAD_FN(min, max, avg)
    };

    struct Condition {
        uint64_t valid_count;
        uint64_t invalid_count;
        Condition() : valid_count(0), invalid_count(0) {}
        Condition(const Condition& other)
            : valid_count(other.valid_count),
            invalid_count(other.invalid_count) {}
        Condition& operator=(const Condition& other) {
            valid_count = other.valid_count;
            invalid_count = other.invalid_count;
            return *this;
        }
        Condition(uint64_t valid_count, uint64_t invalid_count)
            : valid_count(valid_count), invalid_count(invalid_count) {}

        DEFINE_SAVE_LOAD_FN(valid_count, invalid_count)
    };

    std::string name;
    std::unordered_map<std::string, Data> data_map;
    std::unordered_map<std::string, Time> time_map;
    std::unordered_map<std::string, Condition> condi_map;

    WorkshopInfo() = default;
    explicit WorkshopInfo(const std::string& name_o) : name(name_o) {}
    WorkshopInfo(const WorkshopInfo& other) {
        this->name = other.name;
        this->data_map = other.data_map;
        this->time_map = other.time_map;
        this->condi_map = other.condi_map;
    }
    WorkshopInfo& operator=(const WorkshopInfo& other) {
        this->name = other.name;
        this->data_map = other.data_map;
        this->time_map = other.time_map;
        this->condi_map = other.condi_map;
        return *this;
    }
    DEFINE_SAVE_LOAD_FN(name, data_map, time_map, condi_map)
};

struct WorkshopSupervisionReport : public SupervisionReport {
    SupervisionReport base;

    WorkshopInfo info;
    DEFINE_SAVE_LOAD_FN(info)
};

class WorkshopSupervision : public SupervisionBase {
 public:
    explicit WorkshopSupervision(const SupervisionSetting& custom_setting,
                                 const std::string& node_name = "");

    void Report(const WorkshopInfo& info, const std::string& msg = "");
    constexpr static SupervisionCppType Type = SupervisionCppType::WORKSHOP;

 private:
    // WorkshopInfo data;
};

typedef SupervisionEndpoint<WorkshopSupervisionReport,
                            WorkshopSupervision::Type>
    WorkshopSupervisionEndpoint;

}  // namespace diag
}  // namespace senseAD
