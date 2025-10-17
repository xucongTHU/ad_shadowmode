/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <unordered_map>
#include <regex>

namespace senseAD {
namespace base {
namespace common {

std::string ProcessName();

bool GetEnvironmentVariable(const std::string &key, std::string *val);
bool GetEnvironmentVariable(const std::string &key, int64_t *val);
bool GetEnvironmentVariable(const std::string &key, double *val);

bool GetEnvironmentVariableBool(const std::string &name, bool default_value);
bool SetEnvironmentVariable(const std::string &key,
                            const std::string &value,
                            bool overwrite = true);
std::unordered_map<std::string, std::string> ListEnvironmentVariables(
    const std::regex &pattern);

/**
 * @brief Replace all $(optenv env_name|default) to the actual env value
 *
 * @param target_str
 * @return std::string
 */
std::string ReplaceEnv(const std::string &target_str);

bool GetVehicleId(std::string *vehicle_id);

std::string SenseautoInstalledRoot();
std::string SenseautoConfigDir();
std::string SenseautoVehicleConfigDir();
std::string SenseautoPerceptionConfigDir();
std::string SenseautoRsclConfigDir(const std::string &process_name = "");
std::string SenseautoRsclInternalConfigDir();

void SetSenseautoConfigDir(const std::string &dir);
void SetSenseautoRsclConfigDir(const std::string &dir);

std::string CurrentWorkingDirectory();
void SetCurrentWorkingDirectory(const std::string &dir);
std::string SenseautoRsclConfigDir();
std::string SenseautoRsclInternalConfigDir();

void SetSenseautoConfigDir(const std::string &dir);
void SetSenseautoRsclConfigDir(const std::string &dir);

std::string CurrentWorkingDirectory();

std::string GetDaemonSlaveIp();

bool MatchRegexStringInConfig(const std::string &regex_string,
                              const std::string &target_string);

}  // namespace common
}  // namespace base
}  // namespace senseAD
