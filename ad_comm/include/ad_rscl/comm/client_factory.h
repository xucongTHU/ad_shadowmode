/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>

#include "ad_rscl/comm/client_base.h"
#include "ad_rscl/comm/internal_impl/internal_client.h"
#include "ad_rscl/comm/inet_impl/inet_client.h"
#include "ad_rscl/global_config.h"

namespace senseAD {
namespace rscl {
namespace comm {

/**
 * @brief Create an client instance according to the conf or global setting
 *
 */
class ClientFactory {
 public:
    template <typename Request, typename Response>
    static std::shared_ptr<Client<Request, Response>> Create(
        NodeOwnerBase* const node_owner, const ClientConf& conf);
};

template <typename Request, typename Response>
std::shared_ptr<Client<Request, Response>> ClientFactory::Create(
    NodeOwnerBase* const node_owner, const ClientConf& conf) {
    auto inet_service_settings =
        GlobalConfig::Instance()->RawIdlConf().inet_service();
    for (const auto& setting : inet_service_settings) {
        if (conf.service_name() == setting.service_name()) {
            return std::make_shared<InetClient<Request, Response>>(
                setting.ip(), setting.port(), conf);
        }
    }

    return std::make_shared<InternalClient<Request, Response>>(node_owner,
                                                               conf);
}
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
