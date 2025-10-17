/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <unordered_map>

#include "ad_service_discovery/service_discovery/types.h"

namespace senseAD {
namespace service_discovery {

enum class ResultCode {
    SUCCESS,    // Operation completed successfully
    FAILED,     // Operation failed
    DUPLICATE,  // Duplicate entry detected, but it is not an error
    CONFLICT    // Duplicate entry detected, and it is an error
};

enum class RequestCommonType {
    UpsertDiscoveryMetaRequest,
    GetDiscoveryMetaRequest,
    UpsertChannelMetaRequest,
    GetChannelMetaRequest,
    NotifyToSubscribeRequest,
    SetAdjustDiscoveryMetaRequest
};

enum class ResponseCommonType {
    UpsertDiscoveryMetaResponse,
    GetDiscoveryMetaResponse,
    UpsertChannelMetaResponse,
    GetChannelMetaResponse,
    NotifyToSubscribeResponse,
    SetAdjustDiscoveryMetaResponse
};

struct RequestWrapper {
    RequestCommonType type;
    std::vector<char> data;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static RequestWrapper FromRaw(const char* raw, size_t size);

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(type, data);
    }
};

struct ResponseWrapper {
    ResponseCommonType type;
    std::vector<char> data;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static ResponseWrapper FromRaw(const char* raw, size_t size);

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(type, data);
    }
};

struct UpsertDiscoveryMetaRequest {
    SrvDiscEvent meta;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static UpsertDiscoveryMetaRequest FromRaw(const char* raw, size_t size);

    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(meta);
    }
};

struct UpsertDiscoveryMetaResponse {
    ResultCode code;
    std::string msg;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static UpsertDiscoveryMetaResponse FromRaw(const char* raw, size_t size);
    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(code, msg);
    }
};

struct GetDiscoveryMetaRequest {
    SrvDiscEvent meta_filter;
    bool get_all = false;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static GetDiscoveryMetaRequest FromRaw(const char* raw, size_t size);
    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(meta_filter, get_all);
    }
};

struct GetDiscoveryMetaResponse {
    ResultCode code;
    std::string msg;
    std::vector<SrvDiscEvent> metas;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static GetDiscoveryMetaResponse FromRaw(const char* raw, size_t size);
    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(code, msg, metas);
    }
};

struct NotifyToSubscribeRequest {
    std::string service_name;
    SrvDiscEvent::Action action;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static NotifyToSubscribeRequest FromRaw(const char* raw, size_t size);
    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(service_name, action);
    }
};

struct NotifyToSubscribeResponse {
    ResultCode code;
    std::string msg;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static NotifyToSubscribeResponse FromRaw(const char* raw, size_t size);
    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(code, msg);
    }
};

struct SetAdjustDiscoveryMetaRequest {
    SrvDiscEvent meta;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static SetAdjustDiscoveryMetaRequest FromRaw(const char* raw, size_t size);
    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(meta);
    }
};

struct SetAdjustDiscoveryMetaResponse {
    ResultCode code;
    std::string msg;

    std::shared_ptr<std::vector<char>> ToRaw() const;
    static SetAdjustDiscoveryMetaResponse FromRaw(const char* raw, size_t size);
    template <class Archive>
    void serialize(Archive& archive) {  // NOLINT
        archive(code, msg);
    }
};

}  // namespace service_discovery
}  // namespace senseAD
