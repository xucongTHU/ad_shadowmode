#include "DataUploader.h"
#include <cstring>
#include <fstream>
#include <filesystem>
#include <map>
#include <chrono>
#include <iomanip>
#include <nlohmann/json.hpp>

#include "FileSplitter.hpp"
#include "common/utils/utils.h"
#include "common/utils/sRegex.h"
#include "common/logger/Logger.h"
#include "common/data.h"

using json = nlohmann::json;

namespace shadow {
namespace data_upload {

bool DataUploader::Init(const config::AppConfigData::DataUpload& config,
                        std::shared_ptr<data_report::DataReporter> data_reporter) {
    config_ = config;
    data_reporter_ = data_reporter;   
    stop_flag_ = false;

    encryptor_ = std::make_unique<shadow::data_encryption::DataEncryption>();                  
    if (!encryptor_->Init(config.rsa_pub_key_path, config.watch_dir, config.enc_dir)) {
        LOG_ERROR("Encryptor init failed !");
        return false;   
    }

    LOG_INFO("encryptorInit success!"); 

    file_status_manager_ = std::make_unique<FileStatusManager>(config_.fileRecordPath);
    auto ret = curl_wrapper_.Init(config_.clientCertPath, config_.clientKeyPath, config_.caCertPath);
    return ret == CURLE_OK;
}

DataUploader::~DataUploader() {
    Stop();
    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
}

bool DataUploader::Start()
{
    worker_thread_ = std::thread(&DataUploader::Run, this);

    return true;
}

//record用于存储文件上传信息   
//检查文件是否有上传记录，获取文件的上传状态并根据需要返回相关的上传信息。
//如果文件没有上传记录，则发起请求来获取上传的 URL 和相关信息
DataUploader::ErrorCode DataUploader::GetUploadInfo(const std::string& full_path, int chunk_count, common::FileUploadRecord& record) {
    record.uploaded_url_map.clear();
    //检查是否已有文件上传记录
    if (file_status_manager_->GetFileRecord(full_path)) {
        record = file_status_manager_->GetFileRecord(full_path).value();
        record.start_chunk = 0;//从文件起始位置开始上传
        std::string resp;
        auto ret = GetUploadStatus(record.file_uuid, resp);
        if (ret != ErrorCode::SUCCESS) {
            LOG_ERROR("Failed to get upload status.");   
            return ret;
        }
        json j = common::ParseJsonFromString(resp);
        std::cout << j << std::endl;
        if (!j.contains("statusCode") || !j.contains("data") || !j["data"].contains("uploadStatus") || !j["data"].contains("uploadedPartList")) {
            LOG_ERROR("Invalid URL response.");   
            return ErrorCode::INVALID_RESPONSE;
        }
        if (j["statusCode"] != "0") {
            LOG_ERROR("Status code is abnormal.");   
            return ErrorCode::INVALID_RESPONSE;
        }  
        if (j["data"]["uploadStatus"] == 3) {
            record.start_chunk = chunk_count+1;
            return ErrorCode::SUCCESS;
        }
        if (j["data"]["uploadedPartList"].is_array()) {    
            // record.start_chunk = j["data"]["uploadedPartList"].size();
            for (const auto& uploadedPart: j["data"]["uploadedPartList"]) {
                record.uploaded_url_map.emplace(uploadedPart["partNumber"].get<int>(),uploadedPart["etag"]);
            }
            return ErrorCode::SUCCESS;
        }
    } else {
        common::UploadUrlReq upload_req;
        upload_req.type = common::UploadType::ActivelyReport;
        upload_req.part_number = chunk_count;
        upload_req.filename = fs::path(full_path).filename().string();
        upload_req.vin = common::Vin();
        
        //模拟上传URL的获取过程，并将结果存入record.uploadedUrlMap中。 
        // for(int i=0; i<chunk_count; i++){
        //     record.upload_url_map.emplace(i+1, "http:");
        // }  
        // record.file_uuid ="sss";
        // record.upload_id = "123456789";
        // record.chunk_count = chunk_count;
        // record.start_chunk = 0;
        // file_status_manager_->AddFileRecord(full_path, record);
        // return ErrorCode::SUCCESS;

        std::string resp;
        auto ret = GetUploadUrl(upload_req, resp);
        if (ret != ErrorCode::SUCCESS) {
            LOG_ERROR("Failed to get upload url.");   
            return ret;
        }
        json j = common::ParseJsonFromString(resp);
        if (!j.contains("statusCode") || !j.contains("data") || !j["data"].contains("partPresignUploadUrlMap") ||
            !j["data"].contains("fileUuid") || !j["data"].contains("uploadId")) {
            LOG_ERROR("Invalid URL response.");   
            return ErrorCode::INVALID_RESPONSE;
        }
        if (j["statusCode"] != "0") {
            LOG_ERROR("Status code is abnormal.");  
            return ErrorCode::INVALID_RESPONSE;
        }
        for (const auto& [slice_id, url] : j["data"]["partPresignUploadUrlMap"].get<std::map<std::string, std::string>>()) {
            record.upload_url_map[std::stoi(slice_id)] = url;  
        }
        // record.upload_url_map = j["data"]["partPresignUploadUrlMap"].get<std::map<std::string, std::string>>();
        record.file_uuid = j["data"]["fileUuid"];
        record.upload_id = j["data"]["uploadId"];
        record.chunk_count = chunk_count;
        record.start_chunk = 0;
        file_status_manager_->AddFileRecord(full_path, record);
        return ErrorCode::SUCCESS;
    }
    return ErrorCode::UNKNOWN_ERROR;
}

void DataUploader::GetUploadBagInfo(shadow::common::FileUploadProgress& upload_progress) {

    std::stringstream ss(upload_progress.fileName);
    std::string item;
    int count = 0;

    while (std::getline(ss, item, '_')) {
        ++count;
        if (count == 5) {
            std::cout << "The fifth element is: " << item << '\n';
            break; // 找到第三个元素后可以跳出循环
        }
    }
    double bag_distance;
    try {   
        bag_distance = std::stod(item);
    }
    catch (const std::invalid_argument& e){   
        std::cout << "Invalid argument: cannot convert string :" << item << "to double"<< '\n';   
        return;   
    }
    std::cout << "upload bag_distance: " << bag_distance << '\n';
    // data_reporter_->addUploadBagInfo(bag_distance, upload_progress.dataSize);
}

//将大文件切割成小分片，通过http put将分片上传到服务器，上传过程中提供重试机制，并在上传完成后通知服务器上传成功
DataUploader::ErrorCode DataUploader::UploadFile(const std::string& full_path) {
    //分割切片
    FileSplitter splitter(full_path, config_.uploadFileSliceSizeMb);
    if (splitter.getErrorCode() != FileSplitter::SUCCESS) {
        LOG_ERROR("Split File Failed.");
        return ErrorCode::FILE_CHUNK_ERROR;
    }

    //获取文件上传信息
    common::FileUploadRecord record;
    // DataUploader::ErrorCode ret;
    auto ret = GetUploadInfo(full_path, splitter.getChunkCount(), record);
    if (ret != ErrorCode::SUCCESS) {
        LOG_ERROR("Get Upload Info Failed.");
        return ret;
    }
    //检查文件是否已上传，如果已上传，则删除文件记录
    if (record.chunk_count+1 == record.start_chunk) {
        file_status_manager_->DeleteFileRecord(full_path);
        LOG_INFO("File %s was uploaded, skip.", full_path.c_str());
        return ErrorCode::SUCCESS;
    }

    common::CompleteUploadReq complete_req;
    complete_req.upload_status = common::UploadStatus::Uploaded;
    common::FileUploadProgress upload_progress;
    upload_progress.fileName = fs::path(full_path).filename().string();
    upload_progress.fileUuid = record.file_uuid;
    upload_progress.dataSize = static_cast<double>(splitter.getFileSize())/1024/1024; 
    upload_progress.uploadStatus = 0;
    for (const auto& [cur_id, url] : record.upload_url_map) {

        LOG_INFO("slice_id: %d", cur_id);   
        LOG_INFO("HttpPut: %s", url.c_str());    
        
        // auto cur_id = std::stoi(slice_id);
        if (record.uploaded_url_map.count(cur_id) > 0) {
            LOG_INFO("slice_id: %d has been uploaded", cur_id);
            continue;
        }
        std::vector<char> buffer;
        // splitter.getChunkData(cur_id, buffer);
        if (splitter.getChunkData(cur_id, buffer) != FileSplitter::ErrorCode::SUCCESS) {
            LOG_ERROR("Get chunk data failed.");
            return ErrorCode::FILE_CHUNK_ERROR;
        }
        for (int i = 0; i < config_.retryCount; ++i) {
            if (i > 0) {
                std::this_thread::sleep_for(std::chrono::seconds(config_.retryIntervalSec));
                LOG_INFO("Retry to upload chunk: %d", cur_id);
            }

            std::string resp;

            LOG_INFO("buffer size: %d", buffer.size());   
            auto ret = curl_wrapper_.HttpPut(
                url, buffer, resp, {"Content-Type:"});
            //模拟HTTP PUT请求，返回CURLE_OK表示成功。
            // auto ret = CURLE_OK;
            // resp = "aaa";
            
            LOG_INFO("HttpPut resp tag: %s", resp.c_str());
            if (resp.length() > 0) {
                complete_req.etag_map[std::to_string(cur_id)] = resp;
            }
            LOG_INFO("HttpPut ret: %d", ret);   
            std::cout << "ret == CURLE_OK:" << (ret == CURLE_OK) << (resp.length() > 0) << std::endl;    
            if (ret == CURLE_OK && resp.length() > 0) {
                std::cout << "upload succeeded" << std::endl;  
                LOG_INFO("Chunk upload succeeded: %d", cur_id);
                upload_progress.uploadUrl = url;
                upload_progress.progress = static_cast<float>(cur_id)/static_cast<float>(record.chunk_count);
                std::this_thread::sleep_for(std::chrono::milliseconds(config_.uploadFileSliceIntervalMs));
                if(cur_id < record.chunk_count){
                    // data_reporter_->publishUploadMessage(upload_progress);
                }
                break;
            }
            complete_req.upload_status = common::UploadStatus::Failed;
            // break;
        }
        if (complete_req.upload_status == common::UploadStatus::Failed) {
            LOG_ERROR("Chunk upload failed: %d", cur_id);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(config_.uploadFileSliceIntervalMs));
    }

    complete_req.type = common::UploadType::ActivelyReport;
    complete_req.file_uuid = record.file_uuid;
    complete_req.upload_id = record.upload_id;
    complete_req.task_id = "";
    complete_req.vin = common::Vin();
    std::string complete_resp;
    ret = CompleteUpload(complete_req, complete_resp);
    // 模拟完成上传的响应。
    // ret = ErrorCode::SUCCESS;

    json j_sim = json{
        {"statusCode","0"}};
    complete_resp = j_sim.dump(); 
           
    LOG_INFO("CompleteUpload ret: %d", ret); 
    if (ret != ErrorCode::SUCCESS || complete_req.upload_status != common::UploadStatus::Uploaded) {
        // file_status_manager_->AddFileRecord(full_path, record);
        LOG_ERROR("Complete url request failed.");  
        upload_progress.uploadStatus = 4;
        // data_reporter_->publishUploadMessage(upload_progress);
        return ErrorCode::INVALID_RESPONSE;
    }

    json j = common::ParseJsonFromString(complete_resp);
    if (j["statusCode"] != "0" ) {
        // file_status_manager_->AddFileRecord(full_path, record);
        LOG_ERROR("Complete upload failed.");  
        upload_progress.uploadStatus = 4;
        // data_reporter_->publishUploadMessage(upload_progress);
        return ErrorCode::UPLOAD_INCOMPLETE;
    }

    upload_progress.uploadStatus = 3;
    // data_reporter_->publishUploadMessage(upload_progress);
    GetUploadBagInfo(upload_progress);

    file_status_manager_->DeleteFileRecord(full_path);
    LOG_INFO( "%s was uploaded successfully.", full_path.c_str());  
    return ErrorCode::SUCCESS;
}

//通过http post请求获取一个url地址
DataUploader::ErrorCode DataUploader::GetUploadUrl(const common::UploadUrlReq& req, std::string& resp) {
    json j = json{
        {"type", static_cast<int>(req.type)},
        {"partNumber", req.part_number},
        {"fileName", req.filename},
        {"vin", req.vin},
        // {"taskId", req.task_id},
        // {"expireMinutes", req.expire_minutes},
    };
    std::string url = "https://" + config_.gateway + "/msinfofeedback/common/file/uploadurl";

    std::cout << url << std::endl;
    std::cout << j << std::endl;

    auto ret = curl_wrapper_.HttpPost(
        url, j.dump(), resp, {"Content-Type: application/json", "Accept: application/json"});
    
    std::cout << resp << std::endl;
    return CurlErrorMapping(ret);
}

DataUploader::ErrorCode DataUploader::UploadFeedback(const common::FeedBackReq& req, std::string& resp) {
    json j = json{
        {"textContent", req.text_content},
        {"softVersion", req.soft_version},
        {"vin", req.vin},
        {"pdsn", req.pdsn},
        {"logUrl", req.log_url},
        {"fileSize", req.file_size},
        {"tboxPdsn", req.tbox_pdsn},
        {"startTime", req.start_time},
        {"endTime", req.end_time},
    };
    std::string url = "https://" + config_.gateway + "/feedbackinfo/v1/common/feedback";
    auto ret = curl_wrapper_.HttpPost(
        url, j.dump(), resp, {"Authorization: " + req.authorization, "Content-Type: application/json", "Accept: application/json"});
    return CurlErrorMapping(ret);
}

//通过get请求查询与特定vin相关的任务
DataUploader::ErrorCode DataUploader::GetQueryTask(const std::string& vin, std::string& resp) {
    std::string url = "https://" + config_.gateway + "/feedback/driving/queryTask?vin=" + vin;
    auto ret = curl_wrapper_.HttpGet(url, resp, {"Accept: application/json"});
    return CurlErrorMapping(ret);
}

//通过post请求向服务器发送一个上传完成的通知
DataUploader::ErrorCode DataUploader::CompleteUpload(const common::CompleteUploadReq& req, std::string& resp) {
    std::string url = "https://" + config_.gateway + "/msinfofeedback/common/file/completeupload";

    json j = json{
        {"vin", req.vin},
        {"type", req.type},
        {"fileUuid", req.file_uuid},
        {"uploadStatus", static_cast<int>(req.upload_status)},
        {"uploadId", req.upload_id},
        // {"taskId", req.task_id},
        {"etagMap", json::object()},
    };
    for (const auto& [slice_id, etag] : req.etag_map) {
        j["etagMap"][slice_id] = etag;
    }
    
    LOG_INFO("completeupload url: %s", url.c_str());
    std::cout << j << std::endl; 

    auto ret = curl_wrapper_.HttpPost(
        url, j.dump(), resp, {"Content-Type: application/json", "Accept: application/json"});
    
    std::cout << resp << std::endl; 
    return CurlErrorMapping(ret);
}

//发送一个httppost请求，向指定服务器查询文件上传状态，http响应将被写在resp中
DataUploader::ErrorCode DataUploader::GetUploadStatus(const std::string& file_uuid, std::string& resp) {
    std::string url = "https://" + config_.gateway + "/msinfofeedback/common/file/uploadstatus";
    json j = json{{"fileUuid", file_uuid}};
    
    std::cout << url << std::endl;   
    std::cout << j <<std::endl;   
    
    auto ret = curl_wrapper_.HttpPost(
        url, j.dump(), resp, {"Content-Type: application/json", "Accept: application/json"});
    
    std::cout << resp <<std::endl; 
    return CurlErrorMapping(ret);
}

//将libcurl返回的错误代码映射为程序自定义的错误类型
DataUploader::ErrorCode DataUploader::CurlErrorMapping(CURLcode code) {
    switch (code) {
        case CURLE_OK:
            return DataUploader::ErrorCode::SUCCESS;
        case CURLE_URL_MALFORMAT:
            return DataUploader::ErrorCode::URL_ERROR;
        case CURLE_COULDNT_CONNECT:
            return DataUploader::ErrorCode::CONNECT_ERROR;
        case CURLE_OPERATION_TIMEDOUT:
            return DataUploader::ErrorCode::TIMEOUT;
        default:
            return DataUploader::ErrorCode::UNKNOWN_ERROR;
    }
}

//加载指定目录中的文件列表，并将符合条件的文件路径加入到upload_queue中
void DataUploader::LoadFileList() {
    std::lock_guard<std::mutex> lock(mutex_);
    // std::string folder_name;
    // encryptor_->GetFolderName(folder_name); 
    // LOG_INFO("upload_folder_name_today: %s", folder_name.c_str());

    for (const auto& [_, upload_dir] : encryptor_->encrypt_paths) {
        std::string upload_dir_today = upload_dir;   
        // std::string upload_dir_today = upload_dir + "/"+ folder_name; 
        LOG_INFO("upload_dir_today: %s", upload_dir_today.c_str());

        if (!common::IsDirExist(upload_dir_today)) {
            LOG_ERROR("Directory %s does not exist.", upload_dir_today.c_str());   
            continue;
        } 
        std::cout << "after Directory exist judge: " << std::endl;
        for (const auto& entry : fs::directory_iterator(upload_dir_today)) {
            // std::cout << "Path: " << entry.path().string() << std::endl;   
            if (entry.is_regular_file() && common::IsMatch(entry.path().filename().string(), config_.filenameRegex)) {
                upload_queue_.push(entry.path().string());
                std::cout << "Path push: " << entry.path().string() << std::endl;   
            }

            upload_queue_.push(entry.path().string());
            std::cout << "Path push: " << entry.path().string() << std::endl;   
            // LOG_ERROR("LoadFileList, file path error: %s", entry.path().string().c_str());
        }
    }
    LOG_INFO("Loaded %d files from upload paths.", upload_queue_.size());  
}

void DataUploader::Run() {
    LOG_INFO("Run.");  
    while (!stop_flag_) {
        LoadFileList();
        ProcessQueue();
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait_for(lock, std::chrono::seconds(1));
    }
}

bool DataUploader::Stop() {
    LOG_INFO("Stop.");  
    encryptor_->Stop();

    stop_flag_ = true;
    cv_.notify_all();
    return true;
}

//从队列中抽出文件进行上传，成功则删除文件，失败则重试上传
void DataUploader::ProcessQueue() {
    const auto& debug_config = config::AppConfig::getInstance().GetConfig().debug;
    while (!stop_flag_) {
        std::string current_file;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (upload_queue_.empty()) {
                LOG_INFO("No files in queue.");  
                break;
            }
            current_file = upload_queue_.front();
            LOG_INFO("begin upload file %s.", current_file.c_str());  
            upload_queue_.pop();
        }

        std::filesystem::path current_file_path(current_file);   
        std::string encrypted_file = encryptor_->enc_dir_ + "/" + current_file_path.filename().string() + ".enc";

        LOG_INFO("Encrypting file: %s", encrypted_file.c_str());    
        if (!std::filesystem::exists(encrypted_file)) {
            std::string decrypted_file = encryptor_->enc_dir_ + "/" + current_file_path.filename().string() + ".dec";
            if (!debug_config.closeDataEnc)
            {
                auto success_enc = encryptor_->EncryptChunkFileWithEnvelope(current_file, encrypted_file);
                // auto success_enc = encryptor_->EncryptFileWithEnvelope(current_file, encrypted_file);
                // encrypted_file = "/home/nvidia/userdata/data_collection/readme.lz4.enc";
                // decrypted_file = "/home/nvidia/userdata/data_collection/readme.zip";
                // auto success_dec = encryptor_->DecryptFileWithEnvelope(encrypted_file, decrypted_file);
                LOG_INFO("encryptor success: %d", success_enc);
                if (success_enc == 0) {
                    LOG_INFO("encryptor file: %s sucessfully.", current_file.c_str());
                    // common::DeleteFile(current_file);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                } else {
                    LOG_ERROR("Failed to encrypt file: %s", current_file.c_str());
                    continue;
                }
            }
        } else {   
            LOG_INFO("file: %s already encrypted.", current_file.c_str());   
        }
       
        auto success_upload = UploadFile(encrypted_file);
        
        LOG_INFO("upload success: %d", success_upload);   

        if (success_upload == ErrorCode::SUCCESS) {
            LOG_INFO("Uploaded file: %s", current_file.c_str());   
            common::DeleteFile(current_file);
            common::DeleteFile(encrypted_file);
            std::this_thread::sleep_for(std::chrono::milliseconds(config_.uploadFileIntervalMs));
        } else {
            std::lock_guard<std::mutex> lock(mutex_);
            LOG_ERROR("Failed to upload file: %s", current_file.c_str());   
            // upload_queue_.push(current_file);
            std::this_thread::sleep_for(std::chrono::seconds(config_.retryIntervalSec));
        }
    }
}

}
}
