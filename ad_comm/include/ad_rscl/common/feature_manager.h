/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include "ad_mw/ad_mw.h"

namespace senseAD {

namespace rscl {

namespace common {

using SupportedFeature = senseAD::mw::SupportedFeature;

template <SupportedFeature Feature>
constexpr bool IsFeatureSupported() {
    return senseAD::mw::IsFeatureSupported<Feature>::value;
}

}  // namespace common
}  // namespace rscl
}  // namespace senseAD
