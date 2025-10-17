/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Gongshusheng <gongshusheng@senseauto.com>
 */

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace mw {
namespace task {

// 定义 CPU 核心绑定信息
struct CpuCoreConfig {
    uint32_t thread_index;
    std::string schedule_policy;
    int32_t schedule_prio;
    std::vector<uint32_t> cpu_core;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(CEREAL_NVP(thread_index), CEREAL_NVP(schedule_policy),
                CEREAL_NVP(schedule_prio), CEREAL_NVP(cpu_core));
    }
};

// 定义调度器配置
struct Configuration {
    std::string scheduler_type;
    std::string fair_type;
    int priority_count;
    int wait_time_for_upgrade;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(CEREAL_NVP(scheduler_type), CEREAL_NVP(fair_type),
                CEREAL_NVP(priority_count), CEREAL_NVP(wait_time_for_upgrade));
    }
};

// 定义线程池调度器
struct ThreadPoolSchedulerConfig {
    std::string scheduler_name{"PoolSg"};
    uint32_t thread_num;
    std::vector<CpuCoreConfig> thread_list;
    Configuration configuration;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(CEREAL_NVP(scheduler_name), CEREAL_NVP(thread_num),
                CEREAL_NVP(thread_list), CEREAL_NVP(configuration));
    }
};

// 定义全局调度器配置
struct GlobalSchedulerConfig {
    bool enable{true};
    std::string scheduler_name{"GlobalSg"};
    uint32_t thread_pool_num;
    std::string thread_pool_policy{"SCHED_OTHER"};
    int thread_pool_prio;
    std::vector<uint32_t> thread_pool_cpu_set;

    struct PersistentTask {
        std::string persistent_task_name;
        std::string persistent_task_policy{"SCHED_OTHER"};
        int persistent_task_priority;
        std::vector<uint32_t> persistent_task_cpuset;
        std::string persistent_task_threadname;

        template <class Archive>
        void serialize(Archive& archive) {  // NOLINT
            archive(CEREAL_NVP(persistent_task_name),
                    CEREAL_NVP(persistent_task_policy),
                    CEREAL_NVP(persistent_task_priority),
                    CEREAL_NVP(persistent_task_cpuset),
                    CEREAL_NVP(persistent_task_threadname));
        }
    };

    struct CommonTask {
        std::string task_name;
        uint8_t task_priority;

        template <class Archive>
        void serialize(Archive& archive) {  // NOLINT
            archive(CEREAL_NVP(task_name), CEREAL_NVP(task_priority));
        }
    };

    std::vector<PersistentTask> persistent_task_list;
    std::vector<CommonTask> common_task_list;

    GlobalSchedulerConfig()
        : enable(true), scheduler_name("global_sg"), thread_pool_num(2) {}

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(CEREAL_NVP(enable), CEREAL_NVP(scheduler_name),
                CEREAL_NVP(thread_pool_num), CEREAL_NVP(thread_pool_policy),
                CEREAL_NVP(thread_pool_prio), CEREAL_NVP(thread_pool_cpu_set),
                CEREAL_NVP(persistent_task_list), CEREAL_NVP(common_task_list));
    }
};

// 定义定时器调度器
struct TimerSchedulerConfig {
    std::string scheduler_name{"TimeSg"};
    uint32_t thread_num;
    std::vector<CpuCoreConfig> thread_list;
    struct TimerSchedulerConfiguration {
        struct TimerThread {
            std::string schedule_policy{"SCHED_FIFO"};
            int schedule_prio;
            std::vector<uint32_t> cpu_core;

            template <class Archive>
            void serialize(Archive& archive) {  // NOLINT
                archive(CEREAL_NVP(schedule_policy), CEREAL_NVP(schedule_prio),
                        CEREAL_NVP(cpu_core));
            }
        };

        TimerThread timer_thread;

        template <class Archive>
        void serialize(Archive& archive) {  // NOLINT
            archive(CEREAL_NVP(timer_thread));
        }
    };

    TimerSchedulerConfiguration configuration;

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(CEREAL_NVP(scheduler_name), CEREAL_NVP(thread_num),
                CEREAL_NVP(thread_list), CEREAL_NVP(configuration));
    }
};

// 定义完整配置结构
struct SchedulerConfig {
    std::vector<ThreadPoolSchedulerConfig> threadpool_scheduler;
    std::vector<TimerSchedulerConfig> timer_scheduler;
    GlobalSchedulerConfig global_scheduler;
    SchedulerConfig()
        : threadpool_scheduler(1), timer_scheduler(1), global_scheduler() {}

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(CEREAL_NVP(threadpool_scheduler), CEREAL_NVP(timer_scheduler),
                CEREAL_NVP(global_scheduler));
        // archive(CEREAL_NVP(timer_scheduler), CEREAL_NVP(global_scheduler));
    }
};

inline bool LoadSchedulerConfig(const std::string& fconfig,
                                SchedulerConfig* config) {
    if (!fconfig.size()) {
        AD_LINFO(LoadSchedulerConfig)
            << " fconfig is null,use default SchedulerConfig";
        SchedulerConfig default_config;
        (*config) = default_config;
        return true;
    }

    std::ifstream file(fconfig);
    if (!file.is_open()) {
        AD_LERROR(LoadSchedulerConfig)
            << "Failed to open file: " << fconfig << std::endl;
        return false;
    }

    // SchedulerConfig config;
    try {
        cereal::JSONInputArchive archive(file);
        (*config).serialize(archive);
    } catch (const cereal::Exception& e) {
        AD_LERROR(LoadSchedulerConfig) << "Serialization error: " << e.what();
        return false;
    }
    return true;
}

}  // namespace task
}  // namespace mw
}  // namespace senseAD
