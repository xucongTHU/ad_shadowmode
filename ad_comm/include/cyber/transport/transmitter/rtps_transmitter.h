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

#ifndef CYBER_TRANSPORT_TRANSMITTER_RTPS_TRANSMITTER_H_
#define CYBER_TRANSPORT_TRANSMITTER_RTPS_TRANSMITTER_H_

#include <memory>
#include <string>

#include "fastdds/dds/domain/DomainParticipant.hpp"
#include "fastdds/dds/publisher/DataWriter.hpp"
#include "fastdds/dds/publisher/DataWriterListener.hpp"
#include "fastdds/dds/publisher/Publisher.hpp"
#include "fastdds/dds/subscriber/DataReader.hpp"
#include "fastdds/dds/subscriber/DataReaderListener.hpp"
#include "fastdds/dds/subscriber/Subscriber.hpp"
#include "fastdds/dds/topic/Topic.hpp"
#include "cyber/common/log.h"
#include "cyber/message/message_traits.h"
#include "cyber/transport/rtps/attributes_filler.h"
#include "cyber/transport/rtps/participant.h"
#include "cyber/transport/transmitter/transmitter.h"

namespace apollo {
namespace cyber {
namespace transport {

template <typename M>
class RtpsTransmitter : public Transmitter<M> {
 public:
  using MessagePtr = std::shared_ptr<M>;

  RtpsTransmitter(const RoleAttributes& attr,
                  const ParticipantPtr& participant);
  virtual ~RtpsTransmitter();

  void Enable() override;
  void Disable() override;

  bool Transmit(const MessagePtr& msg, const MessageInfo& msg_info) override;

  bool Matched(uint32_t* matched_count = nullptr);

 private:
  bool Transmit(const M& msg, const MessageInfo& msg_info);

  ParticipantPtr participant_;

  eprosima::fastdds::dds::Publisher* mp_publisher_;
  eprosima::fastdds::dds::DataWriter* mp_writer_;
  eprosima::fastdds::dds::Topic* mp_topic_pub_;

  class PubListener : public eprosima::fastdds::dds::DataWriterListener {
   public:
    PubListener() : n_matched_(0), first_connected_(false) {}

    ~PubListener() override {}

    void on_publication_matched(
        eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

  //  private:
    int n_matched_;
    bool first_connected_;

  } m_listener;
};

template <typename M>
RtpsTransmitter<M>::RtpsTransmitter(const RoleAttributes& attr,
                                    const ParticipantPtr& participant)
    : Transmitter<M>(attr),
      participant_(participant),
      mp_publisher_(nullptr),
      mp_writer_(nullptr),
      mp_topic_pub_(nullptr) {}

template <typename M>
RtpsTransmitter<M>::~RtpsTransmitter() {
  Disable();
}

template <typename M>
void RtpsTransmitter<M>::Enable() {
  if (this->enabled_) {
    return;
  }

  RETURN_IF_NULL(participant_);

  eprosima::fastdds::dds::DataWriterQos wqos;
  RETURN_IF(!AttributesFiller::FillInPubAttr(this->attr_.channel_name(),
                                             this->attr_.qos_profile(), &wqos));
  // CREATE THE PUBLISHER
  mp_publisher_ = participant_->fastrtps_participant()->create_publisher(
      eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT);
  RETURN_IF_NULL(mp_publisher_)

  // CREATE THE TOPIC
  auto find_topic_desc =
      participant_->fastrtps_participant()->lookup_topicdescription(
          this->attr_.channel_name());
  if (find_topic_desc != nullptr) {
    mp_topic_pub_ =
        dynamic_cast<eprosima::fastdds::dds::Topic*>(find_topic_desc);
  } else {
    mp_topic_pub_ = participant_->fastrtps_participant()->create_topic(
        this->attr_.channel_name(), "UnderlayMessage",
        eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
  }
  RETURN_IF_NULL(mp_topic_pub_)

  mp_writer_ = mp_publisher_->create_datawriter(mp_topic_pub_, wqos, &m_listener);
  RETURN_IF_NULL(mp_writer_)

  this->enabled_ = true;
}

template <typename M>
void RtpsTransmitter<M>::Disable() {
  if (this->enabled_) {
    if (mp_writer_ != nullptr) {
      mp_publisher_->delete_datawriter(mp_writer_);
      mp_writer_ = nullptr;
    }
    auto mp_participant = participant_->fastrtps_participant();
    if (mp_publisher_ != nullptr) {
      mp_participant->delete_publisher(mp_publisher_);
      mp_publisher_ = nullptr;
    }
    if (mp_topic_pub_ != nullptr) {
      mp_participant->delete_topic(mp_topic_pub_);
      mp_topic_pub_ = nullptr;
    }
    this->enabled_ = false;
  }
}

template <typename M>
bool RtpsTransmitter<M>::Transmit(const MessagePtr& msg,
                                  const MessageInfo& msg_info) {
  return Transmit(*msg, msg_info);
}

template <typename M>
bool RtpsTransmitter<M>::Transmit(const M& msg, const MessageInfo& msg_info) {
  if (!this->enabled_) {
    ADEBUG << "not enable.";
    return false;
  }

  UnderlayMessage m;
  RETURN_VAL_IF(!message::SerializeToString(msg, &m.data()), false);

  eprosima::fastrtps::rtps::WriteParams wparams;

  char* ptr =
      reinterpret_cast<char*>(&wparams.related_sample_identity().writer_guid());

  memcpy(ptr, msg_info.sender_id().data(), ID_SIZE);
  memcpy(ptr + ID_SIZE, msg_info.spare_id().data(), ID_SIZE);

  wparams.related_sample_identity().sequence_number().high =
      (int32_t)((msg_info.seq_num() & 0xFFFFFFFF00000000) >> 32);
  wparams.related_sample_identity().sequence_number().low =
      (int32_t)(msg_info.seq_num() & 0xFFFFFFFF);

  if (participant_->is_shutdown()) {
    return false;
  }

  return mp_writer_->write(reinterpret_cast<void*>(&m), wparams);
}

template <typename M>
bool RtpsTransmitter<M>::Matched(uint32_t* matched_count) {
  if (matched_count) {
    *matched_count = m_listener.n_matched_;
  }
  return m_listener.first_connected_;
}

template <typename M>
void RtpsTransmitter<M>::PubListener::on_publication_matched(
    eprosima::fastdds::dds::DataWriter*,
    const eprosima::fastdds::dds::PublicationMatchedStatus& info) {
  if (info.current_count_change == 1) {
    n_matched_ = info.total_count;
    first_connected_ = true;
  } else if (info.current_count_change == -1) {
    n_matched_ = info.total_count;
  }
}

}  // namespace transport
}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_TRANSPORT_TRANSMITTER_RTPS_TRANSMITTER_H_
