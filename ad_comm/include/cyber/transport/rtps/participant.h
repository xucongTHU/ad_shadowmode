/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef CYBER_TRANSPORT_RTPS_PARTICIPANT_H_
#define CYBER_TRANSPORT_RTPS_PARTICIPANT_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <string>

#include "fastdds/dds/domain/DomainParticipant.hpp"
#include "fastdds/dds/domain/DomainParticipantFactory.hpp"
#include "fastdds/dds/topic/Topic.hpp"

#include "cyber/transport/rtps/underlay_message_type.h"

namespace apollo {
namespace cyber {
namespace transport {

class Participant;
using ParticipantPtr = std::shared_ptr<Participant>;

using ddsStatusMask = eprosima::fastdds::dds::StatusMask;

class Participant {
 public:
  Participant(
      const std::string& name, int send_port, uint32_t max_message_size = 0,
      eprosima::fastdds::dds::DomainParticipantListener* listener = nullptr,
      const ddsStatusMask& mask = ddsStatusMask::all());
  virtual ~Participant();

  void Shutdown();

  eprosima::fastdds::dds::DomainParticipant* fastrtps_participant();
  bool is_shutdown() const { return shutdown_.load(); }

 private:
  void CreateFastRtpsParticipant(
      const std::string& name, int send_port,
      eprosima::fastdds::dds::DomainParticipantListener* listener,
      const ddsStatusMask& mask = ddsStatusMask::all());

  std::atomic<bool> shutdown_;
  std::string name_;
  int send_port_;
  uint32_t max_message_size_;
  eprosima::fastdds::dds::DomainParticipantListener* listener_;
  ddsStatusMask mask_;
  std::shared_ptr<eprosima::fastdds::dds::DomainParticipantFactory>
      dds_participant_shared_inst_;
  eprosima::fastdds::dds::DomainParticipant* fastrtps_participant_;
  eprosima::fastdds::dds::TypeSupport type_;
  std::mutex mutex_;
};

}  // namespace transport
}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_TRANSPORT_RTPS_PARTICIPANT_H_
