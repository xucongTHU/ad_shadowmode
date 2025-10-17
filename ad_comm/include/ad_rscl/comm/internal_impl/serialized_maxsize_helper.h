
/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once
#include <stdint.h>
#include <type_traits>
#include <utility>

/**
 * @note this helper used to Check Capnp Struct if has GetMaxSize method.
 * if it has GetMaxSize method, set SerializedMaxSize.
 */

/**
 * @brief Check if capnp struct has GetMaxSize method.
 *
 * @tparam MessageT
 */
template <typename T>
class HasGetMaxSizeFunc {
 private:
    template <typename U>
    static auto Check(int)  // NOLINT
        -> decltype(std::declval<U>().GetMaxSize(), std::true_type());
    template <typename U>
    static std::false_type Check(...);

 public:
    enum { value = std::is_same<decltype(Check<T>(0)), std::true_type>::value };
};

template <typename T>
typename std::enable_if<HasGetMaxSizeFunc<T>::value, uint32_t>::type
GetMsgSerializedMaxSize() {
    return T::GetMaxSize();
}

template <typename T>
typename std::enable_if<!HasGetMaxSizeFunc<T>::value, uint32_t>::type
GetMsgSerializedMaxSize() {
    return 0;
}
