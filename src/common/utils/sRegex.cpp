//
// Created by xucong on 25-5-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "sRegex.h"

namespace shadow {
namespace common {

bool IsMatch(const std::string &path, const std::string &regexPattern) {
    try
    {
        regex pattern(regexPattern);
        return regex_match(path, pattern);
    }
    catch (const regex_error &e)
    {
        std::cerr << "Regex error: " << e.what() << std::endl;
        return false;
    }
}

}
}
