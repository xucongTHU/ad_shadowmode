//
// Created by xucong on 25-6-27.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <memory>
#include <string>
#include "trigger/base/TriggerBase.h"
#include "strategy/StrategyConfig.h"

namespace shadow {
namespace trigger {

enum class TaskState { WAITING, RUNNING, PAUSED, FINISHED };

struct TriggerTask {
    std::string trigger_name;
    int8_t priority;
    std::shared_ptr<TriggerBase> trigger;
    strategy::StrategyConfig strategyConfig;
    bool cancelled = false;
    TaskState state = TaskState::WAITING;
    int retry_count = 0;
    int max_retries;
    std::chrono::steady_clock::time_point last_attempt_time;
    std::chrono::milliseconds retry_interval;
};

class Scheduler {
public:
    virtual ~Scheduler() = default;
    virtual void AddTask(TriggerTask task) = 0;
    virtual void StartScheduling() = 0;
};

} // namespace trigger
} // namespace shadow

#endif // SCHEDULER_H
