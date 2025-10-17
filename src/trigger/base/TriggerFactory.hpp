//
// Created by xucong on 25-2-10.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//


#ifndef TRIGGER_FACTORY_H
#define TRIGGER_FACTORY_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>
#include <utility>
#include <shared_mutex>
#include "trigger/base/TriggerBase.h"
#include "trigger/scheduler/Scheduler.h"
#include "ThreadPool/ThreadPool.h"

namespace shadow {
namespace trigger {

class TriggerFactory : public std::enable_shared_from_this<TriggerFactory> {
public:
    TriggerFactory() = default;
    ~TriggerFactory() = default;

    using CreatorFunc = std::function<std::shared_ptr<TriggerBase>()>;
    using TriggerCallback = std::function<void(const TriggerContext&)>;

    /**
     * @brief Set the trigger callback function.
     * @param callback The callback function to set.
     */
    void SetTriggerCallback(TriggerCallback cb) {
        std::unique_lock<std::shared_mutex> lock(callback_mutex_);
        callback_ = std::move(cb);
    }


    /**
     * @brief Invoke the trigger callback function.
     * @param context The trigger context to pass to the callback function.
     */
    void OnTriggerContext(const TriggerContext& ctx) {
        std::shared_lock<std::shared_mutex> lock(callback_mutex_);
        if (callback_) callback_(ctx);
    }

    /**
     * @brief Set the global NodeHandle instance.
     * @param node The node handle instance to set.
     */
    void SetNodeHandle(const std::shared_ptr<senseAD::rscl::comm::Node>& node) {
        node_ptr_ = node;
    }

    /**
     * @brief Get the global NodeHandle instance.
     * @return Pointer to the NodeHandle instance.
     */
    std::shared_ptr<senseAD::rscl::comm::Node> GetNodeHandle() {
        return node_ptr_;
    }

    void SetTriggerConfig(const strategy::StrategyConfig& config) {
        std::unique_lock<std::shared_mutex> lock(config_mutex_);
        strategy_config_ = config;
    }

    /**
     * @brief Register a trigger with the factory.
     * @param trigger_name The name of the trigger.
     * @param creator A function that creates an instance of the trigger.
     */
    void RegisterTrigger(const std::string& trigger_name, CreatorFunc creator) {
        std::unique_lock<std::shared_mutex> lock(trigger_mutex_);
        triggers_[trigger_name] = std::move(creator);
    }

    std::shared_ptr<TriggerBase> GetTrigger(const std::string& trigger_name) {
        CreatorFunc creator;
        {
            std::shared_lock<std::shared_mutex> lock(trigger_mutex_);
            auto it = triggers_.find(trigger_name);
            if (it == triggers_.end()) return nullptr;
            creator = it->second;
        }

        return creator ? creator() : nullptr;
    }

    /**
     * @brief Initialize the trigger scheduler.
     * @param node The node handle instance.
     * @param scheduler The scheduler instance to use.
     */
    bool InitTriggerScheduler(const std::shared_ptr<senseAD::rscl::comm::Node>& node,
                              std::shared_ptr<Scheduler> scheduler) {
        scheduler_ = std::move(scheduler);
        if (!scheduler_) {
            LOG_ERROR("Scheduler is not initialized.");
            return false;
        }

        std::vector<std::string> enabled_triggers;
        int trigger_priority = std::numeric_limits<int>::max();
        size_t threads = 0;
        {
            std::shared_lock lock(config_mutex_);
            for (const auto& s : strategy_config_.strategies) {
                if (s.trigger.enabled) {
                    enabled_triggers.push_back(s.trigger.triggerName);
                    trigger_priority = s.trigger.priority;
                    ++threads;
                }
            }
        }

        if (threads == 0) threads = 1;

        if (enabled_triggers.empty())
        {
            LOG_WARN("[TriggerFactory] No enabled triggers found.");
        }

        bool success = true;
        for (const auto& trigger_name : enabled_triggers)
        {
            auto trigger = CreateTrigger(trigger_name, strategy_config_);
            if (!trigger) {
                LOG_ERROR("Trigger creation failed for %s", trigger_name.c_str());
                success = false;
                continue;
            }
            trigger->SetFactory(shared_from_this()); // 设置工厂指针
            success = trigger->InitTrigger(strategy_config_);
            CHECK_AND_RETURN(success, TriggerFactory, "Trigger init failed", false);

            TriggerTask task;
            task.trigger_name = trigger_name;
            task.priority = trigger_priority;
            task.trigger = std::move(trigger);
            task.strategyConfig = strategy_config_;
            scheduler_->AddTask(task);
        }

        return success;
    }

    /**
     * @brief Start the trigger scheduler.
     */
    void PostTriggerProcess() {
        if (scheduler_) {
            scheduler_->StartScheduling();
        }
    }

    size_t GetThreadPoolSize() {
        std::shared_lock<std::shared_mutex> lock(config_mutex_);
        size_t threads = 0;
        for (const auto& st : strategy_config_.strategies) {
            if (st.trigger.enabled) ++threads;
        }

        return threads > 0 ? threads : 1;
    }

private:
    /**
     * @brief Create an instance of a registered trigger.
     * @param trigger_name The name of the trigger.
     * @param strategyConfig The config file of the trigger.
     * @return A unique pointer to the created trigger, or nullptr if the trigger is not found.
     */
    std::shared_ptr<TriggerBase> CreateTrigger(const std::string& trigger_name,
                                               const strategy::StrategyConfig& strategyConfig) {
        CreatorFunc creator;
        {
            std::unique_lock<std::shared_mutex> lock(trigger_mutex_);
            auto it = triggers_.find(trigger_name);
            if (it == triggers_.end()) {
                LOG_ERROR("Trigger not found: %s", trigger_name.c_str());
                return nullptr;
            }

            creator = it->second;
        }

        auto trigger = creator();
        if (!trigger) return nullptr;

        return trigger;
    }

private:
    std::shared_mutex trigger_mutex_;
    std::shared_mutex config_mutex_;
    std::shared_ptr<senseAD::rscl::comm::Node> node_ptr_;
 //    stoic::cm::NodeHandle* nodeHandle_ = nullptr;

    std::unordered_map<std::string, CreatorFunc> triggers_;
    strategy::StrategyConfig strategy_config_;
    std::shared_ptr<Scheduler> scheduler_;
    mutable std::shared_mutex callback_mutex_;
    TriggerCallback callback_;
};

/**
 * @brief Macro for registering a trigger with the factory.
 * Automatically adds the trigger to the factory's registry.
 * @param TriggerClassName The class name of the trigger to register.
 */
#define SHADOW_REGISTER_TRIGGER(TriggerName)                                     \
    static bool Register##TriggerName = []  {                                    \
        auto factory = std::make_shared<shadow::trigger::TriggerFactory>();      \
        auto node = std::make_shared<senseAD::rscl::comm::Node>();               \
        factory->RegisterTrigger(#TriggerName, [node] {                          \
            return std::make_shared<TriggerName>(node);                          \
        });                                                                      \
        LOG_INFO("Trigger registered successfully: %s", #TriggerName);           \
        return true;                                                             \
    }();


#define REGISTER_TRIGGER(TriggerName)                                            \
    inline bool Register##TriggerName(std::shared_ptr<TriggerFactory> factory,   \
                                      const std::shared_ptr<senseAD::rscl::comm::Node>& node) { \
        if (!node) {                                                             \
            LOG_WARN("node is null when registering trigger: %s", #TriggerName); \
            return false;                                                        \
        }                                                                        \
        factory->RegisterTrigger(#TriggerName, [node] {                          \
            return std::make_shared<TriggerName>(node);                          \
        });                                                                      \
        LOG_INFO("Trigger registered successfully: %s", #TriggerName);           \
        return true;                                                             \
    }       
    
} // namespace trigger
} // namespace shadow


#endif //TRIGGER_FACTORY_H
