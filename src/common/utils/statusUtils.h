//
// Created by ymm on 25-7-11.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_STATUS_UTILS_H
#define SHADOW_MODE_STATUS_UTILS_H

#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h> 
#include <chrono>
#include <iomanip>
#include <random>
#include <unistd.h>
#include <sys/statvfs.h>
#include "nlohmann/json.hpp"

#include "../data.h"

namespace shadow {
namespace common {

CPUData readCPUData();
double calculateCPUPercentage(const CPUData& oldData, const CPUData& newData);
void getMemData(SysInfo& sys_info);
void getSpaceData(SysInfo& sys_info, const char *path);

} // namespace common
} // namespace shadow


#endif //SHADOW_MODE_STATUS_UTILS_H
