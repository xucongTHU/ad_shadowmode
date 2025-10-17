/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <type_traits>
#include <exception>
#include <memory>
#include <utility>
#include <iostream>

namespace senseAD {
namespace base {

struct in_place_t {
    explicit in_place_t() = default;  // NOLINT
};
constexpr in_place_t in_place{};

class bad_optional_access : public std::exception {
 public:
    explicit bad_optional_access() {}  // NOLINT
    virtual char const* what() const noexcept {
        return "Invalid optional access";
    }
};

// Ref:
// http://www.club.cc.cmu.edu/~ajo/disseminate/2017-02-15-Optional-From-Scratch.pdf

template <typename T>
constexpr bool trivial = std::is_trivially_constructible<T>::value&&
    std::is_trivially_destructible<T>::value;

template <class T, class E = void>
struct optional_storage {
    bool has_value_ = false;
    union {
        char dummy_;
        T val_;
    };

    optional_storage() : dummy_(0) {}
    optional_storage(const optional_storage& rhs) {
        has_value_ = rhs.has_value_;
        if (rhs.has_value_) new (&val_) T(rhs.val_);
    }
    optional_storage(optional_storage&& rhs) {
        has_value_ = rhs.has_value_;
        if (rhs.has_value_) new (&val_) T(std::forward<T>(rhs.val_));
    }
    optional_storage& operator=(const optional_storage& rhs) {
        if (has_value_) val_.~T();
        has_value_ = rhs.has_value_;
        if (rhs.has_value_) new (&val_) T(rhs.val_);
        return *this;
    }
    optional_storage& operator=(optional_storage&& rhs) {
        if (has_value_) val_.~T();
        has_value_ = rhs.has_value_;
        if (rhs.has_value_) new (&val_) T(std::forward<T>(rhs.val_));
        return *this;
    }
    ~optional_storage() {
        if (has_value_) val_.~T();
    }
};

template <class T>
struct optional_storage<T, std::enable_if_t<trivial<T>>> {
    union {
        char dummy_;
        T val_;
    };
    bool has_value_;
    optional_storage() : dummy_(0) {}
    ~optional_storage() = default;
};

template <class T>
struct optional {
 public:
    constexpr optional() noexcept {
        storage_.has_value_ = false;
        storage_.dummy_ = 0;
    }
    constexpr optional(std::nullptr_t) noexcept {  // NOLINT
        storage_.has_value_ = false;
        storage_.dummy_ = 0;
    }

    optional(optional&& rhs) noexcept(
        std::is_nothrow_move_constructible<T>::value) {
        storage_ = std::move(rhs.storage_);
    }

    optional(const optional& rhs) { storage_ = rhs.storage_; }

    template <class U = T>
    optional(U&& u) {  // NOLINT
        storage_.has_value_ = true;
        new (&storage_.val_) T(std::forward<U>(u));
    }

    template <class... Args>
    constexpr explicit optional(in_place_t, Args&&... args) {
        storage_.has_value_ = true;
        new (&storage_.val_) T(std::forward<Args>(args)...);
    }

    ~optional() = default;

    template <class... Args>
    void emplace(Args&&... args) {
        reset();
        storage_.has_value_ = true;
        new (&storage_.val_) T(std::forward<Args>(args)...);
    }

    void reset() {
        if (storage_.has_value_) {
            storage_.val_.~T();
            storage_.has_value_ = false;
        }
    }

    T& value() & {
        if (!storage_.has_value_) {
            throw bad_optional_access();
        }
        return storage_.val_;
    }
    const T& value() const& {
        if (!storage_.has_value_) {
            throw bad_optional_access();
        }
        return storage_.val_;
    }
    T&& value() && {
        if (!storage_.has_value_) {
            throw bad_optional_access();
        }
        return std::move(storage_.val_);
    }
    const T&& value() const&& {
        if (!storage_.has_value_) {
            throw bad_optional_access();
        }
        return std::move(storage_.val_);
    }

    template <class U = T>
    T value_or(U&& u) const {
        if (storage_.has_value_) {
            return storage_.val_;
        } else {
            return u;
        }
    }
    bool has_value() const { return storage_.has_value_; }

    optional& operator=(const T& rhs) {
        emplace(rhs);
        return *this;
    }
    optional& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    constexpr T& operator*() & noexcept { return storage_.val_; }
    constexpr const T& operator*() const& noexcept { return storage_.val_; }
    constexpr T&& operator*() && { return std::move(storage_.val_); }
    constexpr const T&& operator*() const&& { return std::move(storage_.val_); }

    constexpr T* operator->() noexcept { return &storage_.val_; }
    constexpr const T* operator->() const noexcept { return &storage_.val_; }

    // TODO(kit) Add comparision operator

    explicit operator bool() const noexcept { return storage_.has_value_; }

 private:
    optional_storage<T> storage_;
};

template <typename T>
constexpr std::enable_if_t<std::is_constructible<std::decay_t<T>, T>::value,
                           optional<std::decay_t<T>>>
make_optional(T&& __t) noexcept(
    std::is_nothrow_constructible<optional<std::decay_t<T>>, T>::value) {
    return optional<std::decay_t<T>>{std::forward<T>(__t)};
}

template <typename T, typename... Args>
constexpr std::enable_if_t<std::is_constructible<T, Args...>::value,
                           optional<T>>
make_optional(Args&&... args) noexcept(
    std::is_nothrow_constructible<T, Args...>::value) {
    return optional<T>{in_place, std::forward<Args>(args)...};
}

}  // namespace base
}  // namespace senseAD
