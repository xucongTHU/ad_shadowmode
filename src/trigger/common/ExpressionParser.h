//
// Created by xucong on 25-5-8.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_EXPRESSIONPARSER_H
#define SHADOW_MODE_EXPRESSIONPARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

namespace shadow {
namespace trigger {

class ExpressionParser {
public:
    ExpressionParser();
    ~ExpressionParser();

    ExpressionParser(const ExpressionParser&) = delete;
    ExpressionParser& operator=(const ExpressionParser&) = delete;

    bool compile(const std::string& expr_str, bool use_cache = true);

    bool set_variable(const std::string& name, double value);
    void set_variables(const std::unordered_map<std::string, double>& vars);

    bool evaluate(bool& result);

    std::vector<std::string> get_variable_names() const;
    std::string last_error() const;

    void clear_cache();

    void add_function(const std::string& name, double (*func)(double));

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}
}

#endif //SHADOW_MODE_EXPRESSIONPARSER_H
