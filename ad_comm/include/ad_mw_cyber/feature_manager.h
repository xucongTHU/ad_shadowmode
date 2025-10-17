/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */
#pragma once

#include "ad_mw/feature_manager.h"

namespace senseAD {

namespace mw {
template <>
struct IsFeatureSupported<
    SupportedFeature::SUPPORT_MULTI_PUBLISHER_IN_UNIQUE_CHANNEL>
    : std::true_type {};

template <>
struct IsFeatureSupported<SupportedFeature::SUPPORT_NATIVELY_SERVICE_DISCOVERY>
    : std::true_type {};

}  // namespace mw
}  // namespace senseAD
