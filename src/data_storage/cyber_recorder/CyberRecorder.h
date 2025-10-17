#ifndef SHADOW_MODE_CYBER_RECORDER_H
#define SHADOW_MODE_CYBER_RECORDER_H


#include <memory>
#include <cm/cm.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "cyber/cyber.h"
#include "cyber/node/node.h"
#include "cyber/record/record_reader.h"
#include "cyber/record/record_writer.h"
#include "cyber/common/time_conversion.h"
#include "proto/common/header.pb.h"
// #include "cyber/message/raw_message.h"
#include "data_storage/common/StorageData.h"
#include "config/AppConfig.h" 
#include "strategy/StrategyConfig.h"
#include "pattern/Observer.h"
// #include "pattern/DataRouter.h"
#include "proto/common/string.pb.h"

namespace shadow {
namespace data_storage {

using apollo::cyber::Node;
using apollo::cyber::ReaderBase;
using apollo::cyber::base::Connection;
// using apollo::cyber::message::RawMessage;
//ADD
using stoic::cm::proto::common::PString;
using apollo::cyber::proto::ChangeMsg;
using apollo::cyber::proto::RoleAttributes;
using apollo::cyber::proto::RoleType;
using apollo::cyber::service_discovery::ChannelManager;
using apollo::cyber::service_discovery::TopologyManager;

struct CyberChannelInfo {
    std::string channel_name;
    std::string message_type;
    uint64_t message_count = 0;
    uint64_t start_time_ns = 0;
    uint64_t end_time_ns = 0;
};

struct CyberBagInfo {
    std::string bag_path;
    uint64_t start_time_ns = 0;
    uint64_t end_time_ns = 0;
    std::vector<CyberChannelInfo> channel_infos;
};

enum class OptMode {
    OPT_READ,
    OPT_WRITE
};

class CyberRecorder : public std::enable_shared_from_this<CyberRecorder>,
                      public Observer<PString> {
public:
    CyberRecorder(stoic::cm::NodeHandle& nh,
                  const std::shared_ptr<strategy::Strategy>& s);
    ~CyberRecorder() = default;

    // 禁止拷贝构造和赋值
    CyberRecorder(const CyberRecorder&) = delete;
    CyberRecorder& operator=(const CyberRecorder&) = delete;
    

    bool Init(strategy::CacheMode cache_mode);
    bool Open(OptMode opt_mode, const std::string& full_path);
    bool WriteChannelInfo();
    bool WriteChannelData(uint64_t trigger_time);
    bool IsOpened() const;
    bool Write(const std::string& topic_name, uint64_t timestamp, const void* buf, size_t buf_len);
    CyberBagInfo GetBagInfo();
    bool ReadNextFrame(apollo::cyber::message::RawMessage* message);
    bool Close();
    bool HasDataWritten() const { return has_data_writen; }

private:
    bool SensorBufferInit(strategy::Channel& channel_info, channel_data_buffer &shadow_init_buff);
    bool InitTopicDataCache();
    void TopologyCallback(const apollo::cyber::proto::ChangeMsg& msg);
    void FindNewChannel(const apollo::cyber::proto::RoleAttributes& role_attr);
    bool InitReaderImpl(const std::string& channel_name,
                        const std::string& message_type);
    void ReaderCallback(const std::shared_ptr<PString>& message,
                        const std::string& channel_name);
    void WriteSingleChannelData(boost::circular_buffer<shadow_sensor_buffer>& sensor_buffer);
    void callback(const PString& message, const std::string& topic) override;

    std::unordered_map<std::string, int> sensor_type_map = {
        {"FRONT_WIDE_IMAGE", SENSOR_TYPE_FW_CAMERA},
        {"FRONT_LEFT_IMAGE", SENSOR_TYPE_FL_CAMERA},
        {"FRONT_RIGHT_IMAGE", SENSOR_TYPE_FR_CAMERA},
        {"REAR_IMAGE", SENSOR_TYPE_R_CAMERA},
        {"REAR_LEFT_IMAGE", SENSOR_TYPE_RL_CAMERA},
        {"REAR_RIGHT_IMAGE", SENSOR_TYPE_RR_CAMERA},
        {"Chassis", SENSOR_TYPE_CHASSIS},
        {"Inspavx", SENSOR_TYPE_INS}
    };


private:
    stoic::cm::NodeHandle* node_ = nullptr;
    std::unique_ptr<apollo::cyber::record::RecordReader> reader_ = nullptr;
    std::unique_ptr<apollo::cyber::record::RecordWriter> writer_ = nullptr;
    apollo::cyber::proto::Header header_;
    std::string full_path_;
    bool has_data_writen = false;
    uint64_t message_count_ = 0;
    uint64_t message_time_;
    bool is_started_ = false;
    bool is_stopping_ = false;
    bool is_writingdata_ = false;
    // std::vector<std::string> channel_vec_;
    // std::unordered_map<std::string, std::shared_ptr<apollo::cyber::ReaderBase>> channel_infos_;
    apollo::cyber::base::Connection<const apollo::cyber::proto::ChangeMsg&> change_conn_;
    // config::AppConfigData::DataStorage config_;
    // strategy::StrategyConfig strategy_config_;
    std::shared_ptr<strategy::Strategy> strategy_{nullptr};
    // strategy::Cyber cyber_;
    channel_data_buffer channelDataBuffer_;
    std::vector<std::string> whiteChannels;
    std::shared_ptr<ChannelManager> channel_manager;
    std::vector<RoleAttributes> role_attr_vec;
    shadow_sensor_buffer psensor_buffer_w;
    std::mutex dataMutex;

    // ADD
    std::unordered_map<std::string, std::unique_ptr<stoic::cm::Subscriber>> channel_subscribers_;
    std::unique_ptr<Subject<PString>> message_subject_;
};

} // namespace data_storage
} // namespace shadow

#endif // SHADOW_MODE_CYBER_RECORDER_H