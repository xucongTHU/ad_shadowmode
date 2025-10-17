/*
 * Copyright (C) 2020-2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <iostream>
#include <system_error>

namespace senseAD {
namespace ad_buf {

enum class ErrorCode {
    kSuccess = 0,
    kRepeatedRegAdBufManagerImpl = 1,
};

std::error_code make_error_code(ErrorCode code) {
    return {
        static_cast<int>(code),
        std::generic_category(),
    };
}

}  // namespace ad_buf
}  // namespace senseAD

namespace std {
template <>
struct is_error_code_enum<senseAD::ad_buf::ErrorCode> : true_type {};
}  // namespace std
