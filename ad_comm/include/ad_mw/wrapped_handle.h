/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <utility>

#include "ad_mw/visibility_control.h"

namespace senseAD {
namespace mw {

using UniqueVoidPtr = std::unique_ptr<void, void (*)(void const*)>;
using SharedVoidPtr = std::shared_ptr<void>;

/**
 * @brief Create a unique pointer with any type while hidding it actual type
 *
 * @tparam T Actual type
 * @param ptr Data Pointer
 * @return UniqueVoidPtr
 */
template <typename T>
auto MakeUniqueVoid(T* ptr) -> UniqueVoidPtr {
    return UniqueVoidPtr(ptr, [](void const* data) {
        T const* p = static_cast<T const*>(data);
        delete p;
    });
}

/**
 * @brief Wrapper Class to Store the internal handle in void unique pointer
 *
 */
class WrapperdPtrInnerHandleU {
 public:
    explicit WrapperdPtrInnerHandleU(UniqueVoidPtr&& internal_handle)
        : internal_handle_(std::move(internal_handle)) {}
    virtual ~WrapperdPtrInnerHandleU() = default;

    /**
     * @brief Get the Internal Handle Pointer
     *
     * @return void*
     */
    inline void* GetInternalHandle() const { return internal_handle_.get(); }

 protected:
    UniqueVoidPtr internal_handle_;
};

/**
 * @brief Wrapper Class to Store the internal handle in void shared pointer
 *
 */
class WrapperdPtrInnerHandleS {
 public:
    explicit WrapperdPtrInnerHandleS(SharedVoidPtr internal_handle)
        : internal_handle_(internal_handle) {}

    virtual ~WrapperdPtrInnerHandleS() = default;

    /**
     * @brief Get the Internal Handle Pointer
     *
     * @return void*
     */
    inline void* GetInternalHandle() const { return internal_handle_.get(); }

 protected:
    SharedVoidPtr internal_handle_;
};

}  // namespace mw
}  // namespace senseAD
