/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */
/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#pragma once

#include <list>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace rscl {
namespace mainboard {

class ModuleArgument {
 public:
    ModuleArgument() = default;
    virtual ~ModuleArgument() = default;
    void DisplayUsage();
    void PrintInfo() const;
    void ParseArgument(int argc, char* const argv[]);
    void GetOptions(const int argc, char* const argv[]);

    const std::pair<int, char**>& GetMainArg() const;
    const std::string& GetBinaryName() const;
    const std::string& GetSchedName() const;
    const std::string& GetProcessName() const;
    const std::string& GetOfflineParamPath() const;
    const std::list<std::string>& GetDAGConfList() const;
    const std::unordered_map<std::string, std::vector<std::string>>&
    GetCustomParamList() const;

 private:
    void SearchNodeDagConf(const std::string& node_name);
    std::string cmd_;
    std::list<std::string> dag_conf_list_;
    std::unordered_map<std::string, std::vector<std::string>>
        custom_param_list_;
    std::string binary_name_;
    std::string process_name_;
    std::string offline_param_path_;
};

inline const std::string& ModuleArgument::GetBinaryName() const {
    return binary_name_;
}

inline const std::string& ModuleArgument::GetProcessName() const {
    return process_name_;
}

inline const std::string& ModuleArgument::GetOfflineParamPath() const {
    return offline_param_path_;
}

inline const std::list<std::string>& ModuleArgument::GetDAGConfList() const {
    return dag_conf_list_;
}

inline const std::unordered_map<std::string, std::vector<std::string>>&
ModuleArgument::GetCustomParamList() const {
    return custom_param_list_;
}

}  // namespace mainboard
}  // namespace rscl
}  // namespace senseAD
