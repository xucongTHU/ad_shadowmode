/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <unordered_map>

namespace senseAD {
namespace rscl {
namespace component {

class ComponentBase;

class EnvParamImpl {
 public:
    explicit EnvParamImpl(ComponentBase *c);

    /**
     * @brief Call after the node created
     *
     */
    void InitializeOnce();

    bool GetEnvParam(const char *param_name,
                     std::string *param_str,
                     bool allow_global) const;

 private:
    bool ReLoadEnvConf();

    ComponentBase *c_;
    std::unordered_map<std::string, std::string> env_map_;
};

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
