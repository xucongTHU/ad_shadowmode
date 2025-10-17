//
// Created by xucong on 25-5-6.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_STRATEGYCONFIG_H
#define SHADOW_MODE_STRATEGYCONFIG_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

namespace shadow {
namespace strategy {

enum class TriggerEngineType : uint8_t {
    UNKNOWN = 0,
    RULE_ENGINE,   /// 规则触发引擎
    AI_ENGINE,     /// AI触发引擎
    SHADOW_ENGINE  /// 影子模式触发引擎
};

enum class TriggerEventType : uint8_t {
    KPIMetrics = 10,              // KPI统计
    Accident,                     // 事故
    ActiveSafetyTrigger,          // 主动安全功能触发类
    RiskEmergencyOperation,       // 风险及紧急操作类
    AbnormalEmergencyTakeover,    // 异常紧急接管类
    FunctionActivationLimitation, // 功能退出/激活受限
    DriverTakeover,               // 驾驶员接管类
    Other                         // 其他
};

struct Trigger {
    std::string triggerName;
    std::string triggerId;
    int8_t priority;
    bool enabled;
    std::string triggerCondition;
};

struct CacheMode {
    int forwardCaptureDurationSec;
    int backwardCaptureDurationSec;
    int cooldownDurationSec;
};

struct Mode {
    int triggerMode;
    CacheMode cacheMode;
};

struct Channel {
    std::string topic;
    std::string type;
    int originalFrameRate;
    int capturedFrameRate;
};

struct Dds {
    std::vector<Channel> channels;
};

struct Cyber {
    std::vector<Channel> channels;
};

struct Strategy {
    std::string businessType;
    Trigger trigger;
    Mode mode;
    bool enableMasking;
    Dds dds;
    Cyber cyber;
    std::unordered_map<std::string, std::string> upload;
};

struct StrategyConfig {
    std::string configId;
    int strategyId;
    std::vector<Strategy> strategies;
};

}
}
#endif //SHADOW_MODE_STRATEGYCONFIG_H
