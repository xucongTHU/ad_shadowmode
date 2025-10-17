/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * fengzhongjie <fengzhongjie@senseauto.com>
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BagWriter BagWriter;

typedef struct {
    uint8_t compress_mode;
    int32_t gmt_offset_sec;
} BagFlags;


typedef struct {
    size_t bagsplit_bytes;
    size_t bagsplit_msgcnt;
    uint32_t msg_per_chunk;
    BagFlags flags;
} BagWriterAttribute;

typedef struct {
    const char* channel_name;
    const char* write_channel_name;
    const char* type;
    const char* descriptor;
} ChannelInfo;

typedef struct {
    const char* platform;
    const char* vehicle;
} BagWriteExtraInfo;

// BagWriterAttribute functions
BagWriterAttribute* BagWriterAttribute_Create();
void BagWriterAttribute_Destroy(BagWriterAttribute* attr);
void BagWriterAttribute_SetBagsplitBytes(BagWriterAttribute* attr,
                                         size_t bagsplit_bytes);
void BagWriterAttribute_SetBagsplitMsgcnt(BagWriterAttribute* attr,
                                          size_t bagsplit_msgcnt);
void BagWriterAttribute_SetMsgPerChunk(BagWriterAttribute* attr,
                                       uint32_t msg_per_chunk);
void BagWriterAttribute_SetFlags(BagWriterAttribute* attr,
                                 const BagFlags flags);

// BagWriter functions
BagWriter* BagWriter_Create(const char* out_bag_basename,
                            const BagWriterAttribute* attr);
void BagWriter_Destroy(BagWriter* writer);

bool BagWriter_AddSerializedMessage(BagWriter* writer,
                                    uint64_t timestamp,
                                    const void* buf,
                                    size_t buf_len,
                                    const ChannelInfo* info);
void BagWriter_DroppedCountIncr(BagWriter* writer);
void BagWriter_Close(BagWriter* writer);
void BagWriter_SetSimTime(BagWriter* writer, bool is_sim_time);
bool BagWriter_MoveToNextBagSeqFile(BagWriter* writer);
bool BagWriter_InitExtraBagInfo(BagWriter* writer,
                                const BagWriteExtraInfo* data);

#ifdef __cplusplus
}
#endif
