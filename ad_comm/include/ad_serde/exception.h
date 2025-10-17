/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace senseAD {

namespace serde {

class AccessSendMsgBufferBeforeBuildException : public std::exception {
 public:
    explicit AccessSendMsgBufferBeforeBuildException(
        const std::string& func_name) {
        std::stringstream ss;
        ss << "Function [" << func_name
           << "] cannot be call before Finalize. This is a programming error. "
              "You should fix your code";
        msg_ = ss.str();
    }
    virtual char const* what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class SendMsgBufferRepeatedBuildException : public std::exception {
 public:
    SendMsgBufferRepeatedBuildException() {}
    virtual char const* what() const noexcept {
        return "SendMsg MUST NOT build more than 1 time. It is a programming "
               "error. Fix your code";
    }
};

class MessageParseException : public std::exception {
 public:
    MessageParseException(const std::string& message_type,
                          const std::string& message_name,
                          const std::string& extra_info = "") {
        std::stringstream ss;
        ss << "Error while parsing [" << message_type << "/" << message_name
           << "]. This is a runtime error, Check the data flow | "
           << extra_info;
        msg_ = ss.str();
    }
    virtual char const* what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

}  // namespace serde
}  // namespace senseAD
