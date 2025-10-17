/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 * @file timer_component.h
 * @brief Provide a time driven component wrapper class.
 * Their Process function will be call when expected time interval is meet
 */
#pragma once

#include <memory>

#include "ad_rscl/component/component.h"
#include "ad_rscl/task/task.h"

namespace senseAD {
namespace rscl {
namespace component {

using senseAD::rscl::task::TimerTask;

/**
 * @brief A time driven component wrapper class.
 *
 */
class TimerComponent : public ComponentBase {
 public:
    TimerComponent();
    ~TimerComponent() override;
    bool Initialize(const ComponentConfig& config) override;
    bool Initialize() override;
    void SetupCustomParam() override;
    void ShutdownImpl() override;
    bool Process();

 protected:
    /**
     * @brief User Process Function
     *
     * @return whether error exist
     */
    virtual bool OnProc() = 0;

    std::unique_ptr<TimerTask> timer_;
};
}  // namespace component
}  // namespace rscl
}  // namespace senseAD
