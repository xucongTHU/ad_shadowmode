//
// Created by xucong on 24-11-27.
// Copyright (c) 2024 T3CAIC. All rights reserved.
//

#include "AEBActiveTrigger.h"

#include "common/time/Timer.h"

namespace shadow {
namespace trigger {

bool AEBActiveTrigger::Proc() {
    bool ok = CheckCondition();
    CHECK_AND_RETURN(ok, AEBActiveTrigger, "CheckCondition failed", false);

    TriggerContext context;
    context.timeStamp = common::Timer::now();
    context.triggerId = trigger_id_;
    context.triggerName = trigger_name_;
    context.businessType = business_type_;
    context.triggerStatus = TriggerContext::TriggerState::Triggered;
    NotifyTriggerContext(context);

    return ok;
}

void AEBActiveTrigger::OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) {
    if (topic == "/caic_pub_test/statemachine") {
        UpdateVehicleInfo(msg);
    }

}

void AEBActiveTrigger::UpdateVehicleInfo(const std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>& msg) {
    kj::ArrayPtr<const capnp::word> vi_kjarr(reinterpret_cast<const capnp::word*>
            (msg->Bytes()), msg->ByteSize());
    ::capnp::FlatArrayMessageReader reader(vi_kjarr);

    auto stateMachine = reader.getRoot<senseAD::msg::tap::AsCmdLgSafe>();
    UpdateASStateMachine(stateMachine);

}

void AEBActiveTrigger::UpdateASStateMachine(const senseAD::msg::tap::AsCmdLgSafe::Reader& report) {
    aeb_active = (report.getShadowMode() == 3); 
    return;
}

bool AEBActiveTrigger::CheckCondition() {
    if (!trigger_checker_.parse(trigger_condition_)) {
        LOG_ERROR("[AEBActiveTrigger]: %s", trigger_checker_.last_error().c_str());
        return false;
    }

    if (debug) {
        auto elmes = trigger_checker_.get_elements();
        for (auto & elem : elmes) {
            LOG_INFO("Variable: %s | Operator: %s | Threshold: %s | Logic: %s",
                      elem.variable.c_str(), elem.comparison_op.c_str(),
                      elem.threshold_str().c_str(),
                      (elem.logical_op.empty() ? "Empty" : elem.logical_op).c_str());
        }
    }

    std::unordered_map<std::string, TriggerConditionChecker::Value> vars;
    vars["aeb_active"] = true; //aeb_active;

    bool ok = trigger_checker_.check(vars);
    
    return ok;
}

void AEBActiveTrigger::NotifyTriggerContext(const TriggerContext& context) {
    if (factoryPtr_) {
        factoryPtr_->OnTriggerContext(context);
    }
    // LOG_INFO("Trigger notified: %s (ID: %s, Time: %ld)",
    //          context.triggerName.c_str(), context.triggerId.c_str(), context.timeStamp);
}

} // namespace trigger
} // namespace shadow
