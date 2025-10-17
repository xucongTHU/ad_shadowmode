/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <bitset>
#include <array>
#include <unordered_set>

#include "ad_mw/visibility_control.h"

namespace senseAD {
namespace mw {

enum class SupportedFeature {
    // 0. support multiple senders of pipes with the same name
    SUPPORT_MULTI_PUBLISHER_IN_UNIQUE_CHANNEL = 0,
    // 1. support natively service discovery(like cyber)
    SUPPORT_NATIVELY_SERVICE_DISCOVERY = 1,
    // 2. support borrowing buffers
    SUPPORT_LOAN_BUFFER = 2
};

// 每个实现需要特化这个模板
template <SupportedFeature Feature>
struct IsFeatureSupported : std::false_type {};

// 编译时检查特性
template <SupportedFeature Feature>
constexpr bool CheckFeatureSupported() {
    return IsFeatureSupported<Feature>::value;
}

}  // namespace mw
}  // namespace senseAD
