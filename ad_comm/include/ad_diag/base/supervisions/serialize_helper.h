/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <cmath>
#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/unordered_map.hpp>

namespace senseAD {
namespace diag {

template <typename T>
void Serialize(const T& msg, std::vector<char>* buffer) {
    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(msg);
    auto s = ss.str();

    buffer->resize(s.size());
    memcpy(&(*buffer)[0], s.c_str(), s.size());
}

template <typename T>
void Deserialize(const std::vector<char>& buffer, T* msg) {
    std::stringstream ss;
    ss.write(buffer.data(), buffer.size());
    cereal::BinaryInputArchive iarchive(ss);
    iarchive(*msg);
}

}  // namespace diag
}  // namespace senseAD
