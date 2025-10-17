/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>

#include "ad_rscl/comm/service_base.h"
#include "ad_rscl/comm/internal_impl/internal_service.h"
#include "ad_rscl/comm/inet_impl/inet_service.h"
#include "ad_rscl/global_config.h"

namespace senseAD {
namespace rscl {
namespace comm {

/**
 * @brief Create an service instance according to the conf or global setting
 *
 */
class ServiceFactory {
 public:
    template <typename Request, typename Response>
    static std::shared_ptr<Service<Request, Response>> Create(
        NodeOwnerBase* const node_owner,
        const typename Service<Request, Response>::ServiceCallback& cb,
        const ServiceConf& conf);
};

template <typename Request, typename Response>
std::shared_ptr<Service<Request, Response>> ServiceFactory::Create(
    NodeOwnerBase* const node_owner,
    const typename Service<Request, Response>::ServiceCallback& cb,
    const ServiceConf& conf) {
    auto inet_service_settings =
        GlobalConfig::Instance()->RawIdlConf().inet_service();
    for (const auto& setting : inet_service_settings) {
        if (conf.service_name() == setting.service_name()) {
            return std::make_shared<InetService<Request, Response>>(
                setting.ip(), setting.port(), cb, conf);
        }
    }

    return std::make_shared<InternalService<Request, Response>>(node_owner, cb,
                                                                conf);
}
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
