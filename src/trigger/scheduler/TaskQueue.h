//
// Created by xucong on 25-6-27.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <queue>
#include "Scheduler.h"

namespace shadow {
namespace trigger {

struct TaskComparator {
    bool operator()(const TriggerTask& a, const TriggerTask& b) const {
        return a.priority > b.priority;
    }
};

using TaskPriorityQueue = std::priority_queue<TriggerTask, std::vector<TriggerTask>, TaskComparator>;

} // namespace trigger
} // namespace shadow

#endif // TASK_QUEUE_H
