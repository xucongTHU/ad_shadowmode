/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * wangxiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <memory>
#include <thread>
#include <vector>
#include <string>
#include <iostream>

#include "ad_base/common/socket.h"

namespace senseAD {
namespace transport {

using TCPClient = senseAD::base::common::TcpFrameClient;
using TCPServer = senseAD::base::common::TcpFrameServer;

using RequestType = std::shared_ptr<std::vector<char>>;
using ResponseType = std::shared_ptr<std::vector<char>>;
using PubSubType = std::shared_ptr<std::vector<char>>;

class Subscriber {
 public:
    using SubscriberCallback = std::function<void(const PubSubType&)>;
    Subscriber(const std::string& ip, uint16_t port, SubscriberCallback sub_cb);
    ~Subscriber() {
        Stop();
        if (sub_th_.joinable()) {
            sub_th_.join();
        }
    }

    bool Start();
    void Stop() { is_running_ = false; }

 private:
    bool is_running_ = false;
    std::unique_ptr<TCPClient> tcp_client_;
    std::thread sub_th_;
    SubscriberCallback sub_cb_;
};

class Publisher {
 public:
    Publisher(const std::string& ip, uint16_t port)
        : tcp_server_(std::make_unique<TCPServer>(ip, port)) {
        tcp_server_->TryBind();
    }
    ~Publisher() = default;
    bool Publish(const PubSubType& msg);

 private:
    std::unique_ptr<TCPServer> tcp_server_;
};

class Survey {
 public:
    using SurveyCallback = std::function<void(const PubSubType&)>;
    Survey(const std::string& ip, uint16_t port)
        : tcp_server_(std::make_unique<TCPServer>(ip, port)) {
        tcp_server_->TryBind();
        survey_th_ = std::thread([this]() {
            while (is_running_) {
                senseAD::base::common::FrameMsg frame;
                // 100ms timeout
                if (tcp_server_->Recv(&frame, std::chrono::milliseconds(100))) {
                    senseAD::transport::RequestType req =
                        std::make_shared<std::vector<char>>(frame.msg.begin(),
                                                            frame.msg.end());
                    if (survey_cb_) {
                        survey_cb_(req);
                    }
                }
            }
        });
    }
    ~Survey() {
        is_running_ = false;
        if (survey_th_.joinable()) {
            survey_th_.join();
        }
    }
    bool Distribute(const PubSubType& msg);

    void SetSurveyCallback(const SurveyCallback& cb) { survey_cb_ = cb; }

 private:
    bool is_running_ = true;
    std::unique_ptr<TCPServer> tcp_server_;
    std::thread survey_th_;
    SurveyCallback survey_cb_;
};

class Reporter {
 public:
    using ReporterCallback =
        std::function<void(const RequestType&, ResponseType&)>;
    Reporter(const std::string& ip, uint16_t port, ReporterCallback cb);
    ~Reporter() {
        Stop();
        if (report_th_.joinable()) {
            report_th_.join();
        }
    }

    bool Start();
    void Stop() { is_running_ = false; }

 private:
    bool is_running_ = false;
    std::unique_ptr<TCPClient> tcp_client_;
    std::thread report_th_;
    ReporterCallback report_cb_;
};

}  // namespace transport
}  // namespace senseAD
