/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * dumengrong <dumengrong@senseauto.com>
 */

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace rscl {
namespace comm {
namespace raw {

class SendRawData {
 public:
    SendRawData() = default;
    SendRawData(const SendRawData&) = delete;
    SendRawData& operator=(const SendRawData&) = delete;

    // it use to defined the size of data. must set before send
    size_t size = 0;
    char* data = nullptr;
};

class RecvRawData {
 public:
    RecvRawData() = default;
    RecvRawData(const RecvRawData&) = delete;
    RecvRawData& operator=(const RecvRawData&) = delete;
    ~RecvRawData();

    // it use to defined the size of data. must set before send
    size_t size = 0;
    char* data = nullptr;
};

using CallbackFunc = std::function<void(const RecvRawData&)>;
using NotifyCallback = std::function<void(void)>;

enum class CommunicationType : int {
    NONE = 0,
    SHM = 1,
    UDP = 2,
    UDP_PACKER = 3  // support pack and unpack several data into one UDP packet
};

class CustomConfig {
 public:
    CustomConfig() = default;
    // Print method for demonstration purposes
    std::string DebugString() const;

 public:
    CommunicationType comm_type() const;
    std::string name() const;
    std::string ip() const;
    uint16_t port() const;
    bool is_blocking() const;
    int buffer_size() const;
    int queue_size() const;
    int merged_packet_size() const;

    void set_comm_type(CommunicationType comm_type);
    void set_name(const std::string& name);
    void set_ip(const std::string& ip);
    void set_port(uint16_t port);
    void set_is_blocking(bool is_blocking);
    void set_buffer_size(int buffer_size);
    void set_queue_size(int queue_size);
    void set_merged_packet_size(int merged_packet_size);

 private:
    std::string name_;
    CommunicationType comm_type_ = CommunicationType::NONE;
    std::string ip_;  // IP address.
    uint16_t port_;   // Port number.

    // Comm options.
    bool is_blocking_ = false;
    int buffer_size_ = 1024 * 1024;  // Default buffer size is 1MB.
    int queue_size_ = 10;

    int merged_packet_size_ = 1;
};

class CommunicaterBase {
 public:
    explicit CommunicaterBase(const CustomConfig& config);
    const CustomConfig& GetCustomConfig() const;
    bool IsReady() const;

    CustomConfig config_;
    uint64_t total_size_ = 0;   // total size of the data
    uint64_t total_count_ = 0;  // total count of the data
    bool is_ready_ = false;
};

class RawPublisherBase : public CommunicaterBase {
 public:
    using Ptr = std::shared_ptr<RawPublisherBase>;
    explicit RawPublisherBase(const CustomConfig& config);
    virtual ~RawPublisherBase() = default;

    /**
     * Send raw data.
     *
     * @param data The raw data to be sent.
     * @return True if the data was successfully sent, false otherwise.
     */
    virtual bool Send(const SendRawData& data) = 0;

    /**
     * @brief Get the total size of data to be sent.
     *
     * @return The total size of data to be sent.
     */
    uint64_t GetTotalSentSize() const;

    /**
     * @brief Get the total count of data to be sent.
     *
     * @return The total count of data to be sent.
     */
    uint64_t GetTotalSentCount() const;
};

class RawSubscriberBase : public CommunicaterBase {
 public:
    using Ptr = std::shared_ptr<RawSubscriberBase>;
    explicit RawSubscriberBase(const CustomConfig& config);
    virtual ~RawSubscriberBase() = default;

    /**
     * @brief Set the notification callback function.
     * @param callback The callback function.
     * @return None.
     */
    virtual void SetNotificationCallback(const NotifyCallback&) = 0;

    /**
     * Receive raw data.
     *
     * @param data Pointer to store the received raw data.
     * @return True if data was successfully received, false otherwise.
     */
    virtual bool Receive(std::shared_ptr<RecvRawData>* data) = 0;

    /**
     * @brief Get the total size of data to be recieved.
     *
     * @return The total size of data to be recieved.
     */
    uint64_t GetTotalRecvSize() const;

    /**
     * @brief Get the total count of data to be recieved.
     *
     * @return The total count of data to be recieved.
     */
    uint64_t GetTotalRecvCount() const;
};

}  // namespace raw
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
