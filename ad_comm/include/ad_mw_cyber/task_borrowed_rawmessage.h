/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ad_mw_cyber/task.h"

namespace senseAD {
namespace mw {
namespace task {

using senseAD::mw::BorrowedRawMessage;

#define RAW_MESSAGE_SYNC_TASK_DEFINE(T0, T1, T2, T3)                        \
    template <>                                                             \
    MessageSyncedCallbackTask<T0, T1, T2, T3>::MessageSyncedCallbackTask(   \
        const std::string& task_name,                                       \
        const std::vector<senseAD::mw::comm::SubscriberBase*>& subscribers, \
        const CallbackFunc& cb);                                            \
    template <>                                                             \
    void MessageSyncedCallbackTask<T0, T1, T2, T3>::Start();                \
    template <>                                                             \
    void MessageSyncedCallbackTask<T0, T1, T2, T3>::Stop();

RAW_MESSAGE_SYNC_TASK_DEFINE(BorrowedRawMessage, NullType, NullType, NullType)
RAW_MESSAGE_SYNC_TASK_DEFINE(BorrowedRawMessage,
                             BorrowedRawMessage,
                             NullType,
                             NullType)
RAW_MESSAGE_SYNC_TASK_DEFINE(BorrowedRawMessage,
                             BorrowedRawMessage,
                             BorrowedRawMessage,
                             NullType)
RAW_MESSAGE_SYNC_TASK_DEFINE(BorrowedRawMessage,
                             BorrowedRawMessage,
                             BorrowedRawMessage,
                             BorrowedRawMessage)

#undef RAW_MESSAGE_SYNC_TASK_DEFINE

}  // namespace task
}  // namespace mw
}  // namespace senseAD
