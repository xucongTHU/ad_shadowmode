//
// Created by xucong on 25-5-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_APPCONFIG_H
#define SHADOW_MODE_APPCONFIG_H

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace shadow {
namespace config {

class AppConfigData {
public:
    struct DataStorage {
        int rollingDeleteThreshold;
        int rollInterval;
        int bagInterval;
        std::unordered_map<std::string, std::string> storagePaths;
        uint64_t capacityMb;
        uint64_t requriedSpaceMb;
    }dataStorage;

    // mqtt
    struct Mqtt {
        std::string broker;
        std::string broker_ssl;
        std::string username;
        std::string password;
        std::string upTopic;
        std::string downTopic;
    };

    struct DataProto {
        std::string vin;
        std::string software_version;
        std::string hardware_version;
        std::string device;
        std::string device_id;
        Mqtt mqtt;
    }dataProto;

    // 配置数据上传相关的参数
    struct DataUpload {
        std::string gateway;
        std::string fileRecordPath;
        std::string clientCertPath;
        std::string clientKeyPath;
        std::string rsa_pub_key_path;
        std::string  caCertPath;
        size_t uploadFileSliceSizeMb;
        int64_t uploadFileSliceIntervalMs;
        int8_t retryCount;
        int64_t retryIntervalSec;
        std::unordered_map<std::string, std::string> uploadPaths;
        std::string  filenameRegex;
        int64_t uploadFileIntervalMs;
        std::string watch_dir;
        std::string enc_dir;
    }dataUpload;

    struct Log {
        std::string logLevel;
        std::string logPattern;
        std::string logPath;
        std::string logBasename;
    }log;

    struct Debug {
        bool closeMqttSsl;
        bool closeDataReporter;
        bool closeDataStorage;
        bool closeDataEnc;
        bool closeDataUpload;
        bool deleteFileAfterDataUpload;
        bool closeLogUpload;
        int cloudtimeOutMs;
    }debug;

};

class AppConfig {
public:
    static AppConfig& getInstance();

    bool Init(const std::string& filePath);
    AppConfigData GetConfig() const;

private:
    AppConfig() = default;
    ~AppConfig() = default;
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    bool CheckValid(const std::string& jsonString);
    bool checkFileExists(const std::string& filePath) const;
    bool checkJsonFormat(const nlohmann::json& jsonData) const;

    nlohmann::json configData;
    AppConfigData parsedConfig;
};

} // namespace config
} // namespace shadow

#endif //SHADOW_MODE_APPCONFIG_H
