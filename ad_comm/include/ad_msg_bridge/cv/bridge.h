/**
 * Copyright (C) 2020-2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file bridge.h
 * @brief Conversion function
 *
 */

#pragma once

#include <cstring>
#include <memory>

#include <opencv2/core.hpp>

#ifndef OPENCV_VERSION_GT_2
// OpenCv 2
#include <opencv2/gpu/gpu.hpp>
namespace cv_gpu = cv::gpu;
#else
// OpenCv 3
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudawarping.hpp>
namespace cv_gpu = cv::cuda;
#endif

#include "ad_msg_idl/std_msgs/raw_data.capnp.h"
#include "ad_serde/ad_serde.h"

namespace senseAD {
namespace ad_msg_bridge {

/**
 * @brief Convert opencv gpumat into idl Image with resize
 *
 * @param in input opencv gpumat
 * @param resize_size output size
 * @param resize_buf a buffer for intermediate step
 * @param out output idl Image
 */
template <typename CvMat>
void CvGpuImageProtoImageWithResize(
    const CvMat &in,
    const cv::Size resize_size,
    CvMat *resize_buf,
    std::shared_ptr<SendMsg<senseAD::msg::std_msgs::Image>> *out) {
    auto step = resize_size.width * 3;

    auto img_size = step * resize_size.height;

    auto builder = (*out)->MsgBuilder();

    auto data_ptr = builder.getData();
    if (img_size != static_cast<int>(data_ptr.asBytes().size())) {
        *out = std::make_shared<SendMsg<senseAD::msg::std_msgs::Image>>();
        builder = (*out)->MsgBuilder();
        builder.initData(img_size);
        data_ptr = builder.getData();
        builder.setEncoding("bgr8");
    }

    builder.setOriWidth(in.size().width);
    builder.setOriHeight(in.size().height);

    builder.setWidth(resize_size.width);
    builder.setHeight(resize_size.height);

    builder.setStep(step);

    cv_gpu::resize(in, *resize_buf, resize_size);
    cv::Mat dst(resize_size.height, resize_size.width, CV_8UC3, &data_ptr[0]);
    resize_buf->download(dst);
}

/**
 * @brief Convert idl Image into opencv Mat
 *
 * @param in input idl Image
 * @param out output opencv Mat
 */
inline void ProtoImageToCvImageInplace(
    const senseAD::msg::std_msgs::Image::Reader &in, cv::Mat *out) {
    *out = cv::Mat(in.getHeight(), in.getWidth(), CV_8UC3,
                   const_cast<char *>(&in.getData().asChars()[0]));
}

}  // namespace ad_msg_bridge
}  // namespace senseAD
