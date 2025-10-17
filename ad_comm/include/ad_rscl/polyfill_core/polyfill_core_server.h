/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once

#include <memory>

#include "ad_mw/types.h"
#include "ad_mw/comm.h"
#include "ad_base/common/shutdownable.h"

#include "ad_service_discovery/polyfill_service_discovery/sd_service_server.h"

using senseAD::service_discovery::SDServiceServer;

namespace senseAD {
namespace rscl {
namespace core {

class PolyFillCoreServer : public ::Shutdownable {
 public:
    PolyFillCoreServer();
    virtual ~PolyFillCoreServer();

    void EnableServiceDiscoveryServer();
    void DisableServiceDiscoveryServer();
    std::weak_ptr<SDServiceServer> GetServiceDiscoveryServerPtr() const;

    virtual void InitAll();
    void ShutdownImpl() override;

 private:
    std::shared_ptr<SDServiceServer> service_discovery_server_;
};

}  // namespace core
}  // namespace rscl
}  // namespace senseAD
