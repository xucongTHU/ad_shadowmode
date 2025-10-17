/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <mutex>
#include <new>
#include <memory>
#include <iostream>
#include <atomic>

// Reference: cyber/common/macros.h

#define NON_COPYABLE_ASSIGNABLE(T) \
    T(const T &) = delete;         \
    T &operator=(const T &) = delete;

#ifdef DECLARE_AD_SINGLETON
#undef DECLARE_AD_SINGLETON
#endif

#define DECLARE_AD_SINGLETON(T)                                             \
 public:                                                                    \
    static T *Instance(const bool create_if_needed = true);                 \
    static std::shared_ptr<T> &SharedInstance(const bool create_if_needed = \
                                                  true);                    \
                                                                            \
 private:                                                                   \
    T();                                                                    \
    NON_COPYABLE_ASSIGNABLE(T)

#define DEFINE_AD_SINGLETON_INSTANCE_CC(T)                                   \
    T *T::Instance(const bool create_if_needed) {                            \
        return SharedInstance(create_if_needed).get();                       \
    }                                                                        \
    std::shared_ptr<T> &T::SharedInstance(const bool create_if_needed) {     \
        static std::shared_ptr<T> instance_sptr = nullptr;                   \
        static std::once_flag flag;                                          \
        if (!instance_sptr && create_if_needed) {                            \
            std::call_once(                                                  \
                flag, [&] { instance_sptr.reset(new (std::nothrow) T()); }); \
        }                                                                    \
        return instance_sptr;                                                \
    }

#define DEFINE_AD_TYPE_HAS_FUNC_TRAIT(name, func)              \
    template <typename T>                                      \
    struct name {                                              \
        template <typename Class>                              \
        static constexpr bool Test(decltype(&Class::func) *) { \
            return true;                                       \
        }                                                      \
        template <typename>                                    \
        static constexpr bool Test(...) {                      \
            return false;                                      \
        }                                                      \
                                                               \
        static constexpr bool value = Test<T>(nullptr);        \
    };                                                         \
                                                               \
    template <typename T>                                      \
    constexpr bool name<T>::value;

#define DEFINE_AD_TYPE_HAS_SUBSTRUCT_TRAIT(name, substruct)       \
    template <typename T>                                         \
    struct name {                                                 \
        template <typename Class>                                 \
        static constexpr bool Test(typename Class::substruct *) { \
            return true;                                          \
        }                                                         \
        template <typename>                                       \
        static constexpr bool Test(...) {                         \
            return false;                                         \
        }                                                         \
                                                                  \
        static constexpr bool value = Test<T>(nullptr);           \
    };                                                            \
                                                                  \
    template <typename T>                                         \
    constexpr bool name<T>::value;
