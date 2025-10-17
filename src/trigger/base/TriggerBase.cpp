//
// Created by xucong on 25-2-10.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "TriggerBase.h"

#include <memory>

namespace shadow {
namespace trigger {

bool TriggerBase::InitTrigger(const strategy::StrategyConfig &strategyConfig) {
    for (const auto& st : strategyConfig.strategies) {
        if (st.trigger.triggerName == GetTriggerName()) {
            trigger_obj_ = std::make_unique<strategy::Trigger>(st.trigger);
            business_type_ = st.businessType;
            trigger_id_ = st.trigger.triggerId;
            trigger_condition_ = st.trigger.triggerCondition;
            break;
        }
    }

    if (trigger_obj_ == nullptr) {
        LOG_ERROR("failed to lock trigger %s", GetTriggerName().c_str());
        return false;
    }
    return true;
}

} // namespace trigger
} // namespace shadow
