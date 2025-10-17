//
// Created by xucong on 25-4-2.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_STRATEGY_PARSER_H
#define SHADOW_MODE_STRATEGY_PARSER_H

#include "nlohmann/json.hpp"
#include "StrategyConfig.h"
#include "config/AppConfig.h"

namespace shadow {
namespace strategy {

using shadow::config::AppConfigData;
using json = nlohmann::json;

class StrategyConfigParser {
public:
    static bool LoadConfigFromFile(const std::string& file_path, StrategyConfig& conf);
    
     /**
     * @brief 获取支持的trigger类型
     * @param trigger_vec trigger类型列表。
     * @return 如果获取成功，返回 `true`；否则返回 `false`。
     */
    static bool getTriggerType(const std::string& filepath, std::vector<std::string>& trigger_vec); 

     /**
     * @brief 检查 JSON 消息的有效性
     *
     * 该函数用于验证接收到的 JSON 消息是否符合预期的结构和内容要求。
     *
     * @param j 需要检查的 JSON 对象。
     * @return 如果消息有效，返回 `true`；否则返回 `false`。
     */
    static bool CheckMessage(const json &j, std::vector<std::string>& trigger_vec, int8_t& bag_duration);

private:
    static bool CheckValid(const std::string& jsonString);
    static void ParseJsonConfig(const nlohmann::json& jsonData, StrategyConfig& config);

};

} // namespace strategy
} // namespace shadow

#endif //SHADOW_MODE_STRATEGY_PARSER_H
