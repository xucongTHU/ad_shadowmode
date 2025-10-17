/**
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung<fengzhongjie@senseauto.com>
 */
#pragma once

#include <type_traits>

namespace senseAD {
namespace rscl {
namespace comm {
namespace someip {

// Check for non-const Enumerate function
template <typename T>
class HasNonConstEnumerateFn {
 private:
    template <typename U>
    static std::true_type Test(
        decltype(std::declval<U&>().template Enumerate<int>(
            std::declval<int&&>()))*);

    template <typename U>
    static std::false_type Test(...);

 public:
    static constexpr bool value =
        std::is_same<decltype(Test<T>(0)), std::true_type>::value;
};

// Check for const Enumerate function
template <typename T>
class HasConstEnumerateFn {
 private:
    template <typename U>
    static std::true_type Test(
        decltype(std::declval<const U&>().template Enumerate<int>(
            std::declval<int&&>()))*);

    template <typename U>
    static std::false_type Test(...);

 public:
    static constexpr bool value =
        std::is_same<decltype(Test<T>(0)), std::true_type>::value;
};

// Type traits for compile time error messages
template <typename T, bool HasNonConst>
struct ValidateNonConstEnumerate {
    static_assert(HasNonConst,
                  "\nError: Missing non-const Enumerate function!"
                  "\nPlease add:\n"
                  "template<typename Helper>\n"
                  "void Enumerate(Helper&& helper) {\n"
                  "    helper(member1);\n"
                  "    helper(member2);\n"
                  "}\n");
};

template <typename T>
struct ValidateNonConstEnumerate<T, true> {};

template <typename T, bool HasConst>
struct ValidateConstEnumerate {
    static_assert(HasConst,
                  "\nError: Missing const Enumerate function!"
                  "\nPlease add:\n"
                  "template<typename Helper>\n"
                  "void Enumerate(Helper&& helper) const {\n"
                  "    helper(member1);\n"
                  "    helper(member2);\n"
                  "}\n");
};

template <typename T>
struct ValidateConstEnumerate<T, true> {};

// Helper function for validation
template <typename T>
inline void ValidateEnumerateFunction() {
    typedef ValidateNonConstEnumerate<T, HasNonConstEnumerateFn<T>::value>
        CheckNonConst;
    typedef ValidateConstEnumerate<T, HasConstEnumerateFn<T>::value> CheckConst;
    (void)sizeof(CheckNonConst);
    (void)sizeof(CheckConst);
}

// Unified trait for checking
template <typename T>
struct HasEnumerateFn {
    static constexpr bool value =
        HasNonConstEnumerateFn<T>::value && HasConstEnumerateFn<T>::value;
};

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
