/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

/**
 * @brief Contain all header files that need to be exports
 *
 */
#include "ad_rscl/comm/node.h"
#include "ad_rscl/comm/raw_impl/raw_impl.h"
#include "ad_service_discovery/service_discovery/service_discovery.h"
#include "ad_base/common/env.h"
#include "ad_rscl/common/errcode.h"
#include "ad_rscl/common/internal_topics.h"
#include "ad_rscl/component/component_utils.h"
#include "ad_rscl/component/synced_component.h"
#include "ad_rscl/component/timer_component.h"
#include "ad_rscl/runtime.h"
#include "ad_rscl/task/task.h"
#include "ad_rscl/time/time.h"

// Namespace alias
namespace senseAD {
namespace rscl {

using senseAD::rscl::comm::Node;
using senseAD::rscl::comm::PublisherBase;
using senseAD::rscl::comm::SubscriberBase;
template <typename T>
using Subscriber = senseAD::rscl::comm::Subscriber<T>;
template <typename T>
using Subscriber = senseAD::rscl::comm::Subscriber<T>;
using senseAD::rscl::comm::SubscriberConf;

template <typename T>
using Publisher = senseAD::rscl::comm::Publisher<T>;
template <typename T>
using Publisher = senseAD::rscl::comm::Publisher<T>;
using senseAD::rscl::comm::PublisherConf;

template <typename request, typename response>
using Service = senseAD::rscl::comm::Service<request, response>;

template <typename request, typename response>
using Client = senseAD::rscl::comm::Client<request, response>;

using senseAD::rscl::time::Time;
}  // namespace rscl
}  // namespace senseAD

namespace rscl = senseAD::rscl;
