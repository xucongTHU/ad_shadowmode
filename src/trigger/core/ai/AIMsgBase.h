/*******************************************************************
* Copyright (c) 2025 T3CAIC. All rights reserved.
*
* @file AiMsgBase.h
* @brief 自定义AI相关消息
*
* @author maqiang
* @date 2025-05-08
*******************************************************************/
#pragma once

#include <vector>
#include <stdint.h>
#include <memory>

#include "cm/cm.h"
#include "caic_interface.h"

namespace shadow::trigger {

// 常量定义
const int AI_MAX_DETECT_OBJ = 50; // 单帧最大检测目标个数
const int AI_MAX_TRACK_OBJ = 100; // 单帧最大跟踪目标个数
const int AI_MAX_CLASSNAME = 50;  // className最大长度
const int AI_DIM_BBOX = 4;        // 检测框维度
const int AI_OBJ_POS_DIM = 3;     // 雷达目标位置维度
const int AI_OBJ_LWH_DIM = 3;     // 雷达目标尺寸维度
const int AI_BBOX_L_INDEX = 0;    // 检测框左侧索引
const int AI_BBOX_T_INDEX = 1;    // 检测框上侧索引
const int AI_BBOX_R_INDEX = 2;    // 检测框右侧索引
const int AI_BBOX_B_INDEX = 3;    // 检测框下侧索引
const long AI_TIME_CONVERSION = 1000l; // 相邻时间精度差异

/**
 * @brief 单个检测目标信息
 */
struct AIDetection {
  float bbox[AI_DIM_BBOX];            // x1, y1, x2, y2
  float conf;                         // 检测置信度
  uint8_t classId;                    // 类别id
  char className[AI_MAX_CLASSNAME];   // 类别名字

  // 获取检测框的大小
  float box_size() {
    return (bbox[AI_BBOX_R_INDEX] - bbox[AI_BBOX_L_INDEX]) 
         * (bbox[AI_BBOX_B_INDEX] - bbox[AI_BBOX_T_INDEX]);
  }
};

/**
 * @brief 2d检测结果
 */
struct AIDetectModelOutput : public caic_std::MessageBase {
  int64_t stamp = -1;
  int64_t sensorStamp = -1;
  uint8_t validCnt = 0;                   // 有效的检测个数
  AIDetection result[AI_MAX_DETECT_OBJ];
};

/**
 * @brief 视觉感知跟踪结果
 */
struct AIVisionTrackObject {
  int trackId;                       // 轨迹id
  float position[AI_OBJ_POS_DIM];    // 自车坐标系下目标的位置
  float speed[AI_OBJ_POS_DIM];       // 自车坐标系下目标速度
  float lwh[AI_OBJ_LWH_DIM];         // 目标的长宽高
  float yaw;                         // 自车坐标下目标的yaw
  AIDetection det2d;                 // 2d的检测记过
};

/**
 * @brief 视觉感知结果
 */
struct AIVisionModelOutput : public caic_std::MessageBase {
  int64_t stamp = -1;
  int64_t sensorStamp = -1;
  uint8_t validCnt = 0;                              // 有效跟踪个数
  AIVisionTrackObject visionRes[AI_MAX_TRACK_OBJ];   // 跟踪结果
};

/**
 * @brief 雷达目标信息
 */
struct AIDetectionLidar {
  uint8_t classId;                   // 类别
  char className[AI_MAX_CLASSNAME];  // 类名
  float position[AI_OBJ_POS_DIM];    // 自车坐标系下的位置
  float speed[AI_OBJ_POS_DIM];       // 自车坐标系下的速度
  float yaw;                         // 自车坐标下的yaw
  float lwh[AI_OBJ_LWH_DIM];         // 长宽高
  int trackId;                       // 跟踪id
};

/**
 * @brief 雷达感知结果
 */
struct AILidarModelOutput : public caic_std::MessageBase {
  int64_t stamp = -1;
  int64_t sensorStamp = -1;
  uint8_t validCnt = 0;                   // 有效个数
  AIDetectionLidar result[AI_MAX_TRACK_OBJ];
};

// 路况枚举
enum class EMRoadType {
  ROAD_UNSET = -1,
  ROAD_HIGHWAY = 0,
  ROAD_URBAN = 1,
  ROAD_BRIDGE = 2,
  ROAD_TUNNEL = 3
};

// 天气枚举
enum class EMWeatherType {
  WEATHER_UNSET = -1,
  WEATHER_SUNNY = 0,
  WEATHER_RAINY = 1,
  WEATHER_OVERCAST = 2,
  WEATHER_UNKNOW = 3
};

// 时间枚举
enum class EMTimeType {
  TIME_UNSET = -1,
  TIME_DAY = 0,
  TIME_NIGHT = 1
};

/**
 * @brief 路况感知结果
 */
struct AIRoadConditionOutput : public caic_std::MessageBase {
  int64_t stamp;
  int64_t sensorStamp = -1;
  EMRoadType roadType = EMRoadType::ROAD_UNSET;
  EMWeatherType weatherType = EMWeatherType::WEATHER_UNSET;
  EMTimeType timeType = EMTimeType::TIME_UNSET;
};

}  // namespace shadow::trigger