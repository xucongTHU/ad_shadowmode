//
// Created by xucong on 25-5-30.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_FILEROLLER_H
#define SHADOW_MODE_FILEROLLER_H

#include <string>
#include <vector>
#include <optional>
#include "config/AppConfig.h"

namespace shadow {
namespace data_storage {

class FileRoller {
public:
    FileRoller();
    ~FileRoller() = default;

    int rollFiles();

private:
    std::string bagPath;

    std::vector<std::string> getSortedCompressedFiles() const;

};

}
}
#endif //SHADOW_MODE_FILEROLLER_H
