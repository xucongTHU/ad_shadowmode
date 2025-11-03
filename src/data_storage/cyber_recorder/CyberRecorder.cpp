//
// Created by xucong on 25-5-30.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#include "CyberRecorder.h"
#include "cyber/record/header_builder.h"
#include "cyber/record/record_writer.h"
#include "cyber/service_discovery/topology_manager.h"
// #include "cyber/message/raw_message.h"
#include <trigger/core/rule/ACCQuitTrigger.h>

#include "cyber/record/record_message.h"
#include "cyber/common/log.h"
#include "common/logger/Logger.h"

namespace shadow {
namespace data_storage {

static const char* LOG_TAG = "DataStorage";

CyberRecorder::CyberRecorder(stoic::cm::NodeHandle& nh, const std::shared_ptr<strategy::Strategy>& s)
    : node_(&nh), strategy_(s) {
    if (node_ == nullptr) {
        std::cerr << "create node failed, node: << data_storage! " << std::endl;
    }
    header_ = apollo::cyber::record::HeaderBuilder::GetHeader();

    // message_count_ = 0;
    message_time_ = 0;
    message_subject_ = std::make_unique<Subject<PString>>();
}

bool CyberRecorder::Init(strategy::CacheMode cache_mode) {
    is_started_ = true;
    message_subject_->attach(shared_from_this());

    channelDataBuffer_.trigger_forward_duration = cache_mode.forwardCaptureDurationSec;
    channelDataBuffer_.trigger_backword_duration = cache_mode.backwardCaptureDurationSec;
    for(auto &item: strategy_->cyber.channels) {
        // LOG_INFO("init data from topic :%s", item.channel_name.c_str());
        if(SensorBufferInit(item, channelDataBuffer_)) {
            LOG_INFO("whiteChannels init data from topic :%s", item.topic.c_str());
            whiteChannels.emplace_back(item.topic);
        }
    }

    if (!InitTopicDataCache()) {
        LOG_ERROR("init_readers error.");
        return false;
    }
    return true;
}

bool CyberRecorder::InitTopicDataCache() {
    channel_manager = TopologyManager::Instance()->channel_manager();
    // get historical writers
    std::vector<RoleAttributes> role_attr_vec;
    channel_manager->GetWriters(&role_attr_vec);
    for (auto role_attr : role_attr_vec) {
        FindNewChannel(role_attr);
    }

    // listen new writers in future
    change_conn_ = channel_manager->AddChangeListener(
        std::bind(&CyberRecorder::TopologyCallback, this, std::placeholders::_1));
    if (!change_conn_.IsConnected()) {
        LOG_ERROR("change connection is not connected");
        return false;
    }
    return true;
}

void CyberRecorder::WriteSingleChannelData(boost::circular_buffer<shadow_sensor_buffer>& sensor_buffer) {
    int frame_count = sensor_buffer.size();
    // std::cout << "frame count: " << frame_count  << std::endl ;
    while(frame_count)
    {
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            psensor_buffer_w = sensor_buffer.front();
            sensor_buffer.pop_front();
        }

        if (!writer_->WriteMessage(psensor_buffer_w.channel_name,
                                   psensor_buffer_w.pbufffer, psensor_buffer_w.timestamp)) {
            LOG_ERROR("write data fail, channel: %s", psensor_buffer_w.channel_name.c_str());
            break;
        }
        if((sensor_buffer.size() < 500) && (frame_count % 10 == 0)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            // std::cout << "frame count write: " << frame_count  << std::endl ;
            //  std::cout << "sleep_for(std::chrono::milliseconds(10))! " << std::endl ;
        }

        frame_count--;
    }
}

bool CyberRecorder::WriteChannelData(uint64_t trigger_time){
    if (!writer_) {
        LOG_ERROR("Writer is not initialized, cannot write channel info");
        return false;
    }
    uint64_t time_to_wait = trigger_time + channelDataBuffer_.trigger_backword_duration*1e6 - apollo::cyber::Time::NowUs();
    std::cout << "time now :" << apollo::cyber::Time::NowUs() << std::endl;
    std::cout << "time to wait for trigger :" << time_to_wait /1e6  << std::endl;
    if(time_to_wait > 0) {
        std::cerr << "Trigger time waiting, waiting for :"<< time_to_wait /1e6 << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(time_to_wait));
    }

    LOG_INFO("time now after: %d", apollo::cyber::Time::NowUs());
    is_writingdata_ = true;

    for (const auto& item: strategy_->cyber.channels) {
        LOG_INFO("write channel data for: %s", item.type.c_str());
        switch (sensor_type_map[item.type]) {
        case SENSOR_TYPE_FW_CAMERA:
            WriteSingleChannelData(channelDataBuffer_.fw_camera_cb);
            break;
        case SENSOR_TYPE_FR_CAMERA:
            WriteSingleChannelData(channelDataBuffer_.fr_camera_cb);
            break;
        case SENSOR_TYPE_FL_CAMERA:
            WriteSingleChannelData(channelDataBuffer_.fl_camera_cb);
            break;
        case SENSOR_TYPE_RR_CAMERA:
            WriteSingleChannelData(channelDataBuffer_.rr_camera_cb);
            break;
        case SENSOR_TYPE_RL_CAMERA:
            WriteSingleChannelData(channelDataBuffer_.rl_camera_cb);
            break;
        case SENSOR_TYPE_R_CAMERA:
            WriteSingleChannelData(channelDataBuffer_.r_camera_cb);
            break;
        case SENSOR_TYPE_CHASSIS:
            WriteSingleChannelData(channelDataBuffer_.chassis_cb);
            break;
        case SENSOR_TYPE_INS:
            WriteSingleChannelData(channelDataBuffer_.ins_cb);
            break;
        default:
            std::cout << "This sensor type doesn't support" << std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    writer_->Close();
    LOG_INFO("writer_->Close()");
    is_writingdata_ = false;
    return true;
}

bool CyberRecorder::WriteChannelInfo(){
    if (!writer_) {
        std::cerr << "Writer is not initialized, cannot write channel info" << std::endl;
        return false;
    }
    channel_manager->GetWriters(&role_attr_vec);
    LOG_INFO("role_attr_vec size: %d", role_attr_vec.size());

    for (auto role_attr : role_attr_vec) {
        LOG_INFO("WriteProgress role_attr.channel_name: %s", role_attr.channel_name().c_str());
        if (channel_subscribers_.find(role_attr.channel_name()) != channel_subscribers_.end()) {
            std::cout << "WriteProgress role_attr.channel_name in channel infos: " << role_attr.channel_name() << std::endl;
            if (!writer_->WriteChannel(role_attr.channel_name(),
                            role_attr.message_type(),
                            role_attr.proto_desc())) {
                LOG_ERROR("write channel fail, channel: %s", role_attr.channel_name().c_str());
            }
        }
    }
    role_attr_vec.clear();
    return true;
}

bool CyberRecorder::Open(OptMode opt_mode, const std::string& full_path) {
    // has_data_writen = false;
    // if (IsOpened() || !node_) {
    //     std::cerr << "Recorder already opened or node is null" << std::endl;
    //     return false;
    // }

    if (opt_mode == OptMode::OPT_READ) {
        reader_ = std::make_unique<apollo::cyber::record::RecordReader>(full_path);
        if (!reader_->IsValid()) {
            std::cerr << "Failed to open record file for reading: " << full_path << std::endl;
            reader_.reset(nullptr);
            return false;
        }
    } else if (opt_mode == OptMode::OPT_WRITE) {
        writer_ = std::make_unique<apollo::cyber::record::RecordWriter>(header_);
        if (!writer_->Open(full_path)) {
            std::cerr << "Failed to open record file for writing: " << full_path << std::endl;
            writer_.reset();
            return false;
        }
    } else {
        std::cerr << "Unsupported operation mode" << std::endl;
        return false;
    }
    // is_started_ = true;
    full_path_ = full_path;
    return true;
}

bool CyberRecorder::IsOpened() const {
    if (reader_ != nullptr) return reader_->IsValid();
    if (writer_ != nullptr) return true;
    return false;
}

bool CyberRecorder::Write(const std::string& topic_name, uint64_t timestamp, const void* buf, size_t buf_len) {
    if (!writer_) {
        std::cerr << "Writer is not initialized, cannot write message to topic: " << topic_name << std::endl;
        return false;
    }

    if (!channel_subscribers_.count(topic_name)) {
        auto channel_manager = apollo::cyber::service_discovery::TopologyManager::Instance()->channel_manager();
        std::vector<apollo::cyber::proto::RoleAttributes> role_attr_vec;
        channel_manager->GetWriters(&role_attr_vec);
        //
        for (const auto& role_attr : role_attr_vec) {
            FindNewChannel(role_attr);
        }

        // listen new writers in future
        change_conn_ = channel_manager->AddChangeListener(
            std::bind(&CyberRecorder::TopologyCallback, this, std::placeholders::_1));
        if (!change_conn_.IsConnected()) {
            std::cerr << "change connection is not connected";
            return false;
        }
        return true;
    }

}

void CyberRecorder::TopologyCallback(const apollo::cyber::proto::ChangeMsg& msg) {
    // LOG_INFO("ChangeMsg in Topology Callback: %d -> %s", msg.role_type(), msg.role_attr().channel_name().c_str());
    // std::cerr << "ChangeMsg in Topology Callback:" << msg.role_type() << " -> " <<  msg.role_attr().channel_name() << std::endl;
            //   << msg.ShortDebugString() << std::endl;
    if (msg.role_type() != apollo::cyber::proto::ROLE_WRITER) {
        LOG_ERROR("Change message role type is not ROLE_WRITER.");
        return;
    }
    FindNewChannel(msg.role_attr());
}

void CyberRecorder::FindNewChannel(const apollo::cyber::proto::RoleAttributes& role_attr) {
    if (!role_attr.has_channel_name() || role_attr.channel_name().empty()) {
        std::cerr << "change message not has a channel name or has an empty one.";
        return ;
    }
    if (!role_attr.has_message_type() || role_attr.message_type().empty()) {
        std::cerr << "Change message not has a message type or has an empty one.";
        return;
    }
    if (!role_attr.has_proto_desc() || role_attr.proto_desc().empty()) {
        std::cerr << "Change message not has a proto desc or has an empty one.";
        return;
    }
    if (std::find(whiteChannels.begin(), whiteChannels.end(),
                  role_attr.channel_name()) == whiteChannels.end()) {
        // LOG_ERROR("role_attr.channel_name(): %s", role_attr.channel_name().c_str());
        // LOG_ERROR("New channel was found, but not in record list.");
        return;
    }
    if (channel_subscribers_.find(role_attr.channel_name()) == channel_subscribers_.end()) {
        // if (!writer_->WriteChannel(role_attr.channel_name(),
        //                            role_attr.message_type(),
        //                            role_attr.proto_desc())) {
        //     std::cerr << "write channel fail, channel:" << role_attr.channel_name();
        //                            }
        InitReaderImpl(role_attr.channel_name(), role_attr.message_type());
    }
}

bool CyberRecorder::InitReaderImpl(const std::string& channel_name,
                                   const std::string& /*message_type*/) {
    try {
        apollo::cyber::ReaderConfig config;
        config.channel_name = channel_name;
        config.pending_queue_size =
            gflags::Int32FromEnv("CYBER_PENDING_QUEUE_SIZE", 50);

        std::weak_ptr<CyberRecorder> weak_this = shared_from_this();
        auto sub_ = std::make_unique<stoic::cm::Subscriber>(
            node_->subscribe<PString, false>(
            channel_name, config.pending_queue_size,
            [weak_this, channel_name](const std::shared_ptr<PString>& raw_message) {
                if (auto share_this = weak_this.lock()) {
                    share_this->ReaderCallback(raw_message, channel_name);
                }
            })
        );

        if (!sub_) {
            std::cerr << "Create subscriber failed for channel: " << channel_name;
            return false;
        }

        channel_subscribers_[channel_name] = std::move(sub_);

        return true;
    } catch (const std::bad_weak_ptr& e) {
        std::cerr << "error:" << e.what();
        return false;
    }
}

void CyberRecorder::ReaderCallback(const std::shared_ptr<PString>& message,
                                   const std::string& channel_name) {
    if (!is_started_ || is_stopping_) {
        std::cerr << "record procedure is not started or stopping.";
        return;
    }

    if (message == nullptr) {
        std::cerr << "message is nullptr, channel: " << channel_name;
        return;
    }
    message_subject_->notifyAll(*message, channel_name);
}

void CyberRecorder::callback(const PString& message, const std::string& topic) {
    if (!is_writingdata_){
        message_time_ = apollo::cyber::Time::Now().ToNanosecond();
        shadow_sensor_buffer psensor_buffer;
        psensor_buffer.timestamp = message_time_;
        psensor_buffer.pbuffer = message;
        psensor_buffer.channel_name= topic.data();
        // std::cout << "storage channel: "<<psensor_buffer.channel_name << " size:"<<psensor_buffer.pbufffer.ByteSize()<< std::endl;

        std::lock_guard<std::mutex> lock(dataMutex);
        for(const auto&  item: strategy_->cyber.channels) {
            if(item.topic == topic) {
                switch (sensor_type_map[item.type]){
                case SENSOR_TYPE_FW_CAMERA:
                    channelDataBuffer_.fw_camera_cb.push_back(psensor_buffer);
                    break;
                case SENSOR_TYPE_FR_CAMERA:
                    channelDataBuffer_.fr_camera_cb.push_back(psensor_buffer);
                    break;
                case SENSOR_TYPE_FL_CAMERA:
                    channelDataBuffer_.fl_camera_cb.push_back(psensor_buffer);
                    break;
                case SENSOR_TYPE_RR_CAMERA:
                    channelDataBuffer_.rr_camera_cb.push_back(psensor_buffer);
                    break;
                case SENSOR_TYPE_RL_CAMERA:
                    channelDataBuffer_.rl_camera_cb.push_back(psensor_buffer);
                    break;
                case SENSOR_TYPE_R_CAMERA:
                    channelDataBuffer_.r_camera_cb.push_back(psensor_buffer);
                    break;
                case SENSOR_TYPE_CHASSIS:
                    channelDataBuffer_.chassis_cb.push_back(psensor_buffer);
                    break;
                case SENSOR_TYPE_INS:
                    channelDataBuffer_.ins_cb.push_back(psensor_buffer);
                    break;
                default:
                    std::cout << "This sensor type doesn't support" << std::endl;
                    break;
                }
            }
        }
    }
   
    // message_count_++;
}

CyberBagInfo CyberRecorder::GetBagInfo() {
    if (reader_ == nullptr || !reader_->IsValid()) {
        std::cerr << "Reader is not valid, cannot get bag info";
        return {};
    }

    CyberBagInfo info;
    info.bag_path = full_path_;

    auto header = reader_->GetHeader();
    info.start_time_ns = header.begin_time();
    info.end_time_ns = header.end_time();

    auto channels = reader_->GetChannelList();
    for (const auto& channel : channels) {
        CyberChannelInfo channel_info;
        channel_info.channel_name = channel;
        channel_info.message_count = reader_->GetMessageNumber(channel);
        channel_info.message_type = reader_->GetMessageType(channel);
        // channel_info.start_time_ns = reader_->GetFirstMessageTime(channel);
        // channel_info.end_time_ns = reader_->GetLastMessageTime(channel);
        info.channel_infos.emplace_back(channel_info);
    }

    return info;
}

bool CyberRecorder::ReadNextFrame(apollo::cyber::message::RawMessage* message) {
    if (reader_ == nullptr || !reader_->IsValid() || message == nullptr) {
        std::cerr << "Invalid reader or message pointer" << std::endl;
        return false;
    }

    apollo::cyber::record::RecordMessage record_msg;
    if (!reader_->ReadMessage(&record_msg)) {
        return false;
    }

    message->message = record_msg.content;
    message->timestamp = record_msg.time;
    return true;
}

bool CyberRecorder::Close() {
    if (!is_started_ || is_stopping_) {
        return false;
    }
    is_stopping_ = true;

    auto channel_manager = apollo::cyber::service_discovery::TopologyManager::Instance()->channel_manager();
    channel_manager->RemoveChangeListener(change_conn_);

    if (reader_ != nullptr) {
        reader_.reset(nullptr);
    }
    if (writer_ != nullptr) {
        writer_->Close();
        writer_.reset();
        channel_subscribers_.clear();
        delete node_;
        // std::cout << "Record file closed. Total messages written: " << message_count_ << std::endl;
    }
    is_started_ = false;
    is_stopping_ = false;

    return reader_ == nullptr && writer_ == nullptr;
}

bool CyberRecorder::SensorBufferInit(strategy::Channel& channel_info, channel_data_buffer& shadow_init_buff) {
      int buffer_count;
      int bag_duration = shadow_init_buff.trigger_forward_duration + shadow_init_buff.trigger_backword_duration;
      switch (sensor_type_map[channel_info.type]) {
          case SENSOR_TYPE_FW_CAMERA:
            if (channel_info.capturedFrameRate > DEFAULT_CAMERA_FRAMERATE || channel_info.capturedFrameRate <= 0)
            {
              channel_info.capturedFrameRate = DEFAULT_CAMERA_FRAMERATE;
            }
            buffer_count = bag_duration * channel_info.capturedFrameRate;
            shadow_init_buff.fw_camera_cb.set_capacity(buffer_count);
            break;
          case SENSOR_TYPE_FR_CAMERA:
            if (channel_info.capturedFrameRate > DEFAULT_CAMERA_FRAMERATE || channel_info.capturedFrameRate <= 0)
            {
              channel_info.capturedFrameRate = DEFAULT_CAMERA_FRAMERATE;
            }
            buffer_count = bag_duration * channel_info.capturedFrameRate;
            shadow_init_buff.fr_camera_cb.set_capacity(buffer_count);
            break;
          case SENSOR_TYPE_FL_CAMERA:
            if (channel_info.capturedFrameRate > DEFAULT_CAMERA_FRAMERATE || channel_info.capturedFrameRate <= 0)
            {
              channel_info.capturedFrameRate = DEFAULT_CAMERA_FRAMERATE;
            }
            buffer_count = bag_duration * channel_info.capturedFrameRate;
            shadow_init_buff.fl_camera_cb.set_capacity(buffer_count);
            break;
          case SENSOR_TYPE_RR_CAMERA:
            if (channel_info.capturedFrameRate > DEFAULT_CAMERA_FRAMERATE || channel_info.capturedFrameRate <= 0)
            {
              channel_info.capturedFrameRate = DEFAULT_CAMERA_FRAMERATE;
            }
            buffer_count = bag_duration * channel_info.capturedFrameRate;
            shadow_init_buff.rr_camera_cb.set_capacity(buffer_count);
            break;
          case SENSOR_TYPE_RL_CAMERA:
            if (channel_info.capturedFrameRate > DEFAULT_CAMERA_FRAMERATE || channel_info.capturedFrameRate <= 0)
            {
              channel_info.capturedFrameRate = DEFAULT_CAMERA_FRAMERATE;
            }
            buffer_count = bag_duration * channel_info.capturedFrameRate;
            shadow_init_buff.rl_camera_cb.set_capacity(buffer_count);
            break;
          case SENSOR_TYPE_R_CAMERA:
            if (channel_info.capturedFrameRate > DEFAULT_CAMERA_FRAMERATE || channel_info.capturedFrameRate <= 0)
            {
              channel_info.capturedFrameRate = DEFAULT_CAMERA_FRAMERATE;
            }
            buffer_count = bag_duration * channel_info.capturedFrameRate;
            shadow_init_buff.r_camera_cb.set_capacity(buffer_count);
            break;
          case SENSOR_TYPE_CHASSIS:
            if (channel_info.capturedFrameRate > DEFAULT_CHASSIS_FRAMERATE || channel_info.capturedFrameRate <= 0)
            {
              channel_info.capturedFrameRate = DEFAULT_CHASSIS_FRAMERATE;
            }
            buffer_count = bag_duration * channel_info.capturedFrameRate;
            shadow_init_buff.chassis_cb.set_capacity(buffer_count);
            break;
          case SENSOR_TYPE_INS:
            if (channel_info.capturedFrameRate > DEFAULT_INS_FRAMERATE || channel_info.capturedFrameRate <= 0)
            {
              channel_info.capturedFrameRate = DEFAULT_INS_FRAMERATE;
            }
            buffer_count = bag_duration * channel_info.capturedFrameRate;
            shadow_init_buff.ins_cb.set_capacity(buffer_count);
            break;
          default:
            std::cout << "This sensor type doesn't support" << std::endl;
            return false;
            break;
          }
    return true;
}

} // namespace data_storage
} // namespace shadow