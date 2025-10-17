/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file schema_serialize.h
 * @brief Provide function to convert schema to binary buffer
 */
#pragma once

#include <cstdio>
#include <vector>
#include <memory>

#include "ad_bag/schema/schema.h"

namespace senseAD {
namespace bag {

bool SerializeSchema(const BagHeaderV3& data, FILE* dst);

bool SerializeSchema(const ChunkHeader& data,
                     char* dst,
                     size_t* step = nullptr);

bool SerializeBufferData(const std::vector<char>& datas,
                         const size_t size,
                         FILE* dst);
bool SerializeChunkData(const char* datas,
                        const size_t size,
                        char* dst,
                        size_t* step = nullptr);

bool SerializeSchema(const MessageHeader& data,
                     char* dst,
                     size_t* step = nullptr);

bool SerializeSchema(const std::vector<ChunkInfoTableIndex>& datas, FILE* dst);

bool SerializeSchema(const ChannelInfoItem& data, char* dst);

bool SerializeSchema(const ChannelInfoItem& data, FILE* dst);
bool SerializeSchema(const ChannelInfoTableIndex& data, FILE* dst);

size_t ExpectedSerializedSize(const ExtraBagInfo& data);
bool SerializeSchema(const ExtraBagInfo& data, FILE* dst);

bool SerializeSchema(const std::vector<UserDefineItem>& datas, FILE* dst);

uint64_t CompressWithLz4(const size_t src_len,
                         const char* src,
                         std::vector<char>* dest);
uint64_t CompressWithZstd(const size_t src_len,
                          const char* src,
                          std::vector<char>* dest);
}  // namespace bag
}  // namespace senseAD
