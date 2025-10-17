/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <system_error>  // NOLINT
#include <string>
namespace senseAD {

namespace mw {
enum class MwErrorCode {
    // MW ERROR 1001 ~ 2000
    kSuccess = 0,
    kFail = 1,
    kWriteGoOverMaxLengthError = 1001,
    kWriterBufferError = 1002,
    kWriterBufferDataError = 1003,
    kNotSupportLoanBuffer = 1004,
};

class MwErrorCategory : public std::error_category {
 public:
    static MwErrorCategory const& instance() {
        static MwErrorCategory instance;
        return instance;
    }

    char const* name() const noexcept override;

    std::string message(int code) const override;
};

std::error_code make_error_code(MwErrorCode code);

}  // namespace mw
}  // namespace senseAD

// 特化模版
namespace std {
template <>
struct is_error_code_enum<senseAD::mw::MwErrorCode> : true_type {};
}  // namespace std
