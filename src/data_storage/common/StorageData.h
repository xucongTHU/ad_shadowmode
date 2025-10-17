#pragma once

#include <memory>
#include <mutex>
#include <string>

// #include <boost/circular_buffer.hpp>
// #include "cyber/record/record_writer.h"
// #include "proto/common/string.pb.h"

#include "common/ringBuffer.h"
#include "ad_rscl/ad_rscl.h"

#define DEFAULT_DURATION 10
#define DEFAULT_CAMERA_FRAMERATE 20
#define DEFAULT_CHASSIS_FRAMERATE 100
#define DEFAULT_INS_FRAMERATE 100
#define DEFAULT_PORT 9876
#define DEFAULT_STORAGE_RATIO 85


#define DEFAULT_CAMERA8M_BUF_SIZE 1024*1024*2
#define DEFAULT_CAMERA2M_BUF_SIZE 1024*1024
#define DEFAULT_CHASSIS_BUF_SIZE 1024*10
#define DEFAULT_GNSS_BUF_SIZE 1024*10

#define BLOCK_SIZE 1024*1024
#define MAX_EVENTS 10

using namespace shadow::common;

#if defined MW_ART_ORIN
using stoic::cm::proto::common::PString;

struct shadow_sensor_buffer {
    std::string channel_name;
    PString pbuffer;
    uint64_t timestamp;
};

struct channel_data_buffer {
     int trigger_forward_duration;
     int trigger_backword_duration;
     boost::circular_buffer<shadow_sensor_buffer> fw_camera_cb;
     boost::circular_buffer<shadow_sensor_buffer> fr_camera_cb;
     boost::circular_buffer<shadow_sensor_buffer> fl_camera_cb;
     boost::circular_buffer<shadow_sensor_buffer> rr_camera_cb;
     boost::circular_buffer<shadow_sensor_buffer> rl_camera_cb;
     boost::circular_buffer<shadow_sensor_buffer> r_camera_cb;
     boost::circular_buffer<shadow_sensor_buffer> chassis_cb;
     boost::circular_buffer<shadow_sensor_buffer> ins_cb;
     boost::circular_buffer<shadow_sensor_buffer> f_lidar_cb;
     boost::circular_buffer<shadow_sensor_buffer> l_lidar_cb;
     boost::circular_buffer<shadow_sensor_buffer> r_lidar_cb;
     boost::circular_buffer<shadow_sensor_buffer> rr_lidar_cb;
};

enum SENSOR_TYPE : uint8_t {
    SENSOR_TYPE_UNKNOWN	= 0,
    SENSOR_TYPE_FW_CAMERA,
    SENSOR_TYPE_FR_CAMERA,
    SENSOR_TYPE_FL_CAMERA,
    SENSOR_TYPE_RR_CAMERA,
    SENSOR_TYPE_RL_CAMERA,
    SENSOR_TYPE_R_CAMERA,
    SENSOR_TYPE_FFISHEYE_CAMERA,
    SENSOR_TYPE_LFISHEYE_CAMERA,
    SENSOR_TYPE_RFISHEYE_CAMERA,
    SENSOR_TYPE_RRFISHEYE_CAMERA,
    SENSOR_TYPE_LIDAR,
    SENSOR_TYPE_CHASSIS,
    SENSOR_TYPE_INS,
    SNESOR_TYPE_ODO,
    SENSOR_TYPE_IMU,
    SENSOR_TYPE_MAX
};
#elif defined MW_RSCL_THOR
using TRawMessagePtr = std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>;

struct shadow_sensor_buffer {
    std::string channel_name;
    TRawMessagePtr pbuffer;
    uint64_t timestamp;
};

struct channel_data_buffer {
     int trigger_forward_duration;
     int trigger_backword_duration;
     RingBuffer<shadow_sensor_buffer> fw_camera_rb;
     RingBuffer<shadow_sensor_buffer> fr_camera_rb;
     RingBuffer<shadow_sensor_buffer> fl_camera_rb;
     RingBuffer<shadow_sensor_buffer> rr_camera_rb;
     RingBuffer<shadow_sensor_buffer> rl_camera_rb;
     RingBuffer<shadow_sensor_buffer> r_camera_rb;
     RingBuffer<shadow_sensor_buffer> chassis_rb;
};

enum SENSOR_TYPE : uint8_t {
    SENSOR_TYPE_UNKNOWN	= 0,
    SENSOR_TYPE_FW_CAMERA,
    SENSOR_TYPE_FR_CAMERA,
    SENSOR_TYPE_FL_CAMERA,
    SENSOR_TYPE_RR_CAMERA,
    SENSOR_TYPE_RL_CAMERA,
    SENSOR_TYPE_R_CAMERA,
    SENSOR_TYPE_CHASSIS
};

#endif
