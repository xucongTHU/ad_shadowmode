/**
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung<fengzhongjie@senseauto.com>
 */

/**
 * @file
 * @brief Interface to class ara_core::varient
 */

#ifndef MODULES_AD_BASE_INCLUDE_AD_BASE_STD_VARIANT_H_
#define MODULES_AD_BASE_INCLUDE_AD_BASE_STD_VARIANT_H_

#include <utility>  // for swap
#include "nonstd/variant.hpp"

namespace senseAD {
namespace base {

#define ARA_CORE_VCLS_TL16                                                \
    class T0, class T1, class T2, class T3, class T4, class T5, class T6, \
        class T7, class T8, class T9, class T10, class T11, class T12,    \
        class T13, class T14, class T15
#define ARA_CORE_V_TL16 \
    T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15

/**
 * @brief a type-safe union
 *
 * @tparam Types the types that may be stored in this variant. All types must
 * meet the Destructible requirements (in particular, array types and non-object
 * types are not allowed).
 */

template <class T0,
          class T1 = nonstd::variants::detail::T1,
          class T2 = nonstd::variants::detail::T2,
          class T3 = nonstd::variants::detail::T3,
          class T4 = nonstd::variants::detail::T4,
          class T5 = nonstd::variants::detail::T5,
          class T6 = nonstd::variants::detail::T6,
          class T7 = nonstd::variants::detail::T7,
          class T8 = nonstd::variants::detail::T8,
          class T9 = nonstd::variants::detail::T9,
          class T10 = nonstd::variants::detail::T10,
          class T11 = nonstd::variants::detail::T11,
          class T12 = nonstd::variants::detail::T12,
          class T13 = nonstd::variants::detail::T13,
          class T14 = nonstd::variants::detail::T14,
          class T15 = nonstd::variants::detail::T15>
using Variant = nonstd::variant<ARA_CORE_V_TL16>;

template <class... Types>
using variant_size = nonstd::variant_size<Types...>;

template <std::size_t K, class... Types>
using variant_alternative = nonstd::variant_alternative<K, Types...>;

template <class T, ARA_CORE_VCLS_TL16>
inline bool holds_alternative(Variant<ARA_CORE_V_TL16> const& v)
    variant_noexcept {
    return nonstd::holds_alternative<T>(v);
}

template <class R, ARA_CORE_VCLS_TL16>
inline R& get(Variant<ARA_CORE_V_TL16>& v,  // NOLINT
              nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R)) {
    return nonstd::get<R>(v, nonstd_lite_in_place_type(R));
}

template <class R, ARA_CORE_VCLS_TL16>
inline R const& get(
    Variant<ARA_CORE_V_TL16> const& v,
    nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R)) {
    return nonstd::get<R>(v, nonstd_lite_in_place_type(R));
}

template <std::size_t K, ARA_CORE_VCLS_TL16>
inline typename nonstd::
    variant_alternative<K, nonstd::variant<ARA_CORE_V_TL16> >::type&
    get(nonstd::variant<ARA_CORE_V_TL16>& v,  // NOLINT
        nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
#if variant_CONFIG_NO_EXCEPTIONS
    assert(K == v.index());
#else
    if (K != v.index()) {
        throw nonstd::bad_variant_access();
    }
#endif
    return v.template get<K>();
}

template <std::size_t K, ARA_CORE_VCLS_TL16>
inline typename nonstd::
    variant_alternative<K, nonstd::variant<ARA_CORE_V_TL16> >::type const&
    get(nonstd::variant<ARA_CORE_V_TL16> const& v,
        nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
#if variant_CONFIG_NO_EXCEPTIONS
    assert(K == v.index());
#else
    if (K != v.index()) {
        throw nonstd::bad_variant_access();
    }
#endif
    return v.template get<K>();
}

template <class R, ARA_CORE_VCLS_TL16>
inline R&& get(Variant<ARA_CORE_V_TL16>&& v,
               nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R)) {
    return nonstd::get<R>(std::forward(v), nonstd_lite_in_place_type(R));
}

template <class R, ARA_CORE_VCLS_TL16>
inline R const&& get(
    Variant<ARA_CORE_V_TL16> const&& v,
    nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R)) {
    return nonstd::get<R>(std::forward(v), nonstd_lite_in_place_type(R));
}

template <std::size_t K, ARA_CORE_VCLS_TL16>
inline typename nonstd::
    variant_alternative<K, nonstd::variant<ARA_CORE_V_TL16> >::type&&
    get(nonstd::variant<ARA_CORE_V_TL16>&& v,
        nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
#if variant_CONFIG_NO_EXCEPTIONS
    assert(K == v.index());
#else
    if (K != v.index()) {
        throw nonstd::bad_variant_access();
    }
#endif
    return std::move(v.template get<K>());
}

template <std::size_t K, ARA_CORE_VCLS_TL16>
inline typename nonstd::
    variant_alternative<K, nonstd::variant<ARA_CORE_V_TL16> >::type const&&
    get(nonstd::variant<ARA_CORE_V_TL16> const&& v,
        nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
#if variant_CONFIG_NO_EXCEPTIONS
    assert(K == v.index());
#else
    if (K != v.index()) {
        throw nonstd::bad_variant_access();
    }
#endif
    return std::move(v.template get<K>());
}

template <ARA_CORE_VCLS_TL16>
void swap(Variant<ARA_CORE_V_TL16>& lhs, Variant<ARA_CORE_V_TL16>& rhs) {
    lhs.swap(rhs);
}

}  // namespace base
}  // namespace senseAD

#endif  // MODULES_AD_BASE_INCLUDE_AD_BASE_STD_VARIANT_H_
