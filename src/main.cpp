//
// Created by xucong on 25-7-10.
// Copyright (c) 2025 T3CAIC. All rights reserved.
// Tsung Xu<xucong@t3caic.com>
//


#include "ShadowModeContext.h"
#include "common/logger/Logger.h"

using namespace shadow;
using namespace shadow::logger;

int main(int argc, char* argv[]) {
    // stoic::cm::init(argc, argv, "shadow_mode");
    // stoic::cm::NodeHandle nh("shadow_mode");

    // ShadowModeContext ctx(nh);
    // ctx.Start();

    // stoic::cm::spin();

#if 1
    auto rt = senseAD::rscl::GetCurRuntime();
    printf("Init Runtime\n");
    rt->Init(argc, argv);

    printf("Init Component\n");
    senseAD::rscl::component::ComponentConfig cfg;
    cfg.timer_conf.set_name("shadow_mode");
    cfg.timer_conf.set_interval_ms(10);
    ShadowModeContext ctx;
    ctx.Initialize(cfg);

    printf("Wait For Shutdown\n");
    while (rt->OK()) {
        rt->WaitForShutdown(std::chrono::seconds(1));
    }

    if (!rt->OK()) {
        printf("Shutdown\n");
        ctx.Shutdown();
        printf("Finish Shutdown");
    }
#endif

    return 0;
}