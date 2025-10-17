/*
 * Copyright (C) 2020-2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

#include "ad_buf/ad_buf.h"
#include "ad_base/common/macros.h"

namespace senseAD {
namespace ad_buf {

class AdBufManagerFactory {
 public:
    using FactoryFn = std::function<std::shared_ptr<AdBufManager>(
        const std::string&, const BufCfg*)>;
    using FactoryReconstructFn =
        std::function<std::shared_ptr<AdBufManager>(const std::string&)>;

    std::error_code RegisterImpl(const std::string& buf_type,
                                 FactoryFn factory_fn);
    std::error_code RegisterImpl(const std::string& buf_type,
                                 FactoryReconstructFn factory_fn);

    std::shared_ptr<AdBufManager> Construct(const std::string& buf_type,
                                            const std::string& mmaped_file,
                                            const BufCfg* meta);
    std::shared_ptr<AdBufManager> ReConstruct(const std::string& buf_type,
                                              const std::string& mmaped_file);

 private:
    std::unordered_map<std::string, FactoryFn> fns_;
    std::unordered_map<std::string, FactoryReconstructFn> reconstruct_fns_;

    DECLARE_AD_SINGLETON(AdBufManagerFactory)
};

}  // namespace ad_buf
}  // namespace senseAD

#define REG_ADBUF_IMPL_INTERNAL_1(reg_fn, UniqueID)                   \
    namespace {                                                       \
    struct RegAdbufImpl_##UniqueID {                                  \
        RegAdbufImpl_##UniqueID() { reg_fn(); }                       \
    };                                                                \
    static RegAdbufImpl_##UniqueID g_adbuf_register_class_##UniqueID; \
    }

#define REG_ADBUF_IMPL_MED(reg_fn, idx) REG_ADBUF_IMPL_INTERNAL_1(reg_fn, idx)
#define REG_ADBUF_IMPL(reg_fn) REG_ADBUF_IMPL_MED(reg_fn, __COUNTER__)
