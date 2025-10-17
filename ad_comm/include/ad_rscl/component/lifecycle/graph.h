/**
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <atomic>
#include <set>
#include <iostream>
#include <mutex>
#include <vector>
#include <map>
#include <utility>
#include <functional>

#include "ad_rscl/component/lifecycle/event.h"
#include "ad_rscl/component/lifecycle/state.h"

namespace senseAD {
// For enum class print
template <typename T>
std::ostream& operator<<(
    typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream,
    const T& e) {
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

class StateGraph {
 public:
    StateGraph();

    bool EmitEvent(const Event e);
    bool CheckEventValidate(const Event e) const;
    StateStage NextStateStage(const Event e) const;

    const State GetState() const;

    std::set<StateStage> PossibleNextStateStage() const;

 private:
    void AddDirectedTransform(const StateStage from_stage,
                              const Event e,
                              const StateStage to_state);
    void AddDirectedTransform(const StateStage from_stage,
                              const Event e,
                              const StateStage to_state,
                              std::function<void(State*)> state_transform_fn);

    std::map<StateStage, std::set<Event>> possible_events_;
    std::map<std::pair<StateStage, Event>, StateStage> stage_graph_;
    std::map<std::pair<StateStage, Event>, std::function<void(State*)>>
        state_transform_fns_;
    State curr_state_;
};
}  // namespace senseAD
