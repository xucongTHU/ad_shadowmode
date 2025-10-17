/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <exception>
#include <string>

namespace senseAD {

namespace mw {

#define REGISTER_EXCEPTION(NAME)                                           \
    class NAME : public std::exception {                                   \
     public:                                                               \
        explicit NAME(std::string const &message) : msg_(message) {}       \
        virtual char const *what() const noexcept { return msg_.c_str(); } \
                                                                           \
     private:                                                              \
        std::string msg_;                                                  \
    };

REGISTER_EXCEPTION(UnknownRemoteException)
REGISTER_EXCEPTION(TopicSizeTooLargeException)
REGISTER_EXCEPTION(LoadTopicConfigException)
REGISTER_EXCEPTION(CreateWriterFailedExecption)
REGISTER_EXCEPTION(CreateReaderFailedExecption)
REGISTER_EXCEPTION(NotSupportedException)
REGISTER_EXCEPTION(WrongIdentityTypeExecption)
REGISTER_EXCEPTION(MwInitError)

#undef REGISTER_EXCEPTION

}  // namespace mw
}  // namespace senseAD
