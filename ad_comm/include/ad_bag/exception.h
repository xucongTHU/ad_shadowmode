/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <exception>
#include <string>

namespace senseAD {

namespace bag {

/**
 * @brief All io related exception occured in bag read/write
 *
 */
class BagCompatibleException : public std::exception {
 public:
    explicit BagCompatibleException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class BagIteratorException : public std::exception {
 public:
    explicit BagIteratorException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

/**
 * @brief All io related exception occured in bag read/write
 *
 */
class BagIoException : public std::exception {
 public:
    explicit BagIoException(std::string const &message) : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class WriteMessageException : public std::exception {
 public:
    explicit WriteMessageException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

class InvalidBagFormatException : public std::exception {
 public:
    explicit InvalidBagFormatException(std::string const &message)
        : msg_(message) {}
    virtual char const *what() const noexcept { return msg_.c_str(); }

 private:
    std::string msg_;
};

}  // namespace bag
}  // namespace senseAD
