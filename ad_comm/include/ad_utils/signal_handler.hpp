/*
 * Copyright (C) 2020 by SenseTime Group Limited. All rights reserved.
 * GUO Zhichong <guozhichong@sensetime.com>
 */

#pragma once

#include <functional>

namespace senseAD {
namespace ad_utils {

// Enable the signal handler in current process
void InstallFailureSignalHandlerIfNotDisabled();

void InstallFailureSignalHandler();

// Set up the custom handdler
void InstallExitSignalHandler(std::function<void(void)> exit_handler);

}  // namespace ad_utils
}  // namespace senseAD
