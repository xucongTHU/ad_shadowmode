/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */
#ifndef __AD_MSG_BRIDGE_MANAGER_EXCEPTION_H__
#define __AD_MSG_BRIDGE_MANAGER_EXCEPTION_H__

#pragma once

#include <exception>
#include <string>

namespace senseAD {
namespace ad_msg_bridge {

class LoadBridgeCoreConfigException : public std::exception {
 public:
    explicit LoadBridgeCoreConfigException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class InvalidExternalTypeException : public std::exception {
 public:
    explicit InvalidExternalTypeException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class InvalidRsclTypeException : public std::exception {
 public:
    explicit InvalidRsclTypeException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

}  // namespace ad_msg_bridge
}  // namespace senseAD

#endif  // __AD_MSG_BRIDGE_MANAGER_EXCEPTION_H__
