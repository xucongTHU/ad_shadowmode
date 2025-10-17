/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#pragma once

#include "ad_base/class_loader/class_loader_utility.h"

#ifdef CLASS_LOADER_REGISTER_CLASS_INTERNAL
#undef CLASS_LOADER_REGISTER_CLASS_INTERNAL
#endif
#ifdef CLASS_LOADER_REGISTER_CLASS_INTERNAL_1
#undef CLASS_LOADER_REGISTER_CLASS_INTERNAL_1
#endif
#ifdef CLASS_LOADER_REGISTER_CLASS
#undef CLASS_LOADER_REGISTER_CLASS
#endif

//************************************************************************
//   Use case 1. Auto register the class when the so is loaded
// - user don't need to care which cc files is combined at one so
// - not fulfill the init order consistent rule
//************************************************************************
#define CLASS_LOADER_REGISTER_CLASS_INTERNAL(Derived, Base, UniqueID)    \
    namespace {                                                          \
    struct ProxyType##UniqueID {                                         \
        ProxyType##UniqueID() {                                          \
            senseAD::base::class_loader::utility::RegisterClass<Derived, \
                                                                Base>(   \
                #Derived, #Base);                                        \
        }                                                                \
    };                                                                   \
    static ProxyType##UniqueID g_register_class_##UniqueID;              \
    }

#define CLASS_LOADER_REGISTER_CLASS_INTERNAL_1(Derived, Base, UniqueID) \
    CLASS_LOADER_REGISTER_CLASS_INTERNAL(Derived, Base, UniqueID)

// register class macro
#define CLASS_LOADER_REGISTER_CLASS(Derived, Base) \
    CLASS_LOADER_REGISTER_CLASS_INTERNAL_1(Derived, Base, __COUNTER__)

//************************************************************************
//   Use case 2. Let user to provide a entry point that determined which class
//   need to be loaded
// - user need to care which cc files is combined at one so
// - fulfill the init order consistent rule
//************************************************************************

#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base)        \
    senseAD::base::class_loader::utility::RegisterClass<Derived, Base>( \
        #Derived, #Base);

#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I2(Derived, Base, ...) \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base)          \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(__VA_ARGS__)
#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I3(Derived, Base, ...) \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base)          \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I2(__VA_ARGS__)
#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I4(Derived, Base, ...) \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base)          \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I3(__VA_ARGS__)
#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I5(Derived, Base, ...) \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base)          \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I4(__VA_ARGS__)
#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I6(Derived, Base, ...) \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base)          \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I5(__VA_ARGS__)
#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I7(Derived, Base, ...) \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base)          \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I6(__VA_ARGS__)
#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I8(Derived, Base, ...) \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base)          \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I7(__VA_ARGS__)

#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I(n, ...) \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I##n(__VA_ARGS__)

#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER(Derived, Base)    \
    extern "C" {                                                 \
    __attribute__((visibility("default"))) void                  \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_FN_NAME() {           \
        CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I1(Derived, Base) \
    }                                                            \
    }

#define CLASS_LOADER_SHARED_OBJECT_INITIALIZER_N(n, ...)         \
    extern "C" {                                                 \
    __attribute__((visibility("default"))) void                  \
    CLASS_LOADER_SHARED_OBJECT_INITIALIZER_FN_NAME() {           \
        CLASS_LOADER_SHARED_OBJECT_INITIALIZER_I(n, __VA_ARGS__) \
    }                                                            \
    }
