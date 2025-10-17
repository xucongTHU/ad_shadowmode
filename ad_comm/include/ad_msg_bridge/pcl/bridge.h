/*
 * Copyright (C) 2020-2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cstring>
#include <memory>

#include "ad_msg_bridge/pcl/point_cloud.h"
#include "ad_serde/ad_serde.h"
#include "ad_msg_idl/std_msgs/raw_data.capnp.h"

namespace senseAD {
namespace ad_msg_bridge {
using AdPointCloud = senseAD::msg::std_msgs::PointCloud;

template <typename T>
typename std::enable_if<(sizeof(T) == sizeof(LidarPointXYZIR))>::type
CloudXYZIRToProtoCloud(const pcl::PointCloud<T> &in,
                       std::shared_ptr<SendMsg<AdPointCloud>> *out) {
    //! Never reduce the point buf size to avoid unneeded memory operation
    size_t data_size = sizeof(LidarPointXYZIR) * in.size();
    auto builder = (*out)->MsgBuilder();
    auto pointbuf = builder.getPointBuf();

    if (pointbuf.size() < data_size) {
        *out = std::make_shared<SendMsg<AdPointCloud>>();
        builder = (*out)->MsgBuilder();
        pointbuf = builder.initPointBuf(data_size);
    }
    memcpy(&pointbuf[0], &in[0], data_size);

    builder.setIsDense(in.is_dense);
    builder.setWidth(in.width);
    builder.setHeight(in.height);
    builder.setPointCount(in.size());
    builder.setStep(sizeof(LidarPointXYZIR));
}

template <typename T>
void CloudXYZIRTToProtoCloud(const pcl::PointCloud<T> &in,
                             std::shared_ptr<SendMsg<AdPointCloud>> *out) {
    //! Never reduce the point buf size to avoid unneeded memory operation
    size_t data_size = sizeof(T) * in.size();
    auto builder = (*out)->MsgBuilder();
    auto pointbuf = builder.getPointBuf();

    if (pointbuf.size() < data_size) {
        *out = std::make_shared<SendMsg<AdPointCloud>>();
        builder = (*out)->MsgBuilder();
        pointbuf = builder.initPointBuf(data_size);
    }
    memcpy(&pointbuf[0], &in[0], data_size);

    builder.setIsDense(in.is_dense);
    builder.setWidth(in.width);
    builder.setHeight(in.height);
    builder.setPointCount(in.size());
    builder.setStep(sizeof(T));
}

template <typename T>
typename std::enable_if<(sizeof(T) == sizeof(pcl::PointXYZI))>::type
ProtoCloudToCloudXYZI(const AdPointCloud::Reader &in, pcl::PointCloud<T> *out) {
    out->is_dense = in.getIsDense();
    out->width = in.getWidth();
    out->height = in.getHeight();
    int n = in.getPointCount();
    out->resize(n);

    const auto &point_buf = in.getPointBuf();
    int step = in.getStep();
    for (int i = 0; i < n; ++i) {
        // Implicit assumed
        // - x, y, z, intensity is at front of the input buf point type
        // - the input x,y,z,intensity is float
        memcpy(&(*out)[i], &point_buf[i * step], sizeof(T));
    }
}

template <typename T>
typename std::enable_if<(sizeof(T) == sizeof(LidarPointXYZIR))>::type
ProtoCloudToCloudXYZIR(const AdPointCloud::Reader &in,
                       pcl::PointCloud<T> *out) {
    out->is_dense = in.getIsDense();
    out->width = in.getWidth();
    out->height = in.getHeight();

    size_t data_size = in.getPointCount() * sizeof(LidarPointXYZIR);
    if (data_size == 0) {
        return;
    }
    out->resize(in.getPointCount());
    memcpy(&(*out)[0], &in.getPointBuf()[0], data_size);
}

}  // namespace ad_msg_bridge
}  // namespace senseAD
