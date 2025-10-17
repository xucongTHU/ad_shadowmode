/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstddef>
namespace senseAD {
namespace base {
struct EnumClassHash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};
}  // namespace base
}  // namespace senseAD
