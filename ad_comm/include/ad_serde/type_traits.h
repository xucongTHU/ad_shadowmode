/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

// https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function
#define HAS_MEM_FUNC(func, name)                                        \
    template <typename T>                                               \
    struct name {                                                       \
        typedef char yes[1];                                            \
        typedef char no[2];                                             \
        template <typename C>                                           \
        static yes &chk(decltype(&C::func) *);                          \
        template <typename>                                             \
        static no &chk(...);                                            \
        static bool constexpr value = sizeof(chk<T>(0)) == sizeof(yes); \
    }

HAS_MEM_FUNC(ParseFromString, HasParseFromString);
HAS_MEM_FUNC(SerializeToString, HasSerializeToString);
