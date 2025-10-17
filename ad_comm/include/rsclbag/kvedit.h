/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <string>

struct KvEditOptions {
    bool show_user_keys = false;
    std::string add_key_value;
    std::string remove_key;
};

int RsclbagKvEdit(const std::string &bag_path,
                  const KvEditOptions &user_define);
