/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>

#include "ad_mw/comm.h"
#include "ad_mw/visibility_control.h"
#include "ad_mw_cyber/message/borrowed_rawmessage.h"
#include "ad_mw_cyber/message/borrowed_rawmessage_traits.h"
#include "cyber/cyber.h"

namespace senseAD {
namespace mw {
namespace comm {

apollo::cyber::proto::RoleAttributes RoleAttributesFromConf(
    const PublisherConf& conf);

apollo::cyber::proto::RoleAttributes RoleAttributesFromConf(
    const SubscriberConf& conf);

inline std::shared_ptr<BorrowedRawMessage> From(
    const std::shared_ptr<apollo::cyber::message::RawMessage>& in) {
    return std::make_shared<BorrowedRawMessage>(in->message.data(),
                                                in->message.size(), in);
}
inline std::shared_ptr<apollo::cyber::message::BorrowedRawMessage> From(
    const std::shared_ptr<BorrowedRawMessage>& in) {
    return std::make_shared<apollo::cyber::message::BorrowedRawMessage>(
        in->data, in->size, in->ref);
}

}  // namespace comm
}  // namespace mw
}  // namespace senseAD
