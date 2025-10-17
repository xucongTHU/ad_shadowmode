//
// Created by xucong on 25-5-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#pragma once

#include <stdlib.h>
#include <string.h>

#include <iostream>
// #include "yaml-cpp/yaml.h"

namespace shadow {
namespace common {

inline char* getInstallRootPath() {
  char* install_root_path = getenv("AD_SHADOWMODE_INSTALL_ROOT_PATH");
  if (NULL != install_root_path) {
    return install_root_path;
  } else {
    std::cerr
        << "the environment variable 'AD_SHADOWMODE_INSTALL_ROOT_PATH' has not been set. "
           "Please refer to the environment variable setting document on the relevant platform."
        << std::endl;
    exit(-1);
  }
}

inline char* getResourceRootPath() {
  char* install_root_path = getenv("AD_SHADOWMODE_RESOURCE_ROOT_PATH");
  if (NULL != install_root_path) {
    return install_root_path;
  } else {
    std::cerr
        << "the environment variable 'AD_SHADOWMODE_RESOURCE_ROOT_PATH' has not been set. "
           "Please refer to the environment variable setting document on the relevant platform."
        << std::endl;
    exit(-1);
  }
}

inline char* getCarId() { return getenv("CAR_ID"); }

inline std::string getCarIdPath() {
  return std::string(getResourceRootPath()) + "/cfg/car_id/" + std::string(getCarId());
}

inline std::string getAlgPath() { return std::string(getResourceRootPath()) + "/cfg/alg/"; }

inline std::string getFVisionModelPath() { return std::string(getInstallRootPath()) + "/config/front_vision/"; }

inline std::string getBEVVisionModelPath() { return std::string(getInstallRootPath()) + "/config/bev_vision/"; }

// template <typename _T>
// void loadNode(YAML::Node &node, _T &out, std::string name) {
//   if (node[name] != nullptr) {
//     out = node[name].as<_T>();
//   } else {
//     std::cout << "Cannot load %s from config" <<  name.c_str();
//   };
// }

}  // namespace common
}  // namespace shadow
