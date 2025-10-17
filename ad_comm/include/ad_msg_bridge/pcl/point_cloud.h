/*
 * Copyright (C) 2020-2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <cstdint>

namespace senseAD {
namespace ad_msg_bridge {
struct IdProp {
    uint8_t ring_id;
    uint8_t lidar_id;
};
struct LidarPointXYZIR {
    PCL_ADD_POINT4D;
    float intensity;
    union {
        std::uint16_t ring;
        IdProp id;
    };
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} EIGEN_ALIGN16;
}  // namespace ad_msg_bridge
}  // namespace senseAD

POINT_CLOUD_REGISTER_POINT_STRUCT(senseAD::ad_msg_bridge::LidarPointXYZIR,
                                  (float, x, x)(float, y, y)(float, z, z)(
                                      float,
                                      intensity,
                                      intensity)(std::uint16_t, ring, ring))
