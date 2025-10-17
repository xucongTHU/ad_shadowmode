/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>

struct InfoOptions {
    bool detail_mode = false;
};

int RsclbagInfo(const std::string &bag_path, const InfoOptions &options);
