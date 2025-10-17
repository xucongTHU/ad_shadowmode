/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <future>
#include <memory>
#include <string>

#include "ad_base/common/shutdownable.h"
#include "ad_log/ad_log.hpp"
#include "ad_mw/ad_mw.h"
#include "ad_serde/ad_serde.h"
#include "ad_rscl/comm/types.h"

namespace senseAD {
namespace rscl {
namespace comm {

template <typename Request, typename Response>
class Client;

class ClientBase : public SwitchableNodeMemberBase, public Shutdownable {
 public:
    using Ptr = std::shared_ptr<ClientBase>;

    virtual ~ClientBase();

    void ShutdownImpl() override;

    template <typename Request, typename Response>
    std::shared_ptr<ReceivedMsg<Response>> SendRequest(
        std::shared_ptr<SendMsg<Request>> request,
        const std::chrono::milliseconds timeout_ms =
            std::chrono::milliseconds(-1)) {
        return dynamic_cast<Client<Request, Response>*>(this)->SendRequest(
            request, timeout_ms);
    }

    template <typename Request, typename Response>
    std::shared_future<std::shared_ptr<ReceivedMsg<Response>>> AsyncSendRequest(
        std::shared_ptr<SendMsg<Request>> request) {
        return dynamic_cast<Client<Request, Response>*>(this)->AsyncSendRequest(
            request);
    }

    template <typename Request>
    static std::shared_ptr<SendMsg<Request>> NewReq() {
        return std::make_shared<SendMsg<Request>>();
    }

    virtual bool IsServiceReady() const = 0;
    virtual bool WaitForServiceReady(
        const std::chrono::milliseconds timeout_ms =
            std::chrono::milliseconds(-1)) const = 0;

    /**
     * @brief Get the Service Name
     *
     * @return const std::string&
     */
    const std::string& GetServiceName() const;

    /**
     * @brief Get the Client Conf
     *
     * @return const ClientConf&
     */
    const ClientConf& GetClientConf() const;

 protected:
    explicit ClientBase(const ClientConf& conf);

    ClientConf conf_;

 private:
};

/**
 * @brief Client send request to Service and get a Response
 *
 * @tparam Request The data type of the request
 * @tparam Response The data type of the response
 */
template <typename Request, typename Response>
class Client : public ClientBase {
 public:
    using Ptr = std::shared_ptr<Client<Request, Response>>;
    /**
     * @brief Request the Service
     *
     * @param request
     * @param timeout_ms request timeout, if timeout, response will be empty. -1
     * mean never timeout
     * @return std::shared_ptr<Response>
     */
    virtual std::shared_ptr<ReceivedMsg<Response>> SendRequest(
        std::shared_ptr<SendMsg<Request>> request,
        const std::chrono::milliseconds timeout_ms =
            std::chrono::milliseconds(-1)) = 0;

    /**
     * @brief Request the Service Asynchronously
     *
     * @param request
     * @return std::shared_future<std::shared_ptr<Response>>
     */
    virtual std::shared_future<std::shared_ptr<ReceivedMsg<Response>>>
    AsyncSendRequest(std::shared_ptr<SendMsg<Request>> request) = 0;

    static std::shared_ptr<SendMsg<Request>> NewReq() {
        return std::make_shared<SendMsg<Request>>();
    }

    /**
     * @brief Whether the service is online
     *
     * @return true
     * @return false
     */
    virtual bool IsServiceReady() const = 0;
    /**
     * @brief Wait until service is online or timeout.
     *
     * @param timeout_ms -1 mean never timeout
     *
     * @return whether the service is ready
     */
    virtual bool WaitForServiceReady(
        const std::chrono::milliseconds timeout_ms =
            std::chrono::milliseconds(-1)) const = 0;

 protected:
    explicit Client(const ClientConf& conf) : ClientBase(conf) {}

 private:
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
