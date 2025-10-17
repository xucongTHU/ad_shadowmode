//
// Created by xucong on 25-2-10.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef TRIGGER_BASE_H
#define TRIGGER_BASE_H

#include <string>
#include <memory>

#include "common/logger/Logger.h"
#include "strategy/StrategyConfig.h"
#include "channel/Observer.h"

namespace shadow {
namespace trigger {

static const char* LOG_TAG = "Trigger";
struct TriggerContext {
    int64_t timeStamp;
    std::string triggerId;
    std::string triggerName;
    std::string businessType;
    enum class TriggerState : int32_t {
        None =0,
        UnTrigger,
        Triggered,
    }triggerStatus;
};

using TriggerContextPtr = std::shared_ptr<TriggerContext>;

class TriggerFactory;

/**
 * @brief Abstract base class for triggers.
 */
class TriggerBase : public Observer {
public:
    virtual ~TriggerBase() = default;

    bool InitTrigger(const strategy::StrategyConfig& strategyConfig);
    virtual bool Proc() = 0;
    virtual bool CheckCondition() = 0;
    virtual void NotifyTriggerContext(const TriggerContext& context) = 0;
    virtual std::string GetTriggerName() const = 0;

    void OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) override = 0;
    void SetFactory(const std::shared_ptr<TriggerFactory>& factory) { factoryPtr_ = factory; }

protected:
    std::unique_ptr<strategy::Trigger> trigger_obj_ = nullptr;
    std::shared_ptr<TriggerFactory> factoryPtr_;
    std::string business_type_;
    std::string trigger_id_;
    std::string trigger_condition_;

};

} // namespace trigger
} // namespace shadow

#endif //TRIGGER_BASE_H
