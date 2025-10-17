/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <exception>
#include <string>

#include "ad_mw/exception.h"

namespace senseAD {

namespace rscl {

class InvalidGlobalConfigException : public std::exception {
 public:
    explicit InvalidGlobalConfigException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class InvalidComponentConfigException : public std::exception {
 public:
    explicit InvalidComponentConfigException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class UnexpectedProgrammingException : public std::exception {
 public:
    explicit UnexpectedProgrammingException(std::string const &message) {
        msg_ = message + "\nPlease contact the module owner for debug\n";
    }
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class MsgTypeVerifyException : public std::exception {
 public:
    explicit MsgTypeVerifyException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class AuthenticationFailException : public std::exception {
 public:
    explicit AuthenticationFailException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class NotSupportSameNameException : public std::exception {
 public:
    explicit NotSupportSameNameException(std::string const &message) {
        msg_ = message +
               "\nPlease try set suffix name, param path comm_conf.proto. \n";
    }
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class MsgBridgeConfigMissingException : public std::exception {
 public:
    explicit MsgBridgeConfigMissingException(std::string const &message) {
        msg_ = message +
               "\nPlease add the msg bridge config. Or don't use this API. \n";
    }
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

}  // namespace rscl
}  // namespace senseAD
