/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <cstdio>
#include <string>
#include <vector>

#include "ad_log/ad_log.hpp"
#include "yaml-cpp/yaml.h"

#include "ad_rscl/idl/launch_param.pb.h"
#include "ad_rscl/idl/global_config.pb.h"

namespace senseAD {
namespace rscl {
namespace common {
using senseAD::RuntimeResourceConfig;

/**
 * @brief Check whether the condition expression should be true
 *
 * @param process_name
 * @param condition
 * @return true
 * @return false
 */
bool EvalCondition(const std::string &process_name,
                   const senseAD::rscl::idl::Condition &condition);

bool GetSensorSchemeYaml(const std::string &module,
                         YAML::Node *node,
                         const std::string &vehicle_id = "",
                         std::vector<std::string> *config_yaml_paths = nullptr);
bool GetSensorConfigFolder(std::string *config_folder,
                           const std::string &config_source_folder = "",
                           const std::string &vehicle_id = "");

bool GetPerceptionVehicleYamlConfigPath(std::string *config_yaml_path,
                                        const std::string &vehicle_id = "");

void DumpConfigNode(const YAML::Node &config_node,
                    const std::string &dump_path = "");

bool GetVehicleSchemeFromMap(const std::string &scheme_map,
                             const std::string &vehicle_id,
                             std::string *scheme_name);

void DFSPrintYaml(YAML::Node node, const std::string &path = "");

//*************************************************
//       For Config Parsing
//*************************************************
bool MatchRegexStringInConfig(const std::string &regex_string,
                              const std::string &target_string);

namespace config_parse {
/**
 * @brief Replace env variable with actual value in param file
 * @param target_str The target string which need to be processed
 */
void ReplaceEnv(std::string *target_str);

/**
 * @brief Replace arg variable with actual value in param file
 * @param target_str The target string which need to be processed
 * @param arg_node The arg yaml node
 */
void ReplaceArg(std::string *target_str, YAML::Node arg_node);

/**
 * @brief Replace getscheme variable with actual value in param file
 * @param target_str The target string which need to be processed
 * @param arg_node The arg yaml node
 */
void ReplaceGetScheme(std::string *target_str, YAML::Node arg_node);

/**
 * @brief Replace custom variable with actual value in param file
 * @param target_str The target string which need to be processed
 * @param arg_node The arg yaml node
 */
void ReplaceCustom(std::string *target_str, YAML::Node arg_node);

}  // namespace config_parse

}  // namespace common
}  // namespace rscl
}  // namespace senseAD
