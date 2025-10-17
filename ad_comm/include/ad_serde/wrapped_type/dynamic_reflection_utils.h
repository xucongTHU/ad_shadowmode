/*
 * Copyright (C) 2024 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <string>
#include <vector>

#include "ad_base/std/optional.h"
#include "ad_serde/wrapped_type/dynamic_reflection.h"
#include "ad_serde/wrapped_type/base.h"

namespace senseAD {
namespace serde {

bool IsDataVerificationFailedException(const std::exception& e);

senseAD::base::optional<std::string> JsonStringByDynamicReflection(
    const std::string& msg_type_name,
    const std::string& schema_str,
    const char* msg_ptr,
    const size_t msg_size);

template <typename T>
senseAD::base::optional<std::string> JsonStringByDynamicReflection(
    const SendMsg<T>& msg) {
    auto bmsg = msg.BorrowRaw();
    return JsonStringByDynamicReflection(
        MessageTypeMeta<T>::GetMessageTypename(),
        MessageTypeMeta<T>::GetMessageDescriptor(), bmsg->data, bmsg->size);
}

template <typename T>
senseAD::base::optional<std::string> JsonStringByDynamicReflection(
    const ReceivedMsg<T>& msg) {
    return JsonStringByDynamicReflection(
        MessageTypeMeta<T>::GetMessageTypename(),
        MessageTypeMeta<T>::GetMessageDescriptor(), msg.Bytes(),
        msg.ByteSize());
}

senseAD::base::optional<std::string> JsonStringByDynamicReflection(
    const senseAD::serde::DynamicStructReader& msg_reader);
senseAD::base::optional<std::string> JsonStringByDynamicReflection(
    const senseAD::serde::DynamicObjectReader& object_reader);

bool VerifyData(const senseAD::serde::DynamicStructReader& msg_reader);

struct FieldPathPart {
    bool is_list = false;
    // If is_list = true, use this value
    int list_index = 0;
    // If is_list = false, use this value
    std::string struct_fieldname;
};
/**
 * @brief Get the Object Reader By Field Path
 *!IMPORTANT: Expect the final level field is a struct or list
 * @param msg_reader The root struct reader
 * @param field_path field path is the path
 * @return senseAD::base::optional<senseAD::serde::DynamicObjectReader>
 */
senseAD::base::optional<senseAD::serde::DynamicObjectReader>
GetObjectReaderByFieldPath(
    const senseAD::serde::DynamicStructReader& msg_reader,
    const std::vector<FieldPathPart>& field_path);

}  // namespace serde
}  // namespace senseAD
