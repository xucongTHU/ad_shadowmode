/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * fengzhongjie <fengzhongjie@senseauto.com>
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct ReadedMessage ReadedMessage;

struct ReadedMessage {
    uint64_t timestamp;
    char channel_name[256];
    char* message_buffer;
    size_t message_buffer_size;

    // Internal use only
    void* cpp_impl;
};

ReadedMessage* ReadedMessage_Create();
void ReadedMessage_Destroy(ReadedMessage* msg);

void Free(void* ptr);

#ifdef __cplusplus
}
#endif
