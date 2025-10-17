//
// Created by xucong on 25-5-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "utils.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <strategy/ConfigFileWatcher.hpp>

namespace shadow {
namespace common {
namespace fs = std::filesystem;

std::string Vin() { return getenv("VIN"); }

json ParseJsonFromString(const std::string &resp) {
    return json::parse(resp);
}

bool IsDirExist(const std::string &path) {
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        closedir(dir);
        return true;
    }
    else
    {
        return false;
    }
    // return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

bool DeleteFile(const std::string &path)
{
    return ::remove(path.c_str()) == 0;
}

bool DeleteFiles(const std::vector<std::string>& inputFilePaths) {
    for(const auto& filePath : inputFilePaths){
        if (fs::exists(filePath)) {
            try {
                fs::remove(filePath);
                std::cout << "File deleted successfully: " << filePath << std::endl;
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error deleting file: " << e.what() << std::endl;
                return false;
            }
        } else {
            std::cerr << "File does not exist: " << filePath << std::endl;
        }
    }
    return true;
}


bool ensureDirectoryExists(const std::string &path) {
#ifdef _WIN32
    return _mkdir(path.c_str()) == 0 || errno == EEXIST;
#else
    return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
#endif
}


void saveDataToFile(const std::string &data, const std::string &filePath) {
    size_t pos = filePath.find_last_of('/');
    if (pos != std::string::npos)
    {
        std::string dirPath = filePath.substr(0, pos);
        ensureDirectoryExists(dirPath);
    }

    std::ofstream ofs(filePath, std::ios::out | std::ios::trunc); 
    if (ofs.is_open())
    {
        ofs << data << "\n";
        ofs.close();
        std::cout << "Saved to: " << filePath << std::endl;
    }
    else
    {
        std::cerr << "Failed to save to: " << filePath << std::endl;
    }
}

bool ForceCreateDirRecursive(const std::string& path) {
    try {
        std::filesystem::path dir_path(path);
        bool created = std::filesystem::create_directories(dir_path);
        if (!created && !std::filesystem::is_directory(dir_path)) {
            std::cerr << "Create directory recursively failed.\n";
            return false;
        }
        std::cout << "Recursively create directory " << path << " ok.\n";
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error creating directory recursively: " << e.what() << std::endl;
        return false;
    }
}

std::string MakeRecorderFileName(const std::string& triggerId, const std::string& businessType, uint64_t triggerTimestamp) {
    auto t_business = GetBusinessType(businessType);
    std::string data_source = t_business.data_source.empty() ? "Other" : t_business.data_source;
    std::string data_type = t_business.data_type.empty() ? "AutoDrivingData" : t_business.data_type;
    std::stringstream ss;
    ss << Vin() << "_" << data_source << "_" << UnixSecondsToString(triggerTimestamp, "%Y%m%d%H%M%S") << "_" << data_type
        << "_" << businessType << "_" << triggerId << ".recording";
    return ss.str();
}

TBussiness GetBusinessType(const std::string &bt) {
    for (const auto& bs : kBussiness) {
        if (bs.bussiness_type == bt) {
            return bs;
        }
    }
    return TBussiness();
}

std::string ReplaceSubstring(std::string str, const std::string& from, const std::string& to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::string FindFilesWithAllSuffix(const std::string& filePath) {
    fs::path path(filePath);
    fs::path directory = path.parent_path();
    std::string fileName = path.stem().string(); 

    std::vector<std::string> foundFiles;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry.status())) {
            std::string currentFileName = entry.path().filename().string();
            if (currentFileName.find(fileName) == 0) {
                foundFiles.push_back(entry.path().string());
                // std::cout << "Found file: " << entry.path() << std::endl;
            }
        }
    }

    if (!foundFiles.empty()) {
        return foundFiles[0];
    } else {
        return "";
    }
}

bool RenameFile(const std::string& old_path, const std::string& new_path) {
    try {
        fs::path old_p(old_path);
        fs::path new_p(new_path);
        if (!fs::exists(old_p)) {
            std::cerr << "RenameFile failed, old file not exist. old_path: " << old_path << std::endl;
            return false;
        }

        fs::rename(old_p, new_p);
        // std::cout << "RenameFile ok. old_path: " << old_path << ", new_path: " << new_path << std::endl;
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "RenameFile error: " << e.what() << std::endl;
        return false;
    }
}

std::string RenameRecordFile(const std::string& path) {
    // std::string FindFilesWithAllSuffix(const std::string& filePath);
    std::string fuzzyPath =  FindFilesWithAllSuffix(path);

    std::string newPath = ReplaceSubstring(path, ".recording.00000.rsclbag", ".rsclbag");
    if(newPath.find(".rsclbag") != std::string::npos && RenameFile(path, newPath))
    {
        std::cout << "RenameRecordFile ok. from:" << path << ", to:" << newPath << std::endl;
        return newPath;
    }

    std::cerr << "RenameRecordFile failed. from:" << path << ", to:" << newPath << std::endl;
    return "";
}

std::string readFileToString(const std::string& filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cout << "readFileToString failed, file not exist. filePath: " << filePath << std::endl;
        return "";
    }
    
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "readFileToString failed, file open failed. filePath: " << filePath << std::endl;
        return "";
    }
    

    auto fileSize = std::filesystem::file_size(filePath);
    std::string content(8, '\0');
    
    file.read(content.data(), fileSize);
    return content;
}


std::string getCurrentTimeFormatted() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    auto timer = std::chrono::system_clock::to_time_t(now);
    std::tm bt = *std::localtime(&timer);
    
    std::ostringstream oss;
    oss << std::put_time(&bt, "%Y%m%d%H%M%S");
    oss << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}

std::string UnixSecondsToString(
    uint64_t unix_seconds,
    const std::string& format_str) {
  std::time_t t = unix_seconds;
  struct tm ptm;
  struct tm* ret = localtime_r(&t, &ptm);
  if (ret == nullptr) {
    return std::string("");
  }
  uint32_t length = 64;
  std::vector<char> buff(length, '\0');
  strftime(buff.data(), length, format_str.c_str(), ret);
  return std::string(buff.data());
}

std::string TimestampNs2Str(uint64_t timestamp) {
    std::stringstream ss;
    std::chrono::nanoseconds duration(timestamp);
    auto target_time = std::chrono::system_clock::time_point(duration);
    std::time_t time = std::chrono::system_clock::to_time_t(target_time);
    std::tm* tm_info = std::localtime(&time);
    ss << std::put_time(tm_info, "%Y%m%d%H%M%S");
    return ss.str();
}


// 生成指定长度的随机字符串
std::string generateRandomString(size_t length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, sizeof(alphanum) - 2);
    
    std::string randomStr;
    randomStr.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        randomStr += alphanum[distribution(generator)];
    }
    
    return randomStr;
}

std::string getRandMsgID() {
    std::string dateStr = getCurrentTimeFormatted();
    std::string randomStr = generateRandomString(6);
    return dateStr + randomStr;
}

uint64_t GetCurrentTimestampNs()
{
    // auto cur_clock_mode = senseAD::base::time::ClockMode::SYSTEM_TIME;
    // return senseAD::base::time::Time::Now(&cur_clock_mode).ToNanosecond();
    
    auto now = std::chrono::system_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
    return ns.count();
}

// 返回当前时间的 Unix 时间戳（毫秒，UTC 时区）
long long getTime() {
    auto now = std::chrono::system_clock::now();    
    auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    );    
    return milliseconds_since_epoch.count();
}

std::string get14DigitTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&t);

    int year = tm->tm_year + 1900; 
    std::cout << "Current year: " << tm->tm_year << std::endl;    
    int month = tm->tm_mon + 1;           
    int day = tm->tm_mday;
    int hour = tm->tm_hour;
    int minute = tm->tm_min;
    int second = tm->tm_sec;

    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << year 
        << std::setw(2) << std::setfill('0') << month
        << std::setw(2) << std::setfill('0') << day
        << std::setw(2) << std::setfill('0') << hour
        << std::setw(2) << std::setfill('0') << minute
        << std::setw(2) << std::setfill('0') << second;

    return oss.str();
}

long generateTimestamp() {
    auto now = std::chrono::system_clock::now();    
    auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    );    
    return milliseconds_since_epoch.count();
}

void jsonFormater(json& infoJson, json& statusJson) { 
    for (auto& [key, value] : statusJson.items()) {
        std::cout << "Key: " << key << ", Value: " << value << std::endl;
        json itemJson;
        itemJson["paraName"] = key;   
        itemJson["paraValue"] = value;     
        infoJson["status"].emplace_back(itemJson);     
    }
} 


uint64_t MonoTime() {
  auto now = std::chrono::steady_clock::now();
  auto nano_time_point =
      std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
  auto epoch = nano_time_point.time_since_epoch();
  uint64_t now_nano =
      std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
  return now_nano;
}

double ToSecond(uint64_t nanoseconds_) {
  return static_cast<double>(nanoseconds_) / 1000000000UL;
}


std::string getTokenContent(const std::string& devId, const std::string& vin) {
    json json_token;
    json content;
    content["lat"] = "312914778";
    content["lon"] = "1212056188";
    content["vin"] = vin;
    
    // 设置外层字段
    json_token["content"] = content.dump(); 
    json_token["devId"] = devId;
    json_token["devType"] = 11;
    json_token["isEncrypt"] = 0;
    json_token["msgId"] = common::getRandMsgID(); 
    json_token["msgType"] = "obu01";
    json_token["oemId"] = 1;
    json_token["requester"] = 2;
    json_token["timeStamp"] = common::getTime();
    json_token["verType"] = "OBU-MQTT";
    json_token["version"] = "v1.3";
    
   
    return json_token.dump(4);
}


std::string trim(const std::string &s)
{
    if (s.empty())
        return s;

    auto left = s.begin();
    while (left != s.end() && std::isspace(*left))
        ++left;

    auto right = s.end();
    do
    {
        --right;
    } while (std::isspace(*right) && right > left);

    return std::string(left, right + 1);
}

} // namespace common
} // namespace shadow