/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <system_error>  // NOLINT
#include <string>
#include <vector>

namespace senseAD {
namespace rscl {

enum class RsclError {
    kSuccess = 0,
    // Rscl comm ERROR  2001 ~ 2100
    kPublisherIsNullptr = 2001,
    kLoanBufferIsNullptr = 2002,
    kPublishExternalFailed = 2003,
    kUndefinedError = 9999,
};

class RsclErrorCategory : public std::error_category {
 public:
    static RsclErrorCategory const& instance() {
        static RsclErrorCategory instance;
        return instance;
    }

    char const* name() const noexcept override;

    std::string message(int code) const override;

    static RsclError Find(const std::error_code& code);
};

std::error_code make_error_code(RsclError code);

}  // namespace rscl
}  // namespace senseAD

namespace std {
template <>
struct is_error_code_enum<senseAD::rscl::RsclError> : true_type {};
}  // namespace std
