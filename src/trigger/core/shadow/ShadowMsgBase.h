/*******************************************************************
* Copyright (c) 2025 T3CAIC. All rights reserved.
*
* @file ShadowMsgBase.h
* @brief 自定义shadow相关消息
*
* @author maqiang
* @date 2025-05-09
*******************************************************************/
#pragma once

#include <Eigen/Eigen>
#include <vector>
#include <stdint.h>
#include <memory>

#include "yaml-cpp/yaml.h"

namespace shadow::trigger {

using float64_t = float;

// 常量定义
const int SHADOW_MAX_DETECT_OBJ = 50; // 单帧最大检测目标个数
const int SHADOW_MAX_CLASSNAME = 50;  // className最大长度
const int SHADOW_DIM_BBOX = 4;        // 检测框维度
const int SHADOW_BBOX_L_INDEX = 0;    // 检测框左侧索引
const int SHADOW_BBOX_T_INDEX = 1;    // 检测框上侧索引
const int SHADOW_BBOX_R_INDEX = 2;    // 检测框右侧索引
const int SHADOW_BBOX_B_INDEX = 3;    // 检测框下侧索引
const int SHADOW_PLANNING_POINT_CNT = 50; // 规划预测点数
const long SHADOW_TIME_CONVERSION = 1000l;  // 相邻时间精度差异

/**
 * @brief 单个检测目标信息
 */
struct ShadowDetection {
  float bbox[SHADOW_DIM_BBOX];           // x1, y1, x2, y2
  float conf;                            // 检测置信度
  uint8_t classId;                       // 类别id
  char className[SHADOW_MAX_CLASSNAME];  // 类别名字

  // 获取检测框的大小
  float box_size() {
    return (bbox[SHADOW_BBOX_R_INDEX] - bbox[SHADOW_BBOX_L_INDEX]) * (bbox[SHADOW_BBOX_B_INDEX] - bbox[SHADOW_BBOX_T_INDEX]);
  }
};

/**
 * @brief 2d检测结果
 */
struct ShadowDetectModelOutput {
  uint64_t stamp = 0;
  uint64_t sensorStamp = 0;
  uint8_t validCnt = 0;                   // 有效的检测个数
  ShadowDetection detRes[SHADOW_MAX_DETECT_OBJ];
};

/**
 * @brief 算法决策输出结果
 */
struct ShadowModelCtlOutput  {
  uint64_t stamp = 0;
  float acc = 0.0f;                    // acc控制值
  float brake = 0.0f;                  // 制动板控制值
  float throttle = 0.0f;               // 油门控制值
  float wheelAngle = 0.0f;             // 方向盘转角
  float wheelAngularVelocity = 0.0f;   // 方向盘角速度
};

template <typename _T>
void ShadowLoadNode(const YAML::Node& node, _T& value, std::string name) {
  if (node[name]) {
    value = node[name].as<_T>();
  } else {
    std::cout << "[Error] Can't find node " << name << std::endl;
  }
}

}  // namespace shadow::trigger