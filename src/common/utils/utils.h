//
// Created by xucong on 25-5-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_UTILS_H
#define SHADOW_MODE_UTILS_H

#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <chrono>
#include <iomanip>
#include <random>

#include "nlohmann/json.hpp"
#include "../data.h"

namespace shadow {
namespace common {
using json = nlohmann::json;

std::string Vin();
json ParseJsonFromString(const std::string &resp);
bool IsDirExist(const std::string &path);
bool DeleteFile(const std::string &path);
bool DeleteFiles(const std::vector<std::string>& inputFilePaths);
void saveDataToFile(const std::string &data, const std::string &filePath);
bool ensureDirectoryExists(const std::string &path);
bool ForceCreateDirRecursive(const std::string &path);
std::string MakeRecorderFileName(const std::string& triggerId, const std::string& businessType, uint64_t triggerTimestamp);
std::string RenameRecordFile(const std::string& path);
std::string FindFilesWithAllSuffix(const std::string& filePath);
std::string ReplaceSubstring(std::string str, const std::string& from, const std::string& to);
bool RenameFile(const std::string& old_path, const std::string& new_path);

TBussiness GetBusinessType(const std::string &bt);
std::string readFileToString(const std::string& filePath);
std::string getRandMsgID();
uint64_t GetCurrentTimestampNs();
long long getTime();
std::string get14DigitTimestamp();
long generateTimestamp();
std::string getCurrentTimeFormatted();
std::string TimestampNs2Str(uint64_t timestamp);
std::string UnixSecondsToString(
    uint64_t unix_seconds,
    const std::string& format_str = "%Y-%m-%d-%H:%M:%S");
std::string trim(const std::string &s);
void jsonFormater(json& infoJson, json& statusJson);
uint64_t MonoTime();
double ToSecond(uint64_t nanoseconds_);
std::string getTokenContent(const std::string& devId, const std::string& vin);

} // namespace common
} // namespace shadow


#endif //SHADOW_MODE_UTILS_H
