/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

namespace senseAD {
namespace rscl {
namespace component {

class GeneralParamLoader {
 public:
    explicit GeneralParamLoader(YAML::Node* node) : inner_config_node_(node) {}

    template <typename T>
    bool GetParam(const std::string& key, T& container);  // NOLINT

 private:
    YAML::Node* inner_config_node_;
};

template <typename T>
bool GeneralParamLoader::GetParam(const std::string& key,
                                  T& container) {  // NOLINT
    std::vector<std::string> param_name_vec;
    std::string delimiter = "/";
    size_t last = 0;
    size_t next = 0;
    while ((next = key.find(delimiter, last)) != std::string::npos) {
        param_name_vec.emplace_back(key.substr(last, next - last));
        last = next + 1;
    }
    param_name_vec.emplace_back(key.substr(last, key.length()));

    auto trav_node_func = [&](YAML::Node* node) {
        std::vector<YAML::Node> traversal;
        traversal.push_back(*node);
        for (size_t idx = 0; idx < param_name_vec.size() - 1; idx++) {
            if ((traversal.back())[param_name_vec[idx]]) {
                traversal.push_back((traversal.back())[param_name_vec[idx]]);
            } else {
                return false;
            }
        }
        if ((traversal.back())[param_name_vec.back()]) {
            try {
                container = (traversal.back())[param_name_vec.back()].as<T>();
            } catch (YAML::BadConversion& e) {
                YAML::Emitter emitter;
                emitter << (traversal.back())[param_name_vec.back()];
                AD_LFATAL(CONFIG) << "Unable to cast " << emitter.c_str()
                                  << " for config " << key
                                  << ". Error message: " << e.what();
                exit(EXIT_FAILURE);
            }
        } else {
            return false;
        }
        return true;
    };

    bool load_succ = false;
    if (!trav_node_func(inner_config_node_)) {
        if ((*inner_config_node_)["node_param"]) {
            auto iter_node = (*inner_config_node_)["node_param"];
            load_succ = trav_node_func(&iter_node);
        }
    } else {
        load_succ = true;
    }
    return load_succ;
}

}  // namespace component
}  // namespace rscl
}  // namespace senseAD
