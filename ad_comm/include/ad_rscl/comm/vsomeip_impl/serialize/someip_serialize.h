/**
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * nakajima <nakajima@senseauto.com>
 */
#pragma once

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <array>
#include <cstddef>
#include <exception>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include "ad_rscl/comm/vsomeip_impl/serialize/helper_traits.h"
#include "ad_base/std/variant.h"

namespace senseAD {
namespace rscl {
namespace comm {
namespace someip {

#if 0
#define CM_VCLS_TL typename... Types
#define CM_V_TL Types...
#else
#define CM_VCLS_TL ARA_CORE_VCLS_TL16
#define CM_V_TL ARA_CORE_V_TL16
#endif

using senseAD::base::get;
using senseAD::base::Variant;
using senseAD::base::variant_alternative;
using senseAD::base::variant_size;

static_assert(sizeof(uint8_t) == 1, "Unexpected data size");
static_assert(sizeof(uint16_t) == 2, "Unexpected data size");
static_assert(sizeof(uint32_t) == 4, "Unexpected data size");

static_assert(sizeof(uint8_t) == 1, "Unexpected data size");
static_assert(sizeof(uint16_t) == 2, "Unexpected data size");
static_assert(sizeof(uint32_t) == 4, "Unexpected data size");

static_assert(sizeof(float) == 4, "Unexpected data size");
static_assert(sizeof(double) == 8, "Unexpected data size");

// Big endian is not supported.

// Required types (numerical + boolean)
// boolean / bool

// uint8 sint8
// uint16 sint16
// uint32 sint32

// float32
// float64

// string UTF-8 32bit dynamic length
// array 32bit dynamic length

// static structure
typedef uint32_t uint32;
typedef bool boolean;
typedef std::string string;

struct SerializerProps {
    uint32 alignment = 8;
    enum ByteOrderEnum {
        mostSignificantByteFirst = 0,
        mostSignificantByteLast = 1,
        opaque = 2
    } byteOrder = mostSignificantByteLast;
    boolean implementsLegacyStringSerialization = false;
    boolean isDynamicLengthFieldSize = false;
    enum SessionHandling {
        sessionHandlingActive = 0,
        sessionHandlingInactive = 1
    } sessionHandling = sessionHandlingInactive;
    uint32 sizeOfArrayLengthField = 4;
    uint32 sizeOfStringLengthField = 4;
    uint32 sizeOfStructLengthField = 4;
    uint32 sizeOfUnionLengthField = 4;
    uint32 sizeOfUnionTypeSelectorField = 4;
    string stringEncoding = "UTF-8";

 public:
    bool Validate() const;
};

template <typename T>
bool WriteDataByteOrder(T src,
                        void *dst,
                        SerializerProps::ByteOrderEnum byteOrder);

template <typename T>
bool ReadDataByteOrder(T src,
                       void *dst,
                       SerializerProps::ByteOrderEnum byteOrder);

class Serializer {
 private:
    uint8_t *payload_begin_;
    uint8_t *end_;

 public:
    explicit Serializer(const SerializerProps *props);
    Serializer(uint8_t *buffer,
               size_t buffer_size,
               const SerializerProps *props);
    uint8_t *ptr_;
    const SerializerProps *serializer_props_;

    bool IsDryRun() const;
    void AdvancePtrBySize(size_t size);
    const SerializerProps *GetSerializerPropsPtr() { return serializer_props_; }
    SerializerProps::ByteOrderEnum GetByteOrder() const {
        return serializer_props_->byteOrder;
    }

    /*
    void advance_ptr_to_align(Serializer *s, size_t alignment) {
        // It must consider header size.
        // Alignment should be power of 2
        assert(alignment & (alignment - 1) == 0);
        s_ptr = (s_ptr + (alignment - 1)) & - (ssize_t)alignment;
    }
    */

    bool HasCapacity(size_t size) const;
    size_t PayloadSizeByte() const;

    // Boundary check might be removed in future version. Don't rely on them.
    // return true if succeeded.
    bool WriteBytes(const void *src, size_t bytes);

    // return true if succeeded.
    template <typename T>
    bool WriteData(const T &data) {
        if (!IsDryRun()) {
            if (!HasCapacity(sizeof(data))) return false;
            if (!WriteDataByteOrder(data, ptr_, GetByteOrder())) return false;
        }
        AdvancePtrBySize(sizeof(data));
        return true;
    }
};

struct Deserializer {
    const uint8_t *payload_begin_;
    const uint8_t *end_;

 public:
    Deserializer(const uint8_t *buffer,
                 size_t buffer_size,
                 const SerializerProps *props);

    const uint8_t *ptr_;
    const SerializerProps *serializer_props_;
    SerializerProps::ByteOrderEnum GetByteOrder() const {
        return serializer_props_->byteOrder;
    }

    void AdvancePtrBySize(size_t size);
    bool HasCapacity(size_t size) const;
    size_t PayloadSizeByte() const;
    const SerializerProps *GetSerializerPropsPtr() { return serializer_props_; }
};

/**
 * @brief An helper class to serialize all the member of a message recusively
 *
 */
class EnumerateSomeIpSerializeHelper {
 public:
    explicit EnumerateSomeIpSerializeHelper(Serializer &serializer)  // NOLINT
        : serializer_(serializer) {}

    template <typename T>
    typename std::enable_if<HasEnumerateFn<T>::value, void>::type operator()(
        const T &v) {  // NOLINT
        v.Enumerate(*this);
    }

    template <typename T>
    typename std::enable_if<!HasEnumerateFn<T>::value, void>::type operator()(
        const T &v) {
        succeeded_ &= Write(&serializer_, v);
    }

    bool is_succeeded() { return succeeded_; }

 private:
    Serializer &serializer_;
    bool succeeded_ = true;
};

/**
 * @brief An helper class to deserialize all the member of a message recusively
 *
 */
class EnumerateSomeIpDeserializeHelper {
 public:
    explicit EnumerateSomeIpDeserializeHelper(Deserializer &deserializer)  // NOLINT
        : deserializer_(deserializer) {}

    template <typename T>
    typename std::enable_if<HasEnumerateFn<T>::value, void>::type operator()(
        T &v) {  // NOLINT
        v.Enumerate(*this);
    }

    template <typename T>
    typename std::enable_if<!HasEnumerateFn<T>::value, void>::type operator()(
        T &v) {  // NOLINT
        typename std::remove_const<T>::type value;
        succeeded_ &= Read(&deserializer_, value);
        v = value;
    }

    bool is_succeeded() { return succeeded_; }

 private:
    Deserializer &deserializer_;
    bool succeeded_ = true;
};

bool Write(Serializer *s, const bool &data);  // NOLINT
bool Read(Deserializer *s, bool &data);  // NOLINT

bool Write(Serializer *s, const std::string &str);
bool Read(Deserializer *s, std::string &data);  // NOLINT

template <
    typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
bool Write(Serializer *s, const T &data) {
    return s->WriteData(data);
}

template <
    typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
bool Read(Deserializer *s, T &data) {  // NOLINT
    constexpr size_t datasize_byte = sizeof(T);
    if (!s->HasCapacity(datasize_byte)) return false;
    if (!ReadDataByteOrder(*reinterpret_cast<const T *>(s->ptr_), &data,
                           s->GetByteOrder()))
        return false;
    s->AdvancePtrBySize(datasize_byte);
    return true;
}

template <typename T>
bool WriteVector(Serializer *s, const std::vector<T> &array) {
    const auto ptr_size_field = s->ptr_;

    switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
        case 1:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint8_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint8_t));
            break;

        case 2:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint16_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint16_t));
            break;

        case 4:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint32_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint32_t));
            break;

        default:
            return false;
    }

    const auto ptr_contents_begin = s->ptr_;

    for (const auto &elem : array) {
        if (!Write(s, elem)) return false;
    }

    if (!s->IsDryRun()) {
        switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
            case 1: {
                uint8_t *serialized_contents_length =
                    reinterpret_cast<uint8_t *>(ptr_size_field);
                uint8_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            case 2: {
                uint16_t *serialized_contents_length =
                    reinterpret_cast<uint16_t *>(ptr_size_field);
                uint16_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            case 4: {
                uint32_t *serialized_contents_length =
                    reinterpret_cast<uint32_t *>(ptr_size_field);
                uint32_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            default:
                return false;
        }
    }

    return true;
}

template <typename T>
bool Write(Serializer *s, const std::vector<T> &array) {
    return WriteVector(s, array);
}

template <typename T>
bool ReadVector(Deserializer *s, std::vector<T> &data) {  // NOLINT
    uint32_t contents_size;

    switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
        case 1: {
            if (!s->HasCapacity(sizeof(uint8_t))) return false;
            uint8_t contents_size_l;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint8_t *>(s->ptr_),
                                   &contents_size_l, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size_l));
            contents_size = contents_size_l;
            break;
        }

        case 2: {
            if (!s->HasCapacity(sizeof(uint16_t))) return false;
            uint16_t contents_size_l;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint16_t *>(s->ptr_),
                                   &contents_size_l, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size_l));
            contents_size = contents_size_l;
            break;
        }

        case 4: {
            if (!s->HasCapacity(sizeof(uint32_t))) return false;
            uint32_t contents_size_l;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint32_t *>(s->ptr_),
                                   &contents_size_l, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size_l));
            contents_size = contents_size_l;
            break;
        }

        default:
            return false;
    }

    if (!s->HasCapacity(contents_size)) return false;

    data.clear();
    const auto ptr_contents_end = s->ptr_ + contents_size;
    while (s->ptr_ < ptr_contents_end) {
        T elem;
        if (!Read(s, elem)) return false;
        data.emplace_back(elem);
    }
    return true;
}

template <typename T>
bool Read(Deserializer *s, std::vector<T> &data) {  // NOLINT
    return ReadVector(s, data);
}

template <typename T, size_t n>
bool Write(Serializer *s, const std::array<T, n> &array) {
    for (const auto &elem : array) {
        if (!Write(s, elem)) return false;
    }
    return true;
}

template <typename T, size_t n>
bool Read(Deserializer *s, std::array<T, n> &data) {  // NOLINT
    for (auto &elem : data) {
        if (!Read(s, elem)) return false;
    }
    return true;
}

template <typename K, typename V>
bool WriteMap(Serializer *s, const std::map<K, V> &array) {
    const auto ptr_size_field = s->ptr_;

    switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
        case 1:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint8_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint8_t));
            break;

        case 2:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint16_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint16_t));
            break;

        case 4:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint32_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint32_t));
            break;

        default:
            return false;
    }

    const auto ptr_contents_begin = s->ptr_;

    for (const auto &elem : array) {
        if (!Write(s, elem.first)) return false;
        if (!Write(s, elem.second)) return false;
    }

    if (!s->IsDryRun()) {
        switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
            case 1: {
                uint8_t *serialized_contents_length =
                    reinterpret_cast<uint8_t *>(ptr_size_field);
                uint8_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            case 2: {
                uint16_t *serialized_contents_length =
                    reinterpret_cast<uint16_t *>(ptr_size_field);
                uint16_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            case 4: {
                uint32_t *serialized_contents_length =
                    reinterpret_cast<uint32_t *>(ptr_size_field);
                uint32_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            default:
                return false;
        }
    }

    return true;
}

template <typename K, typename V>
bool Write(Serializer *s, const std::map<K, V> &array) {
    return WriteMap(s, array);
}

template <typename K, typename V>
bool ReadMap(Deserializer *s, std::map<K, V> &data) {  // NOLINT
    uint32_t contents_size;

    switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
        case 1: {
            if (!s->HasCapacity(sizeof(uint8_t))) return false;
            uint8_t contents_size_l;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint8_t *>(s->ptr_),
                                   &contents_size_l, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size_l));
            contents_size = contents_size_l;
            break;
        }

        case 2: {
            if (!s->HasCapacity(sizeof(uint16_t))) return false;
            uint16_t contents_size_l;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint16_t *>(s->ptr_),
                                   &contents_size_l, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size_l));
            contents_size = contents_size_l;
            break;
        }

        case 4: {
            if (!s->HasCapacity(sizeof(uint32_t))) return false;
            uint32_t contents_size_l;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint32_t *>(s->ptr_),
                                   &contents_size_l, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size_l));
            contents_size = contents_size_l;
            break;
        }

        default:
            return false;
    }

    if (!s->HasCapacity(contents_size)) return false;

    data.clear();
    const auto ptr_contents_end = s->ptr_ + contents_size;
    while (s->ptr_ < ptr_contents_end) {
        K key;
        V value;
        if (!Read(s, key)) return false;
        if (!Read(s, value)) return false;
        data[key] = value;
    }

    return true;
}

template <typename K, typename V>
bool Read(Deserializer *s, std::map<K, V> &data) {  // NOLINT
    return ReadMap(s, data);
}

template <size_t I, CM_VCLS_TL>
struct WVC;

template <CM_VCLS_TL>
struct WVC<0, CM_V_TL> {
    static bool WriteVariantContents(Serializer *, const Variant<CM_V_TL> &) {
        assert(false);
        return false;
    }
};

template <size_t I, CM_VCLS_TL>
struct WVC {
    static bool WriteVariantContents(Serializer *s,
                                     const Variant<CM_V_TL> &data) {
        if (variant_size<Variant<CM_V_TL>>::value <= I) {
            assert(false);
            return false;
        }
        if (I == data.index()) {
            return Write(s, get<I>(data));
        }
        return WVC<I - 1, CM_V_TL>::WriteVariantContents(s, data);
    }
};

template <CM_VCLS_TL>
bool WriteVariant8bitType(Serializer *s, const Variant<CM_V_TL> &data) {
    static_assert(
        variant_size<Variant<CM_V_TL>>::value <= 0xff,
        "Size of variant is too large to serialize with 8 bit type field");

    const auto ptr_size_field = s->ptr_;

    switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
        case 1:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint8_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint8_t));
            break;

        case 2:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint16_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint16_t));
            break;

        case 4:
            if (!s->IsDryRun()) {
                if (!s->HasCapacity(sizeof(uint32_t))) return false;
            }
            s->AdvancePtrBySize(sizeof(uint32_t));
            break;

        default:
            return false;
    }

    if (!s->WriteData<uint8_t>(data.index())) {
        return false;
    }

    const auto ptr_contents_begin = s->ptr_;

    if (!WVC<variant_size<Variant<CM_V_TL>>::value - 1,
             CM_V_TL>::WriteVariantContents(s, data))
        return false;
    if (!s->IsDryRun()) {
        switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
            case 1: {
                uint8_t *serialized_contents_length =
                    reinterpret_cast<uint8_t *>(ptr_size_field);
                uint8_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            case 2: {
                uint16_t *serialized_contents_length =
                    reinterpret_cast<uint16_t *>(ptr_size_field);
                uint16_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            case 4: {
                uint32_t *serialized_contents_length =
                    reinterpret_cast<uint32_t *>(ptr_size_field);
                uint32_t length = s->ptr_ - ptr_contents_begin;
                if (!WriteDataByteOrder(length, serialized_contents_length,
                                        s->GetByteOrder()))
                    return false;
                break;
            }

            default:
                return false;
        }
    }

    return true;
}

template <CM_VCLS_TL>
bool Write(Serializer *s, const Variant<CM_V_TL> &data) {
    return WriteVariant8bitType(s, data);
}

template <size_t I, CM_VCLS_TL>
struct RVC;

template <CM_VCLS_TL>
struct RVC<0, CM_V_TL> {
    static bool ReadVariantContents(Deserializer *,
                                    Variant<CM_V_TL> &,
                                    size_t) {
        assert(false);
        return false;
    }
};

template <size_t I, CM_VCLS_TL>
struct RVC {
    static bool ReadVariantContents(Deserializer *s,
                                    Variant<CM_V_TL> &data,  // NOLINT
                                    size_t index) {
        if (variant_size<Variant<CM_V_TL>>::value <= I) {
            assert(false);
            return false;
        }
        if (I == index) {
            typename variant_alternative<I, Variant<CM_V_TL>>::type value;
            if (!Read(s, value)) return false;
            data = value;
            return true;
        }
        return RVC<I - 1, CM_V_TL>::ReadVariantContents(s, data, index);
    }
};

template <CM_VCLS_TL>
bool ReadVariant8bitType(Deserializer *s, Variant<CM_V_TL> &data) {  // NOLINT
    static_assert(
        variant_size<Variant<CM_V_TL>>::value <= 0xff,
        "Size of variant is too large to serialize with 8 bit type field");

    switch (s->GetSerializerPropsPtr()->sizeOfArrayLengthField) {
        case 1: {
            if (!s->HasCapacity(sizeof(uint8_t))) return false;
            uint8_t contents_size;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint8_t *>(s->ptr_),
                                   &contents_size, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size));
            break;
        }

        case 2: {
            if (!s->HasCapacity(sizeof(uint16_t))) return false;
            uint16_t contents_size;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint16_t *>(s->ptr_),
                                   &contents_size, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size));
            break;
        }

        case 4: {
            if (!s->HasCapacity(sizeof(uint32_t))) return false;
            uint32_t contents_size;
            if (!ReadDataByteOrder(*reinterpret_cast<const uint32_t *>(s->ptr_),
                                   &contents_size, s->GetByteOrder()))
                return false;
            s->AdvancePtrBySize(sizeof(contents_size));
            break;
        }

        default:
            return false;
    }

    if (!s->HasCapacity(sizeof(uint8_t))) return false;
    const uint8_t contents_type_index =
        *reinterpret_cast<const uint8_t *>(s->ptr_);
    s->AdvancePtrBySize(sizeof(contents_type_index));

    if (!RVC<variant_size<Variant<CM_V_TL>>::value - 1,
             CM_V_TL>::ReadVariantContents(s, data, contents_type_index))
        return false;
    return true;
}

template <CM_VCLS_TL>
bool Read(Deserializer *s, Variant<CM_V_TL> &data) {  // NOLINT
    return ReadVariant8bitType(s, data);
}

template <typename T,
          typename std::enable_if_t<!std::is_arithmetic<T>::value, int> = 0>
bool Write(Serializer *s, const T &data) {
    ValidateEnumerateFunction<T>();
    EnumerateSomeIpSerializeHelper helper(*s);
    helper(data);
    return helper.is_succeeded();
}

template <typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value, int>::type = 0>
bool Read(Deserializer *s, T &data) {  // NOLINT
    ValidateEnumerateFunction<T>();
    EnumerateSomeIpDeserializeHelper helper(*s);
    helper(data);
    return helper.is_succeeded();
}

template <typename T>
size_t SerializedSize(const T &data, const SerializerProps *props = nullptr) {
    if (props && !props->Validate()) {
        return 0;
    }
    Serializer s(props);
    if (Write(&s, data)) return s.PayloadSizeByte();
    return 0;
}

template <typename T>
bool Serialize(const T &data,
               uint8_t *buffer,
               size_t buffer_size,
               const SerializerProps *props = nullptr) {
    if (props && !props->Validate()) {
        return false;
    }
    Serializer s(buffer, buffer_size, props);
    return Write(&s, data);
}

template <typename T>
bool Deserialize(T *data,
                 uint8_t *buffer,
                 size_t buffer_size,
                 const SerializerProps *props = nullptr) {
    if (props && !props->Validate()) {
        return false;
    }
    Deserializer d(buffer, buffer_size, props);
    return Read(&d, *data);
}

}  // namespace someip
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
