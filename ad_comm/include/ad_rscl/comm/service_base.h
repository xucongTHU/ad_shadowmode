/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>

#include "ad_log/ad_log.hpp"
#include "ad_mw/ad_mw.h"
#include "ad_serde/ad_serde.h"
#include "ad_base/common/shutdownable.h"
#include "ad_rscl/comm/types.h"

namespace senseAD {
namespace rscl {
namespace comm {

/**
 * @brief Base type for service.
 * For the usecase that the user will not call member functions that have
 * message
 * type in its signature
 *
 */
class ServiceBase : public SwitchableNodeMemberBase, public ::Shutdownable {
 public:
    using Ptr = std::shared_ptr<ServiceBase>;
    virtual ~ServiceBase();

    void ShutdownImpl() override;

    /**
     * @brief Get the Service Name
     *
     * @return const std::string&
     */
    const std::string &GetServiceName() const;

    /**
     * @brief Get the Service Conf
     *
     * @return const ServiceConf&
     */
    const ServiceConf &GetServiceConf() const;

 protected:
    explicit ServiceBase(const ServiceConf &conf);

    ServiceConf conf_;

 private:
};

/**
 * @brief Service receive one request at a time and provide a response to that
 * request
 *
 * @tparam Request The data type of the request
 * @tparam Response The data type of the response
 */
template <typename Request, typename Response>
class Service : public ServiceBase {
 public:
    using Ptr = std::shared_ptr<Service<Request, Response>>;
    using ServiceCallback =
        std::function<void(const std::shared_ptr<ReceivedMsg<Request>> &,
                           std::shared_ptr<SendMsg<Response>> &)>;

 protected:
    explicit Service(const ServiceConf &conf) : ServiceBase(conf) {}
    virtual ~Service() = default;

 private:
};

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
