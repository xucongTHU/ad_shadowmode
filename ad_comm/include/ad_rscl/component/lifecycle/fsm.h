/**
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <array>
#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <utility>
#include <vector>

#include "ad_rscl/component/lifecycle/graph.h"

namespace senseAD {

enum class StateChangeStage { PRE_CHANGE, CHANGING, POST_CHANGE };

using StatusChangedCall = std::function<void(const State, const Event)>;
using Call = std::function<bool(const State, const Event)>;
using Calls = std::vector<Call>;
using StateChangeKey = std::pair<StateStage, StateChangeStage>;

class Fsm {
 public:
    Fsm() {}
    virtual ~Fsm() = default;

    bool RegisterFunc(const StateStage state,
                      const StateChangeStage change_stage,
                      const Call& c);
    bool RegisterFunc(const StateChangeKey key, const Call& c);

    bool RegisterStatusChangedFunc(const StatusChangedCall& c);

    bool EmitEvent(const Event event);

    inline const State GetState() const { return graph_.GetState(); }

 private:
    bool Exec(const StateStage state,
              const Event event,
              const StateChangeStage change_stage);
    void TriggerStateChangeCalls(const State, const Event) const;

    std::vector<StatusChangedCall> general_status_changed_callbacks_;
    std::map<StateChangeKey, Calls> callbacks_;

    std::recursive_mutex exec_mutex_;
    StateGraph graph_;
};

}  // namespace senseAD
