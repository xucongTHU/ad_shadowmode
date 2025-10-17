/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <capnp/dynamic.h>
#include <capnp/schema-loader.h>
#include <capnp/serialize-packed.h>
#include <capnp/message.h>
#include <capnp/serialize.h>

#include <vector>
#include <string>
#include <memory>

#include "ad_serde/wrapped_type/dynamic_reflection.h"

namespace senseAD {
namespace serde {

class CapnpDynamicListReaderImpl : public DynamicListReader::Impl {
 public:
    explicit CapnpDynamicListReaderImpl(capnp::DynamicList::Reader reader);
    virtual ~CapnpDynamicListReaderImpl() = default;

    size_t Size() const override;
    DynamicObjectReader at(size_t index) const override;
    std::shared_ptr<DynamicObjectReader::Impl> ToObjectReaderImpl()
        const override;

 private:
    capnp::DynamicList::Reader reader_;
};

class CapnpDynamicEnumReaderImpl : public DynamicEnumReader::Impl {
 public:
    explicit CapnpDynamicEnumReaderImpl(capnp::DynamicEnum reader);
    virtual ~CapnpDynamicEnumReaderImpl() = default;
    std::string GetName() const override;
    int GetRaw() const override;

 private:
    capnp::DynamicEnum reader_;
};

class CapnpDynamicStructReaderImpl : public DynamicStructReader::Impl {
 public:
    explicit CapnpDynamicStructReaderImpl(capnp::DynamicStruct::Reader reader);
    CapnpDynamicStructReaderImpl(const char* msg_ptr,
                                 const size_t msg_size,
                                 capnp::StructSchema msg_schema);

    virtual ~CapnpDynamicStructReaderImpl() = default;

    std::vector<DynamicStructReader::Field> GetFields() const override;
    std::shared_ptr<DynamicObjectReader::Impl> ToObjectReaderImpl()
        const override;

 private:
    void InitFields();

    std::shared_ptr<::capnp::FlatArrayMessageReader> msg_reader_;
    kj::ArrayPtr<const capnp::word> raw_msg_kjarr_;

    capnp::DynamicStruct::Reader reader_;
    std::vector<DynamicStructReader::Field> fields_;
};

class CapnpDynamicObjectReaderImpl : public DynamicObjectReader::Impl {
 public:
    explicit CapnpDynamicObjectReaderImpl(capnp::DynamicValue::Reader reader);
    virtual ~CapnpDynamicObjectReaderImpl() noexcept(false) {}
    DynamicValue GetType() const override;

    bool as_bool() const override;
    uint64_t as_uint() const override;
    int64_t as_int() const override;
    double as_float() const override;
    std::string as_str() const override;
    std::vector<char> as_data() const override;
    DynamicListReader as_list() const override;
    DynamicEnumReader as_enum() const override;
    DynamicStructReader as_struct() const override;

 private:
    capnp::DynamicValue::Reader reader_;
};

class CapnpDynamicMessageReaderImpl : public DynamicMessageReader::Impl {
 public:
    CapnpDynamicMessageReaderImpl(const std::string& msg_type_name,
                                  const std::string& schema_str);
    virtual ~CapnpDynamicMessageReaderImpl() noexcept(false) {}

    senseAD::base::optional<DynamicStructReader> ParseMessage(
        const char* msg_ptr, const size_t msg_size) const;

 private:
    std::string msg_type_name_;
    std::string schema_str_;

    capnp::SchemaLoader schema_loader_;
    kj::ArrayPtr<const capnp::word> kjarr_;
    std::shared_ptr<::capnp::FlatArrayMessageReader> schema_reader_;
    capnp::StructSchema msg_schema_;
};

}  // namespace serde
}  // namespace senseAD
