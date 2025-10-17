//
// Created by xucong on 25-5-8.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_TRIGGERCONDITIONCHECKER_H
#define SHADOW_MODE_TRIGGERCONDITIONCHECKER_H

#include "ExpressionParser.h"
#include <variant>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>


namespace shadow {
namespace trigger {

class TriggerConditionChecker {
public:
    using Value = std::variant<double, int, bool>;

    struct ConditionElement {
        std::string variable;
        std::string comparison_op;
        std::variant<double, bool> threshold;
        std::string logical_op;

        std::string threshold_str() const {
            if (std::holds_alternative<double>(threshold)) {
                return std::to_string(std::get<double>(threshold));
            }
            return std::get<bool>(threshold) ? "true" : "false";
        }
    };

    TriggerConditionChecker();
    ~TriggerConditionChecker();

    // 禁止拷贝和赋值
    TriggerConditionChecker(const TriggerConditionChecker&) = delete;
    TriggerConditionChecker& operator=(const TriggerConditionChecker&) = delete;

    bool parse(const std::string& condition);
    bool check(const std::unordered_map<std::string, Value>& variables);
    std::string last_error() const;
    std::vector<ConditionElement> get_elements() const;

private:
    std::unique_ptr<ExpressionParser> parser_;
    std::vector<ConditionElement> elements_;
    std::string last_error_;

    void extract_elements(const std::string& condition);
    static std::vector<std::pair<std::string, std::string>> split_logical_ops(const std::string& expr);
    static std::string trim(const std::string& s);
};


}
}

#endif //SHADOW_MODE_TRIGGERCONDITIONCHECKER_H
