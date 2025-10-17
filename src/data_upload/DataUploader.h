#ifndef SHADOW_MODE_DATAUPLOAD_H
#define SHADOW_MODE_DATAUPLOAD_H

#include <string>
#include <queue>
#include <memory>
#include <mutex>
#include <iostream>
#include <filesystem>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <atomic>

#include "data_proto/CurlWrapper.h"
#include "FileStatusManager.h"
#include "common/data.h"
#include "config/AppConfig.h"
#include "data_encryption/DataEncryption.h"
#include "data_upload/DataReporter.h"


namespace shadow {
namespace data_upload {

namespace fs = std::filesystem;

class DataUploader {
  public:
    enum ErrorCode {
      SUCCESS = 0,
      TIMEOUT = 1,
      CONNECT_ERROR = 2,
      FILE_NOT_FOUND = 3,
      FILE_CHUNK_ERROR = 4,
      URL_ERROR = 5,
      INVALID_RESPONSE = 6,
      UPLOAD_INCOMPLETE = 7,
      UNKNOWN_ERROR = 8,
    };

    DataUploader() = default;
    ~DataUploader();

    /**
     * @brief 初始化数据上传
     * @param config 上传配置
     * @return 是否初始化成功
     */
    bool Init(const config::AppConfigData::DataUpload& config, 
              std::shared_ptr<data_report::DataReporter> data_reporter);
    
    /**
     * @brief 启动服务（自动加密并触发上传）
     */
    bool Start();

     /**
     * @brief 停止服务
     */
    bool Stop();
    ErrorCode UploadFile(const std::string& full_path);
    ErrorCode GetUploadUrl(const common::UploadUrlReq& req, std::string& resp);
    ErrorCode UploadFeedback(const common::FeedBackReq& req, std::string& resp);
    ErrorCode GetQueryTask(const std::string& vin, std::string& resp);
    ErrorCode CompleteUpload(const common::CompleteUploadReq& req, std::string& resp);
    ErrorCode GetUploadStatus(const std::string& file_uuid, std::string& resp);

  private:
    ErrorCode CurlErrorMapping(CURLcode code);
    ErrorCode GetUploadInfo(const std::string& full_path, int chunk_count, common::FileUploadRecord& record);
    void Run();
    void LoadFileList();
    void ProcessQueue();
    void GetUploadBagInfo(shadow::common::FileUploadProgress& upload_progress);

    CurlWrapper curl_wrapper_;
    std::unique_ptr<FileStatusManager> file_status_manager_;
    config::AppConfigData::DataUpload config_;
    std::queue<std::string> upload_queue_;
    std::thread worker_thread_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stop_flag_;
    std::unique_ptr<shadow::data_encryption::DataEncryption> encryptor_;
    std::shared_ptr<data_report::DataReporter> data_reporter_;

};

}
}


#endif //SHADOW_MODE_DATAUPLOAD_H
