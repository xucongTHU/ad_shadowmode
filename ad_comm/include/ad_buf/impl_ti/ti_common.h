/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Zhang Yue <zhangyue3@senseauto.com>
 * zhangyi <zhangyi15@senseauto.com>
 */

#pragma once

#include <VX/vx.h>
// #include <VX/vx_helper.h>
#include <VX/vx_khr_pipelining.h>

#include <TI/tivx.h>
#include <TI/tivx_target_kernel.h>
#include <TI/tivx_task.h>
#include <tivx_kernels_host_utils.h>

#include <TI/j7_kernels_imaging_aewb.h>
#include <TI/j7_tidl.h>
#include <TI/j7_video_encoder.h>

extern "C" {
// #include <tivx_utils_file_rd_wr.h>
// #include <tivx_utils_graph_perf.h>
#include <app_init.h>
#include <app_iss.h>
}

#include <memory>
#include <mutex>

#define MACRO_CONCAT_(prefix, suffix) prefix##suffix
#define MACRO_CONCAT(prefix, suffix) MACRO_CONCAT_(prefix, suffix)

// useful macros for openvx error checking
#define VX_STATUS_CHECK(status, failure_block)                              \
    {                                                                       \
        vx_status status_ = (status);                                       \
        if (status_ != VX_SUCCESS) {                                        \
            AD_LERROR(VX_STATUS_CHECK) << "failed with status:" << status_; \
            failure_block;                                                  \
        }                                                                   \
    }
#define VX_STATUS_CHECK_CONTINUE(status) VX_STATUS_CHECK(status, continue)
#define VX_STATUS_CHECK_BREAK(status) VX_STATUS_CHECK(status, break)
#define VX_STATUS_CHECK_RETURN(status, ...) \
    VX_STATUS_CHECK(status, return __VA_ARGS__)
#define VX_STATUS_CHECK_RETURN_STATUS(status)                    \
    {                                                            \
        vx_status MACRO_CONCAT(status_, __LINE__) = (status);    \
        VX_STATUS_CHECK(MACRO_CONCAT(status_, __LINE__),         \
                        return MACRO_CONCAT(status_, __LINE__)); \
    }

#define VX_OBJECT_CHECK(obj, failure_block) \
    VX_STATUS_CHECK(vxGetStatus((vx_reference)(obj)), failure_block)
#define VX_OBJECT_CHECK_CONTINUE(status) VX_OBJECT_CHECK(status, continue)
#define VX_OBJECT_CHECK_BREAK(status) VX_OBJECT_CHECK(status, break)
#define VX_OBJECT_CHECK_RETURN(status, ...) \
    VX_OBJECT_CHECK(status, return __VA_ARGS__)
#define VX_OBJECT_CHECK_RETURN_STATUS(obj)                       \
    {                                                            \
        vx_status MACRO_CONCAT(status_, __LINE__) =              \
            vxGetStatus((vx_reference)(obj));                    \
        VX_STATUS_CHECK(MACRO_CONCAT(status_, __LINE__),         \
                        return MACRO_CONCAT(status_, __LINE__)); \
    }

namespace senseAD {
namespace ad_buf {

bool TiMallocGetFd(vx_uint32 size,
                   vx_enum mem_heap_region,
                   void*& data,      // NOLINT
                   uint64_t& fd,     // NOLINT
                   void*& phyaddr);  // NOLINT

bool TiGetDataFromFd(uint64_t fd,
                     vx_uint32 size,
                     void*& data,      // NOLINT
                     void*& phyaddr);  // NOLINT

void TiFreeMem(void* data, vx_uint32 size, vx_enum mem_heap_region);

class TiInitilizer {
 public:
    class TiInitializerImpl {
     public:
        TiInitializerImpl() {
            appCommonInit();
            tivxInit();
            tivxHostInit();
        }
        ~TiInitializerImpl() {
            tivxHostDeInit();
            tivxDeInit();
            appCommonDeInit();
        }
    };

    TiInitilizer() : impl_{getImpl()} {}

    ~TiInitilizer() {
        if (impl_ && impl_.use_count() == 2) {
            impl_.reset();
            g_impl_.reset();
        }
    }

 private:
    std::shared_ptr<TiInitializerImpl> getImpl() {
        static std::once_flag flag;
        std::call_once(flag, [impl = &g_impl_]() {
            *impl = std::make_shared<TiInitializerImpl>();
        });
        return g_impl_;
    }

 private:
    std::shared_ptr<TiInitializerImpl> impl_;
    static std::shared_ptr<TiInitializerImpl> g_impl_;
};
}  // namespace ad_buf
}  // namespace senseAD
