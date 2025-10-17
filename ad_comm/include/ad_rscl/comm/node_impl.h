/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <map>
#include <utility>
#include <vector>

#include "ad_base/common/shutdownable.h"
#include "ad_mw/ad_mw.h"
#include "ad_mw/qos_default.h"
#include "ad_rscl/comm/client_factory.h"
#include "ad_rscl/exception.h"
#include "ad_rscl/comm/service_factory.h"
#include "ad_rscl/comm/subscriber_factory.h"
#include "ad_rscl/comm/publisher_factory.h"
#include "ad_rscl/comm/external_subscriber_to_internal_publisher_factory.h"
#include "ad_rscl/common/feature_manager.h"
#include "ad_rscl/task/task.h"
#include "ad_msg_bridge/external_msg_comm/service_register.h"
#include "ad_rscl/common/interface.h"
#if ENABLE_SOMEIP
#include "ad_rscl/comm/vsomeip_impl/someip_publisher.h"
#include "ad_rscl/comm/vsomeip_impl/someip_subscriber.h"
#include "ad_rscl/comm/vsomeip_impl/someip_client.h"
#include "ad_rscl/comm/vsomeip_impl/someip_service.h"
#include "ad_rscl/comm/vsomeip_impl/someip_wrapper.h"
#include "ad_rscl/comm/vsomeip_impl/common.h"
#endif

namespace senseAD {
namespace rscl {
namespace component {
class ComponentBase;
}

namespace service_discovery {
class ServiceDiscovery;
}
namespace comm {

class MsgTypeVerifierManager;

using senseAD::rscl::common::Suspendable;
#if ENABLE_SOMEIP
using senseAD::rscl::comm::someip::MessageCallback;
using senseAD::rscl::comm::someip::RequestHandler;
using senseAD::rscl::comm::someip::SomeIPClient;
using senseAD::rscl::comm::someip::SomeipConfig;
using senseAD::rscl::comm::someip::SomeipEventConfig;
using senseAD::rscl::comm::someip::SomeipMethodConfig;
using senseAD::rscl::comm::someip::SomeIPPublisher;
using senseAD::rscl::comm::someip::SomeIPService;
using senseAD::rscl::comm::someip::SomeIPSubscriber;
using senseAD::rscl::comm::someip::SomeipWrapper;
#endif

void EnsureFitQos(SwitchableNodeMemberBase *,
                  const senseAD::rscl::idl::PriorityLevel,
                  const senseAD::rscl::idl::PriorityLevel);

/**
 * @brief The actual implementation for node
 *
 */
class NodeImpl : public NodeOwnerBase,
                 public ::Shutdownable,
                 public Suspendable {
 public:
    /**
     * @brief Construct a new Node Impl object
     *
     * @param name node name
     * @param node_namespace node namespace
     */
    NodeImpl(const std::string &name, const std::string &node_namespace);

    /**
     * @brief Destroy the Node Impl object
     *
     */
    virtual ~NodeImpl();

    /**
     * @brief Shutdown the Node Impl object before destroying
     *
     */
    void ShutdownImpl() override;

    /**
     * @brief Get the Node Name
     *
     * @return const std::string&
     */
    const std::string &GetName() const override;

    /**
     * @brief Build the real configuration of publisher
     *
     * @param conf publisher
     */
    template <typename MessageT>
    PublisherConf BuildRealPubConf(const PublisherConf &conf,
                                   const std::string &topic);

    /**
     * @brief Create a Publisher object
     *
     * @tparam MessageT Data type of the message
     * @param topic Topic Name
     * @param conf Configuration Parameter
     * @return std::shared_ptr<Publisher<MessageT>>
     */
    template <typename MessageT>
    std::shared_ptr<Publisher<MessageT>> CreatePublisher(
        const std::string &topic, const PublisherConf &conf);

    template <typename RsclType, typename ExternalType>
    std::shared_ptr<PublisherGateway<RsclType, ExternalType>>
    CreatePublisherGateway(const std::string &topic, const PublisherConf &conf);

    template <typename MessageT>
    std::shared_ptr<Publisher<MessageT>> _CreatePublisher(
        const std::string &topic,
        std::shared_ptr<Publisher<MessageT>> publisher);

    template <typename RsclType, typename ExternalType>
    std::shared_ptr<PublisherGateway<RsclType, ExternalType>>
    _CreatePublisherGateway(
        const std::string &topic,
        std::shared_ptr<PublisherGateway<RsclType, ExternalType>> publisher);

    /**
     * @brief Create a Subscriber object
     *
     * @tparam MessageT MessageT Data type of the message
     * @param topic Topic Name
     * @param cb Callback to trigger when receive message in topic
     * @param conf  Configuration Parameter
     * @return std::shared_ptr<Subscriber<MessageT>>
     */
    template <typename MessageT>
    std::shared_ptr<Subscriber<MessageT>> CreateSubscriber(
        const std::string &topic,
        const CallbackFunc<MessageT> &cb,
        const SubscriberConf &conf);

    template <typename RsclType, typename ExternalType>
    std::shared_ptr<SubscriberGateway<RsclType, ExternalType>>
    CreateSubscriberGateway(
        const std::string &topic,
        const ExternalCallbackFunc<ExternalType> &external_cb,
        const CallbackFunc<RsclType> &cb,
        const SubscriberConf &conf);

    /**
     * @brief Create an ExternalSubscriberToInternalPublisher object
     *
     * @tparam MessageT MessageT Data type of the message
     * @param external_topic External subscriber topic name
     * @param internal_topic Internal publisher topic name
     * @param cb Callback to trigger when receive message in topic
     * @param sub_conf External subscriber configuration parameter
     * @param pub_conf Internal publisher configuration parameter
     * @return std::shared_ptr<ExternalSubscriberToInternalPublisher<MessageT>>
     */
    template <typename MessageT>
    std::shared_ptr<ExternalSubscriberToInternalPublisher<MessageT>>
    CreateExternalSubscriberToInternalPublisher(
        const std::string &external_topic,
        const std::string &internal_topic,
        const CallbackFunc<MessageT> &cb,
        const SubscriberConf &sub_conf,
        const PublisherConf &pub_conf);

    // Used by internal_only
    template <typename MessageT>
    std::shared_ptr<Subscriber<MessageT>> _CreateSubscriber(
        const std::string &topic,
        std::shared_ptr<Subscriber<MessageT>> subscriber);
    template <typename RsclType, typename ExternalType>
    std::shared_ptr<SubscriberGateway<RsclType, ExternalType>>
    _CreateSubscriberGateway(
        const std::string &topic,
        std::shared_ptr<SubscriberGateway<RsclType, ExternalType>> subscriber);

    /**
     * @brief Create a Service object
     *
     * @tparam Request Data Type of the request
     * @tparam Response Data Type of the Response
     * @param service_name Name of Service
     * @param cb Function to process the request and output the response
     * @param conf Configuration Parameter
     * @return std::shared_ptr<Service<Request, Response>>
     */
    template <typename Request, typename Response>
    std::shared_ptr<Service<Request, Response>> CreateService(
        const std::string &service_name,
        const typename Service<Request, Response>::ServiceCallback &cb,
        const ServiceConf &conf);

    /**
     * @brief Create a Client object
     *
     * @tparam Request Data Type of the request
     * @tparam Response Data Type of the Response
     * @param service_name Name of Service
     * @param conf Configuration Parameter
     * @return std::shared_ptr<Client<Request, Response>>
     */
    template <typename Request, typename Response>
    std::shared_ptr<Client<Request, Response>> CreateClient(
        const std::string &service_name, const ClientConf &conf);

    inline MwNodeHandle *InnerHandle() const override {
        return node_handle_.get();
    }

#if ENABLE_SOMEIP
    /**
     * @brief Create a SomeIPPublisher object
     * @tparam MessageT Data type of the message
     * @param topic Topic Name
     * @param conf Configuration Parameter
     * @return std::shared_ptr<SomeIPPublisher<MessageT>>
     */
    template <typename MessageT>
    std::shared_ptr<SomeIPPublisher<MessageT>> CreateSomeIPPublisher(
        const std::string &topic, const SomeipEventConfig &conf);

    /**
     * @brief Create a SomeIPSubscriber object
     * @tparam MessageT Data type of the message
     * @param topic Topic Name
     * @param cb Callback to trigger when receive message in topic
     * @param conf Configuration Parameter
     * @return std::shared_ptr<SomeIPSubscriber<MessageT>>
     */
    template <typename MessageT>
    std::shared_ptr<SomeIPSubscriber<MessageT>> CreateSomeIPSubscriber(
        const std::string &topic,
        const MessageCallback<MessageT> &cb,
        const SomeipEventConfig &conf);

    /**
     * @brief Create a SomeIPClient object
     * @tparam RequestT Data type of the request
     * @tparam ResponseT Data type of the response
     * @param service_name Name of Service
     * @param conf Configuration Parameter
     * @return std::shared_ptr<SomeIPClient<RequestT, ResponseT>>
     */
    template <typename RequestT, typename ResponseT>
    std::shared_ptr<SomeIPClient<RequestT, ResponseT>> CreateSomeIPClient(
        const std::string &service_name, const SomeipMethodConfig &conf);

    /**
     * @brief Create a SomeIPService object
     * @tparam RequestT Data type of the request
     * @tparam ResponseT Data type of the response
     * @param service_name Name of Service
     * @param handler Function to process the request and output the response
     * @param conf Configuration Parameter
     * @return std::shared_ptr<SomeIPService<RequestT, ResponseT>>
     */
    template <typename RequestT, typename ResponseT>
    std::shared_ptr<SomeIPService<RequestT, ResponseT>> CreateSomeIPService(
        const std::string &service_name,
        const RequestHandler<RequestT, ResponseT> &handler,
        const SomeipMethodConfig &conf);
#endif

    /*******************************************
     *
     *             Helping Function
     *
     ********************************************/

    /**
     * @brief Get the Created Subscriber objects of same topic
     *
     * @tparam MessageT
     * @param topic
     * @return std::vector<shared_ptr<Subscriber<MessageT>>>
     */
    template <typename MessageT>
    std::vector<std::shared_ptr<Subscriber<MessageT>>> GetSubscribers(
        const std::string &topic) const;

    /**
     * @brief Get all Created Subscriber objects
     *
     * @param topic
     * @return std::vector<std::weak_ptr<SubscriberBase>>
     */
    std::vector<std::weak_ptr<SubscriberBase>> GetAllSubscribers() const;

    /**
     * @brief change default_conf_ default_disable_callback
     */
    void SetConfDisableCallback(const bool &enable);
    /**
     * @brief set conf.default_disable_callback
     * if conf.default_disable_callback not set
     */
    SubscriberConf ToDefaultConf(const SubscriberConf &in);
    /**
     * @brief set real_cb_callable_
     */
    void ToggleSwitch(const bool enable);
    inline senseAD::service_discovery::ServiceDiscovery *GetServiceDiscovery()
        const {
        return service_discovery_.get();
    }

    std::string GetTopicNameWithSuffix(const std::string &topic,
                                       const PublisherConf &conf) const;
    bool CheckTopic(const std::string &topic, const PublisherConf &conf);
    void SendNodeStatus();
    /*******************************************
     *
     *             Enable/Disable sub comm
     *
     ********************************************/

    /**
     * @brief Adjust the allowed priority level.
     * For all sub-comm that have level lower that it, will be disabled
     *
     * @param p
     */
    void AdjustAllowedPriorityLevel(const senseAD::rscl::idl::PriorityLevel p);
    senseAD::rscl::idl::PriorityLevel GetPriorityLevel() const;

    /**
     * @brief clear sub/pub map to prevent weak_ptr destruct coredump
     */
    void ClearEntities();

    /*******************************************
     *
     *             Suspendable Support
     *
     ********************************************/
    bool Suspend() final;
    bool Resume() final;

 private:
    /**
     * @brief  Aggregate the TransmitEval of subs with the same topic.
     */
    std::map<std::string, TransmitEval> AggregateSubsTransmitEval() const;

    void SetupPublisherConf(const std::string &input_topic,
                            const PublisherConf &input_conf,
                            std::string &output_topic,    // NOLINT
                            PublisherConf &output_conf);  // NOLINT

    void SetupSubscriberConf(const std::string &input_topic,
                             const SubscriberConf &input_conf,
                             std::string &output_topic,     // NOLINT
                             SubscriberConf &output_conf);  // NOLINT

 private:
    std::string name_;
    std::string namespace_;

    std::unique_ptr<MwNodeHandle> node_handle_;
    std::unique_ptr<rscl::task::TimerTask> status_timer_;
    std::shared_ptr<MsgTypeVerifierManager> msg_verifier_manager_;

    uint32_t timer_trigger_cnt_{0};

    mutable std::mutex subs_mtx_;
    mutable std::mutex pubs_mtx_;

    std::unordered_multimap<std::string, std::weak_ptr<SubscriberBase>> subs_;
    std::unordered_multimap<std::string, std::weak_ptr<PublisherBase>> pubs_;

    PublisherBase::Ptr state_pub_;
    uint64_t pid_;

    std::shared_ptr<senseAD::service_discovery::ServiceDiscovery>
        service_discovery_;
    std::shared_ptr<senseAD::service_discovery::SDServiceClient>
        sd_client_sptr_ =
            senseAD::service_discovery::SDServiceClient::SharedInstance();

    senseAD::rscl::idl::PriorityLevel priority_level_;
    std::vector<std::pair<senseAD::rscl::idl::PriorityLevel,
                          std::weak_ptr<SwitchableNodeMemberBase>>>
        sub_comms_;
    SubscriberConf default_conf_;

    std::mutex suspend_mtx_;
    bool suspended_{false};
    std::vector<std::weak_ptr<Suspendable>> suspended_member_;
#if ENABLE_SOMEIP
    std::shared_ptr<someip::SomeipWrapper> someip_app_;
#endif
};

// This function is used to set msg_type and msg_descriptor in PublisherConf
// or MsgMeta.
// If conf has msg_type or msg_descriptor, it will not be set in real_conf
// else it will be set default msg meta fomr MessageT in real_conf.
template <typename MessageT>
static void SetMsgMeta(PublisherConf *real_conf) {
    auto meta = senseAD::serde::MsgMeta::From<MessageT>();
    if (!real_conf->has_msg_type() || real_conf->msg_type() == "") {
        real_conf->set_msg_type(meta.msg_type);
    }

    if (!real_conf->has_msg_descriptor() || real_conf->msg_descriptor() == "") {
        real_conf->set_msg_descriptor(meta.msg_descriptor);
    }
}

template <typename MessageT>
PublisherConf NodeImpl::BuildRealPubConf(const PublisherConf &conf,
                                         const std::string &topic) {
    PublisherConf real_conf;
    real_conf.CopyFrom(conf);

    // Try to get topic name with suffix.
    std::string real_topic_name = GetTopicNameWithSuffix(topic, conf);
    real_conf.set_channel_name(real_topic_name);

    SetMsgMeta<MessageT>(&real_conf);

    return real_conf;
}

template <typename MessageT>
std::shared_ptr<Publisher<MessageT>> NodeImpl::CreatePublisher(
    const std::string &topic, const PublisherConf &conf) {
    // Check if the topic name is valid.
    CheckTopic(topic, conf);

    auto real_conf = BuildRealPubConf<MessageT>(conf, topic);

    auto ptr = PublisherFactory::Create<MessageT>(this, real_conf);

    return _CreatePublisher(real_conf.channel_name(), ptr);
}

template <typename RsclType, typename ExternalType>
std::shared_ptr<PublisherGateway<RsclType, ExternalType>>
NodeImpl::CreatePublisherGateway(const std::string &topic,
                                 const PublisherConf &conf) {
    // Check if the topic name is valid.
    CheckTopic(topic, conf);

    auto real_conf = BuildRealPubConf<RsclType>(conf, topic);

    auto ptr =
        PublisherFactory::CreateGW<RsclType, ExternalType>(this, real_conf);

    return _CreatePublisherGateway(real_conf.channel_name(), ptr);
}

template <typename MessageT>
std::shared_ptr<Publisher<MessageT>> NodeImpl::_CreatePublisher(
    const std::string &topic, std::shared_ptr<Publisher<MessageT>> publisher) {
    const auto &conf = publisher->GetPublisherConf();

    {
        std::lock_guard<std::mutex> lock(pubs_mtx_);
        pubs_.emplace(topic, publisher);
    }
    {
        std::lock_guard<std::mutex> lock(subs_mtx_);
        sub_comms_.emplace_back(conf.qos_profile().priority_level(), publisher);
    }
    EnsureFitQos(publisher.get(), conf.qos_profile().priority_level(),
                 priority_level_);
    return publisher;
}

template <typename RsclType, typename ExternalType>
std::shared_ptr<PublisherGateway<RsclType, ExternalType>>
NodeImpl::_CreatePublisherGateway(
    const std::string &topic,
    std::shared_ptr<PublisherGateway<RsclType, ExternalType>> publisher) {
    const auto &conf = publisher->GetPublisherConf();

    {
        std::lock_guard<std::mutex> lock(pubs_mtx_);
        pubs_.emplace(topic, publisher);
    }
    {
        std::lock_guard<std::mutex> lock(subs_mtx_);
        sub_comms_.emplace_back(conf.qos_profile().priority_level(), publisher);
    }
    EnsureFitQos(publisher.get(), conf.qos_profile().priority_level(),
                 priority_level_);
    return publisher;
}

template <typename MessageT>
std::shared_ptr<Subscriber<MessageT>> NodeImpl::CreateSubscriber(
    const std::string &topic,
    const CallbackFunc<MessageT> &cb,
    const SubscriberConf &conf) {
    std::string real_topic;
    SubscriberConf real_conf;
    SetupSubscriberConf(topic, conf, real_topic, real_conf);

    auto ptr = SubscriberFactory::Create<MessageT>(this, cb, real_conf);

    if (!ptr) {
        return nullptr;
    }

    return _CreateSubscriber(real_topic, ptr);
}

// Used by internal_only
template <typename MessageT>
std::shared_ptr<Subscriber<MessageT>> NodeImpl::_CreateSubscriber(
    const std::string &topic,
    std::shared_ptr<Subscriber<MessageT>> subscriber) {
    const auto &conf = subscriber->GetSubscriberConf();

    {
        std::lock_guard<std::mutex> lock(subs_mtx_);
        // Don't care repeated push
        subs_.emplace(topic, subscriber);
        sub_comms_.emplace_back(conf.qos_profile().priority_level(),
                                subscriber);
    }
    EnsureFitQos(subscriber.get(), conf.qos_profile().priority_level(),
                 priority_level_);
    return subscriber;
}

template <typename RsclType, typename ExternalType>
std::shared_ptr<SubscriberGateway<RsclType, ExternalType>>
NodeImpl::CreateSubscriberGateway(
    const std::string &topic,
    const ExternalCallbackFunc<ExternalType> &external_cb,
    const CallbackFunc<RsclType> &cb,
    const SubscriberConf &conf) {
    SubscriberConf real_conf;
    real_conf.CopyFrom(conf);
    real_conf.set_channel_name(topic);
    if (real_conf.qos_profile().depth() <
        senseAD::mw::comm::qos::kDefaultQueueSize) {
        real_conf.mutable_qos_profile()->set_depth(
            senseAD::mw::comm::qos::kDefaultQueueSize);
    }
    auto ptr = SubscriberFactory::CreateGW<RsclType, ExternalType>(
        this, external_cb, cb, real_conf);

    if (!ptr) {
        return nullptr;
    }

    return _CreateSubscriberGateway(topic, ptr);
}

// Used by internal_only
template <typename RsclType, typename ExternalType>
std::shared_ptr<SubscriberGateway<RsclType, ExternalType>>
NodeImpl::_CreateSubscriberGateway(
    const std::string &topic,
    std::shared_ptr<SubscriberGateway<RsclType, ExternalType>> subscriber) {
    const auto &conf = subscriber->GetSubscriberConf();

    {
        std::lock_guard<std::mutex> lock(subs_mtx_);
        // Don't care repeated push
        subs_.emplace(topic, subscriber);
        sub_comms_.emplace_back(conf.qos_profile().priority_level(),
                                subscriber);
    }
    EnsureFitQos(subscriber.get(), conf.qos_profile().priority_level(),
                 priority_level_);
    return subscriber;
}

template <typename MessageT>
std::shared_ptr<ExternalSubscriberToInternalPublisher<MessageT>>
NodeImpl::CreateExternalSubscriberToInternalPublisher(
    const std::string &external_topic,
    const std::string &internal_topic,
    const CallbackFunc<MessageT> &cb,
    const SubscriberConf &sub_conf,
    const PublisherConf &pub_conf) {
    CheckTopic(internal_topic, pub_conf);

    std::string real_external_topic;
    SubscriberConf real_sub_conf;
    SetupSubscriberConf(external_topic, sub_conf, real_external_topic,
                        real_sub_conf);

    std::string real_internal_topic;
    PublisherConf real_pub_conf;
    SetupPublisherConf(internal_topic, pub_conf, real_internal_topic,
                       real_pub_conf);

    SetMsgMeta<MessageT>(&real_pub_conf);

    AD_LERROR(NodeImpl)
        << "ExternalSubscriberToInternalPublisher real_external_topic: "
        << real_external_topic;
    AD_LERROR(NodeImpl)
        << "ExternalSubscriberToInternalPublisher real_internal_topic: "
        << real_internal_topic;

    auto ext_sub_int_pub =
        ExternalSubscriberToInternalPublisherFactory::Create<MessageT>(
            this, cb, real_sub_conf, real_pub_conf);

    if (ext_sub_int_pub == nullptr) {
        AD_LERROR(NodeImpl)
            << "Failed to create ExternalSubscriberToInternalPublisher for "
               "external topic: ["
            << external_topic << "], internal topic: [" << internal_topic
            << "]";
        return nullptr;
    }

    _CreateSubscriber<MessageT>(real_external_topic,
                                ext_sub_int_pub->GetExternalSubscriber());
    _CreatePublisher<MessageT>(real_internal_topic,
                               ext_sub_int_pub->GetInternalPublisher());

    return ext_sub_int_pub;
}

template <typename Request, typename Response>
std::shared_ptr<Service<Request, Response>> NodeImpl::CreateService(
    const std::string &service_name,
    const typename Service<Request, Response>::ServiceCallback &cb,
    const ServiceConf &conf) {
    ServiceConf real_conf;
    real_conf.CopyFrom(conf);
    real_conf.set_service_name(service_name);
    auto out = ServiceFactory::Create<Request, Response>(this, cb, real_conf);

    {
        std::lock_guard<std::mutex> lock(subs_mtx_);
        sub_comms_.emplace_back(real_conf.priority_level(), out);
    }
    EnsureFitQos(out.get(), real_conf.priority_level(), priority_level_);
    return out;
}

template <typename Request, typename Response>
std::shared_ptr<Client<Request, Response>> NodeImpl::CreateClient(
    const std::string &service_name, const ClientConf &conf) {
    ClientConf real_conf;
    real_conf.CopyFrom(conf);
    real_conf.set_service_name(service_name);
    auto out = ClientFactory::Create<Request, Response>(this, real_conf);

    {
        std::lock_guard<std::mutex> lock(subs_mtx_);
        sub_comms_.emplace_back(real_conf.priority_level(), out);
    }
    EnsureFitQos(out.get(), real_conf.priority_level(), priority_level_);
    return out;
}

template <typename MessageT>
std::vector<std::shared_ptr<Subscriber<MessageT>>> NodeImpl::GetSubscribers(
    const std::string &topic) const {
    std::lock_guard<std::mutex> lock(subs_mtx_);

    std::vector<std::weak_ptr<SubscriberBase>> subscribers;
    auto same_topic_subs = subs_.equal_range(topic);
    for (auto it = same_topic_subs.first; it != same_topic_subs.second; ++it) {
        subscribers.push_back(it->second);
    }

    std::vector<std::shared_ptr<Subscriber<MessageT>>> ptrs;
    for (auto &sub : subscribers) {
        auto ptr = sub.lock();
        if (ptr) {
            ptrs.push_back(
                std::dynamic_pointer_cast<Subscriber<MessageT>>(ptr));
        }
    }

    return ptrs;
}
#if ENABLE_SOMEIP
template <typename MessageT>
std::shared_ptr<SomeIPPublisher<MessageT>> NodeImpl::CreateSomeIPPublisher(
    const std::string &topic, const SomeipEventConfig &conf) {
    if (someip_app_ == nullptr) {
        someip_app_ = std::make_shared<someip::SomeipWrapper>(name_);
    }
    return std::make_shared<SomeIPPublisher<MessageT>>(someip_app_, topic,
                                                       conf);
}

template <typename MessageT>
std::shared_ptr<SomeIPSubscriber<MessageT>> NodeImpl::CreateSomeIPSubscriber(
    const std::string &topic,
    const MessageCallback<MessageT> &cb,
    const SomeipEventConfig &conf) {
    if (someip_app_ == nullptr) {
        someip_app_ = std::make_shared<someip::SomeipWrapper>(name_);
    }
    return std::make_shared<SomeIPSubscriber<MessageT>>(someip_app_, topic, cb,
                                                        conf);
}

template <typename RequestT, typename ResponseT>
std::shared_ptr<someip::SomeIPClient<RequestT, ResponseT>>
NodeImpl::CreateSomeIPClient(const std::string &service_name,
                             const SomeipMethodConfig &conf) {
    if (someip_app_ == nullptr) {
        someip_app_ = std::make_shared<someip::SomeipWrapper>(name_);
    }
    return std::make_shared<someip::SomeIPClient<RequestT, ResponseT>>(
        someip_app_, service_name, conf);
}

template <typename RequestT, typename ResponseT>
std::shared_ptr<someip::SomeIPService<RequestT, ResponseT>>
NodeImpl::CreateSomeIPService(
    const std::string &service_name,
    const someip::RequestHandler<RequestT, ResponseT> &handler,
    const SomeipMethodConfig &conf) {
    if (someip_app_ == nullptr) {
        someip_app_ = std::make_shared<someip::SomeipWrapper>(name_);
    }
    return std::make_shared<someip::SomeIPService<RequestT, ResponseT>>(
        someip_app_, service_name, handler, conf);
}
#endif

}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
