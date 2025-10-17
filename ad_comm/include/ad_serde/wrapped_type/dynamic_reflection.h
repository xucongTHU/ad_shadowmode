/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <string>
#include <memory>
#include <vector>

#include "ad_base/std/optional.h"
#include "ad_serde/wrapped_type/base.h"

namespace senseAD {
namespace serde {

enum class DynamicValue {
    UNSUPPORTED,
    VOID,
    BOOL,
    INT,
    UINT,
    FLOAT,
    TEXT,
    DATA,
    LIST,
    ENUM,
    STRUCT
};

class DynamicObjectReader;
class DynamicStructReader;
class DynamicListReader;
class DynamicEnumReader;

class DynamicObjectReader {
 public:
    class Impl {
     public:
        virtual ~Impl() noexcept(false) {}
        virtual DynamicValue GetType() const = 0;

        virtual bool as_bool() const = 0;
        virtual uint64_t as_uint() const = 0;
        virtual int64_t as_int() const = 0;
        virtual double as_float() const = 0;
        virtual std::string as_str() const = 0;
        virtual std::vector<char> as_data() const = 0;
        virtual DynamicListReader as_list() const = 0;
        virtual DynamicEnumReader as_enum() const = 0;
        virtual DynamicStructReader as_struct() const = 0;
    };

 public:
    DynamicObjectReader();
    DynamicObjectReader(const DynamicObjectReader& rhs) : impl_(rhs.impl_) {}
    DynamicObjectReader(DynamicStructReader struct_reader);  // NOLINT
    DynamicObjectReader(DynamicListReader list_reader);      // NOLINT
    explicit DynamicObjectReader(
        std::shared_ptr<DynamicObjectReader::Impl> impl);

    DynamicObjectReader& operator=(const DynamicObjectReader& rhs) {
        if (this != &rhs) {
            impl_ = rhs.impl_;
        }
        return *this;
    }

    virtual ~DynamicObjectReader() = default;
    DynamicValue GetType() const;
    inline bool IsValid() const { return impl_ != nullptr; }

    template <typename T>
    senseAD::base::optional<T> as() const {
        return {};
    }

 public:
    std::shared_ptr<Impl> impl_;
};

class DynamicListReader {
 public:
    class Impl {
     public:
        virtual ~Impl() = default;

        virtual size_t Size() const = 0;
        virtual DynamicObjectReader at(size_t index) const = 0;
        virtual std::shared_ptr<DynamicObjectReader::Impl> ToObjectReaderImpl()
            const = 0;
    };

    class Iterator {
     public:
        Iterator(DynamicListReader::Impl* ptr, int index)
            : ptr_(ptr), index_(index) {}
        inline Iterator operator++() {
            index_++;
            return *this;
        }
        inline bool operator!=(const Iterator& other) {
            return other.index_ != index_;
        }
        DynamicObjectReader operator*() const;

     private:
        DynamicListReader::Impl* ptr_;
        int index_;
    };

 public:
    explicit DynamicListReader(std::shared_ptr<DynamicListReader::Impl> impl);
    virtual ~DynamicListReader() = default;

    inline Iterator begin() const { return Iterator(impl_.get(), 0); }
    inline Iterator end() const { return Iterator(impl_.get(), impl_->Size()); }
    inline size_t size() const { return impl_->Size(); }
    DynamicObjectReader at(size_t index) const;

 private:
    std::shared_ptr<DynamicListReader::Impl> impl_;

    friend class DynamicObjectReader;
};

class DynamicEnumReader {
 public:
    class Impl {
     public:
        virtual ~Impl() = default;
        virtual std::string GetName() const = 0;
        virtual int GetRaw() const = 0;
    };

 public:
    explicit DynamicEnumReader(std::shared_ptr<DynamicEnumReader::Impl> impl);
    virtual ~DynamicEnumReader() = default;

    std::string GetName() const;
    int GetRaw() const;
    std::shared_ptr<DynamicEnumReader::Impl> impl_;
};

class DynamicStructReader {
 public:
    struct Field {
        std::string field_name;
        DynamicObjectReader reader;
    };
    class Impl {
     public:
        virtual ~Impl() = default;
        virtual std::vector<Field> GetFields() const = 0;
        virtual std::shared_ptr<DynamicObjectReader::Impl> ToObjectReaderImpl()
            const = 0;
    };

 public:
    DynamicStructReader() = default;
    explicit DynamicStructReader(
        std::shared_ptr<DynamicStructReader::Impl> impl);
    virtual ~DynamicStructReader() = default;
    std::vector<Field> GetFields() const;

 private:
    std::shared_ptr<DynamicStructReader::Impl> impl_;

    friend class DynamicObjectReader;
};

#define DEFINE_AS_FUNC(T) \
    template <>           \
    senseAD::base::optional<T> DynamicObjectReader::as<T>() const;

DEFINE_AS_FUNC(bool)
DEFINE_AS_FUNC(uint8_t)
DEFINE_AS_FUNC(uint16_t)
DEFINE_AS_FUNC(uint32_t)
DEFINE_AS_FUNC(uint64_t)
DEFINE_AS_FUNC(int8_t)
DEFINE_AS_FUNC(int16_t)
DEFINE_AS_FUNC(int32_t)
DEFINE_AS_FUNC(int64_t)
DEFINE_AS_FUNC(float)
DEFINE_AS_FUNC(double)
DEFINE_AS_FUNC(std::string)
DEFINE_AS_FUNC(std::vector<char>)
DEFINE_AS_FUNC(DynamicListReader)
DEFINE_AS_FUNC(DynamicEnumReader)
DEFINE_AS_FUNC(DynamicStructReader)

#undef DEFINE_AS_FUNC

class DynamicMessageReader {
 public:
    class Impl {
     public:
        virtual ~Impl() noexcept(false) {}
        virtual senseAD::base::optional<DynamicStructReader> ParseMessage(
            const char* msg_ptr, const size_t msg_size) const = 0;
    };

 public:
    DynamicMessageReader(const std::string& msg_type_name,
                         const std::string& schema_str);

    template <typename T>
    static std::unique_ptr<DynamicMessageReader> New() {
        return std::make_unique<DynamicMessageReader>(
            MessageTypeMeta<T>::GetMessageTypename(),
            MessageTypeMeta<T>::GetMessageDescriptor());
    }

    virtual ~DynamicMessageReader() = default;

    senseAD::base::optional<DynamicStructReader> ParseMessage(
        const char* msg_ptr, const size_t msg_size) const;

    bool VerifyData(const char* msg_ptr, const size_t msg_size) const;

 private:
    std::shared_ptr<Impl> impl_;
};

}  // namespace serde
}  // namespace senseAD
