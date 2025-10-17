//
// Created by xucong on 25-7-8.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef MQTTWRAPPER_H
#define MQTTWRAPPER_H

#include <string>
#include <memory>
#include <vector>
#include <mqtt/async_client.h>
#include <nlohmann/json.hpp>

namespace shadow {
namespace data_proto {

class MqttWrapper {
public:
    enum class ErrorCode {
        SUCCESS = 0,
        INIT_FAILED,
        CONNECTION_FAILED,
        SSL_FAILED,
        PUBLISH_FAILED,
        SUBSCRIBE_FAILED,
        INVALID_PARAMETER
    };

    MqttWrapper();
    ~MqttWrapper();

    ErrorCode Init(const std::string& serverURI,
                   const std::string& clientId,
                   const std::string& username,
                   const std::string& password,
                   int keepAliveInterval= 5);

    ErrorCode Init(const std::string& serverURI,
                   const std::string& clientId,
                   const std::string& username,
                   const std::string& password,
                   const std::string& caCertPath,
                   const std::string& clientCertPath,
                   const std::string& clientKeyPath,
                   int keepAliveInterval= 5);

    ErrorCode Connect(int timeoutMs = 5000); // 5s

    void Disconnect();

    ErrorCode Publish(const std::string& topic,
                      const std::string& payload,
                      int qos = 1,
                      bool retain = false);

    ErrorCode Subscribe(const std::string& topic,
                        int qos = 1);

    using MessageCallback = std::function<void(const std::string& topic, const std::string& payload)>;
    void SetMessageCallback(MessageCallback callback);

    MqttWrapper(const MqttWrapper&) = delete;
    MqttWrapper& operator=(const MqttWrapper&) = delete;

private:
    class MqttCallbackImpl : public virtual mqtt::callback {
      public:
        MqttCallbackImpl(MqttWrapper* parent);
        void message_arrived(mqtt::const_message_ptr msg) override;
        void connection_lost(const std::string& cause) override;
      private:
        MqttWrapper* parent_;
    };

    std::unique_ptr<mqtt::async_client> client_;
    std::unique_ptr<MqttCallbackImpl> callback_;
    mqtt::connect_options connOpts_;
    MessageCallback userCallback_;
    std::string serverURI_;
    std::string clientId_;
    bool isConnected_;
    bool autoReconnect_;
    std::mutex mutex_;
    std::vector<std::pair<std::string, int>> subscriptions_;
};

} // namespace data_proto
} // namespace shadow

#endif //MQTTWRAPPER_H
