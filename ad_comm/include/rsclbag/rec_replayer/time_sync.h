/**
 * Copyright (C) 2025 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */
#pragma once

#include <cstdint>

int64_t CalculateRemoteTimeOffset(uint64_t local_sent_time,
                                  uint64_t remote_received_time,
                                  uint64_t remote_sent_time,
                                  uint64_t local_received_time);
