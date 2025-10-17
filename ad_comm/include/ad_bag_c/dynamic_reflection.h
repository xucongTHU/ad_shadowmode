/*
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum DynamicValue {
    DynamicValue_UNSUPPORTED,
    DynamicValue_VOID,
    DynamicValue_BOOL,
    DynamicValue_INT,
    DynamicValue_UINT,
    DynamicValue_FLOAT,
    DynamicValue_TEXT,
    DynamicValue_DATA,
    DynamicValue_LIST,
    DynamicValue_ENUM,
    DynamicValue_STRUCT
};

// Forward declarations of the structs
typedef enum DynamicValue DynamicValue;
typedef struct DynamicMessageReader DynamicMessageReader;
typedef struct DynamicStructReader DynamicStructReader;
typedef struct DynamicObjectReader DynamicObjectReader;

// Struct to hold a field in a dynamic struct
typedef struct DynamicStructField {
    char* field_name;             // Name of the field
    DynamicObjectReader* reader;  // Reader for the field's value
} DynamicStructField;

typedef struct {
    char unique_node_id[32];
    uint64_t stamp;
    uint64_t seq;
    char pub_node_name[32];
    bool is_enabled;
    uint8_t feature_flag;
    uint64_t header_flag;
    uint64_t version;
} RsclMsgHeader_C;

// Function to create a DynamicMessageReader
DynamicMessageReader* DynamicMessageReader_Create(const char* msg_type_name,
                                                  const char* schema_str,
                                                  size_t schema_len);

// Function to destroy a DynamicMessageReader
void DynamicMessageReader_Destroy(DynamicMessageReader* reader);

// Function to parse a message using a DynamicMessageReader
DynamicStructReader* DynamicMessageReader_ParseMessage(
    DynamicMessageReader* reader, const char* msg_ptr, size_t msg_size);

void DynamicStructReader_Destroy(DynamicStructReader* reader);

// Function to get the fields of a DynamicStructReader
size_t DynamicStructReader_GetFields(DynamicStructReader* reader,
                                     DynamicStructField*** fields);

// Function to verify data using a DynamicMessageReader
bool DynamicMessageReader_VerifyData(DynamicMessageReader* reader,
                                     const char* msg_ptr,
                                     size_t msg_size);

// Function to get a specific field from a DynamicStructReader
DynamicObjectReader* DynamicStructReader_GetField(DynamicStructReader* reader,
                                                  const char* field_name);

// Function to destroy a DynamicObjectReader
void DynamicObjectReader_Destroy(DynamicObjectReader* reader);

// Function to destroy a DynamicStructField
void DynamicStructField_Destroy(DynamicStructField* field);

// Function to destroy an array of DynamicStructField
void DynamicStructFieldArray_Destroy(DynamicStructField** fields, size_t count);

// Function to check if a DynamicObjectReader holds a boolean value
bool DynamicObjectReader_AsBool(DynamicObjectReader* reader, bool* value);

DynamicValue DynamicObjectReader_GetType(DynamicObjectReader* reader);

// Function to check if a DynamicObjectReader holds an unsigned integer value
bool DynamicObjectReader_AsUint(DynamicObjectReader* reader, uint64_t* value);

// Function to check if a DynamicObjectReader holds a signed integer value
bool DynamicObjectReader_AsInt(DynamicObjectReader* reader, int64_t* value);

// Function to check if a DynamicObjectReader holds a floating-point value
bool DynamicObjectReader_AsFloat(DynamicObjectReader* reader, double* value);

// Function to check if a DynamicObjectReader holds a string value
bool DynamicObjectReader_AsStr(DynamicObjectReader* reader, char** value);

// Function to check if a DynamicObjectReader holds a data value
bool DynamicObjectReader_AsData(DynamicObjectReader* reader,
                                char** value,
                                size_t* size);

// Function to check if a DynamicObjectReader holds a list value
bool DynamicObjectReader_AsList(DynamicObjectReader* reader,
                                DynamicObjectReader*** value,
                                size_t* size);

// Function to destroy an array of DynamicObjectReader
void DynamicObjectReaderArray_Destroy(DynamicObjectReader** readers,
                                      size_t count);

// Function to check if a DynamicObjectReader holds an enum value
bool DynamicObjectReader_AsEnum(DynamicObjectReader* reader, int* value);

// Function to check if a DynamicObjectReader holds a struct value
bool DynamicObjectReader_AsStruct(DynamicObjectReader* reader,
                                  DynamicStructReader** value);

bool ParseHeaderFromBufferTail_C(const char* source,
                                 const uint64_t len,
                                 RsclMsgHeader_C* header);

#ifdef __cplusplus
}
#endif
