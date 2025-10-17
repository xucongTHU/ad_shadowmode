//
// Created by xucong on 25-5-7.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_DATA_H
#define SHADOW_MODE_DATA_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <cstdint>

namespace shadow {
namespace common {
using json = nlohmann::json;

struct TBussiness {
    std::string bussiness_type;
    std::string data_source; // Cam Radar Log Other
    std::string data_type; // syslog、otherlog、edr、 AutoDrivingData
};

const std::vector<TBussiness> kBussiness{
    {"CNOP", "Other", "AutoDrivingData"},
    {"AVP", "Other", "AutoDrivingData"},
    {"EDR", "Other", "edr"},
    {"ITC", "Log", "syslog"}
};

enum class UploadStatus : uint8_t
{
    Uploaded = 3, //
    Failed = 4,   //
};

enum class UploadType : uint8_t
{
    ActivelyReport = 3,
    InstructionDelivery = 4,
};

// 上传URL请求
struct UploadUrlReq {
    UploadType type;
    int part_number;
    std::string vin;
    std::string filename;
};

// 反馈请求
struct FeedBackReq {
    std::string text_content;
    std::string soft_version;
    std::string vin;
    std::string pdsn;
    std::string log_url;
    int64_t file_size;
    std::string tbox_pdsn;
    int64_t start_time;
    int64_t end_time;
    std::string device;
    std::string deviceId;
    std::string deviceType;
    std::string deviceModel;
    std::vector<int> devicesIdList;
    std::string deviceSerial;
    std::string deviceManufacturer;
    std::vector<int> devicesTypeList;
    std::string deviceManufacturerName;
    std::string deviceModelName;
    std::vector<int> devicesManufacturerList;
    std::string authorization;
};

// 完成上传请求
struct CompleteUploadReq {
    std::string vin;
    UploadType type;
    std::string file_uuid;
    UploadStatus upload_status;
    std::string upload_id;
    std::string task_id;
    std::map<std::string, std::string> etag_map;
};


// 文件上传记录
struct FileUploadRecord {
    int8_t start_chunk;
    std::string file_uuid;
    std::map<int, std::string> upload_url_map;
    std::map<int, std::string> uploaded_url_map;
    std::string upload_id;
    int8_t chunk_count;
};

// 文件上传进度
struct FileUploadProgress {
    std::string vin;
    std::string taskId;
    std::string batchId;   
    std::string uploadUrl;
    std::string fileName;
    std::string fileUuid;
    double dataSize;
    int8_t uploadStatus;
    float progress;
    // 方法将结构体转换为json对象
    json to_json() const {
        return json{
            {"vin", vin},
            {"taskId", taskId},  
            {"batchId", batchId},  
            {"uploadUrl", uploadUrl},
            {"fileName", fileName},
            {"fileUuid", fileUuid},
            {"dataSize", dataSize},
            {"uploadStatus", uploadStatus},
            {"progress" , progress}
        };
    }
};

// 系统状态信息
struct SysInfo {
    std::string taskId;
    float cpuUsage;
    float memUsage;
    float harddriveUsage;
    float ssdUsage;
    int8_t imageFrameRateException;
    int8_t pcFrameRateException;
    int8_t canbusFrameRateException;
    int8_t insFrameRateException;
    // 方法将结构体转换为json对象
    json to_json() const {
        return json{
            {"taskId", taskId},  
            {"cpuUsage", cpuUsage},
            {"memUsage", memUsage},
            {"harddriveUsage", harddriveUsage},
            {"ssdUsage", 0.0},
            {"imageFrameRateException" , imageFrameRateException},   
            {"pcFrameRateException" , pcFrameRateException},   
            {"canbusFrameRateException", canbusFrameRateException},     
            {"insFrameRateException ", insFrameRateException}       
        };
    }
};


// 采集状态信息
struct CollectInfo {
    std::string vin;
    std::string taskId;
    double distanceCollect;
    int32_t durationCollect;
    int32_t triggercountCollect;
    double bagcapacityCollect;
    double distanceUpload;
    int32_t durationUpload;
    int32_t triggercountUpload;
    double bagcapacityUpload; 
    // 方法将结构体转换为json对象
    json to_json() const {
        return json{
            {"vin", vin},
            {"taskId", taskId},
            {"distanceCollect", distanceCollect},
            {"durationCollect", durationCollect},
            {"triggercountCollect", triggercountCollect},
            {"bagcapacityCollect", bagcapacityCollect},
            {"distanceUpload", distanceUpload},
            {"durationUpload", durationUpload},
            {"triggercountUpload", triggercountUpload},
            {"bagcapacityUpload", bagcapacityUpload}
        };
    }
};

// 采集状态信息
struct RtspInfo {
    std::string taskId; 
    int8_t status;
    std::string rtspUrl;
    // 方法将结构体转换为json对象
    json to_json() const {
        return json{
            {"taskId", taskId},
            {"status", status},
            {"rtspUrl", rtspUrl}
        };
    }
};

struct OBUStateInfo {
    std::string vin;
    /**
     * 经度, 东经为正，西经为负, 精度0.0000001
     */
    std::string longitude;
    /**
     * 纬度，北纬为正，南纬为负，精度0.0000001
     */
    std::string latitude;
    /**
     * 航向，正北方为0度，顺时针方向，精度0.01
     */
    std::string heading;
    /**
     * 速度，千米/时，精度0.01
     */
    std::string speed;
    /**
     * 加速度，米/秒2，精度0.01
     */
    std::string accSpeed;
    /**
     * 启动状态，0：ACC，1：ON，2：READY
     */
    std::string gpsTime;
    /**
     * 高度，米，精度0.01
     */
    std::string altitude;
    /**
     * 差分状态，差分状态，A=自动，D=差分，E=估测，N=数据无效
     */
    std::string rtkState;
    /**
     * 转向灯灯态，指示灯状态：if(data & 0x100) 左转灯亮，if(data & 0x1000)右转灯亮，if(data & 0x10)hazardLight亮 （0x00=都不亮，255 = 无效）
     */
    std::string light;
    /**
     * 刹车状态，刹车幅度0~100（255=无效值）
     */
    int32_t brakeState;
    /**
     * 油门状态，油门幅度0~100（255=无效值）
     */
    int32_t gasState;
    /**
     * 档位状态，P、D、N、R （A=无效值）
     */
    std::string gearState;
    /**
     * 方向盘转角，方向盘转角 左转为负，右转为正，-500~500 (10000=无效值)
     */
    std::string wheelAngle;
    /**
     * 驾驶状态，驾驶状态：0x0 SVB control activated; 0x1 Losing SVB mode; 0x2 Remote control activated; 0x3 Reserved（255=无效）
     */
    std::string driveState;
    /**
     * 异常状态，车辆故障码
     */
    std::string abnormalState;
    /**
     * 交通信号灯，0：灭，1：绿，2：绿闪，3：红，4：黄，5：黄闪，6：红闪
     */
    int32_t traffic_light;
    std::string mileage;
    std::string engineSpeed;
    /**
     * 充电状态，0：未充，1：充电
     */
    int8_t chargeState;
    /**
     * 手刹状态，0：放下，1：拉起
     */
    int8_t handbrake;
    /**
     * 启动状态，0：ACC，1：ON，2：READY
     */
    int8_t startState;
    std::string elecQuantity;
    std::string endurance;
    int8_t starNm;
     /**
     * 是否正在采集，0：未采集，1：采集中
     */
    int8_t collectState;
    json to_json() const {
        return json{
            {"vin", vin},
            {"longitude", longitude},
            {"latitude", latitude},
            {"heading", 0.0},
            {"speed", speed},
            {"accspeed", 0.0},
            {"gpstime", gpsTime},
            {"altitude", altitude},
            {"starNm", 0},
            {"rtkState", "A"},
            {"light", light},
            {"brakeState", brakeState},   
            {"gasState", 0},
            {"gearState", gearState},    
            {"wheelAngle", wheelAngle}, 
            {"driveState","0"}, 
            {"abnormalState","0"}, 
            {"traffic_light",0}, 
            {"mileage","0"}, 
            {"engineSpeed","0"}, 
            {"chargeState",0}, 
            {"handbrake",0}, 
            {"startState",0}, 
            {"elecQuantity","0"}, 
            {"endurance",0},
            {"collectState", collectState}
        };
    }
};


struct CPUData {
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
};

struct FrameCounter {
    int32_t frame_counter_canbus = 0; 
    int32_t frame_counter_inspva = 0; 
    int32_t frame_counter_img = 0; 
    int32_t frame_counter_pc = 0; 
};


} // namespace common
} // namespace shadow

#endif //SHADOW_MODE_DATA_H
