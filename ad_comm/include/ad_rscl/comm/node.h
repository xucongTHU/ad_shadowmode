/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ad_base/common/shutdownable.h"
#include "ad_rscl/comm/node_impl.h"

namespace senseAD {
namespace rscl {

namespace runtime {
class NodeManagement;
}

namespace comm {

class Node : public NodeOwnerBase, public ::Shutdownable {
 public:
    static const SubscriberConf DEFAULT_SUBCONF;
    static const SubscriberConf DEFAULT_INTERNAL_SUBCONF;
    static const PublisherConf DEFAULT_PUBCONF;

    virtual ~Node();

    /**
     * @brief Shutdown Node before destroying
     */
    void ShutdownImpl() override;

    /**
     * @brief Get the Node Name
     *
     * @return const std::string&
     */
    const std::string &GetName() const override;

    /**
     * @brief Create a Publisher object
     *
     * @tparam MessageT Data type of the message (Only Support Protobuf Right
     * Now)
     * @param topic Topic Name
     * @param conf Configuration Parameter
     * @return std::shared_ptr<Publisher<MessageT>>
     */
    template <typename MessageT>
    std::shared_ptr<Publisher<MessageT>> CreatePublisher(
        const std::string &topic, const PublisherConf &conf = DEFAULT_PUBCONF) {
        return impl_->CreatePublisher<MessageT>(topic, conf);
    }
    template <typename RsclType, typename ExternalType>
    std::shared_ptr<PublisherGateway<RsclType, ExternalType>>
    CreatePublisherGateway(const std::string &topic,
                           const PublisherConf &conf = DEFAULT_PUBCONF) {
        return impl_->CreatePublisherGateway<RsclType, ExternalType>(topic,
                                                                     conf);
    }

    // Used by internal_only
    template <typename MessageT>
    std::shared_ptr<Publisher<MessageT>> _CreatePublisher(
        const std::string &topic,
        std::shared_ptr<Publisher<MessageT>> publisher) {
        return impl_->_CreatePublisher<MessageT>(topic, publisher);
    }
    template <typename RsclType, typename ExternalType>
    std::shared_ptr<PublisherGateway<RsclType, ExternalType>>
    _CreatePublisherGateway(
        const std::string &topic,
        std::shared_ptr<PublisherGateway<RsclType, ExternalType>> publisher) {
        return impl_->CreatePublisherGateway<RsclType, ExternalType>(topic,
                                                                     publisher);
    }

    /**
     * @brief Create a Subscriber object
     *
     * @tparam MessageT MessageT Data type of the message (Only Support Protobuf
     * Right Now)
     * @param topic Topic Name
     * @param cb Callback to trigger when receive message in topic
     * @param conf  Configuration Parameter
     * @return std::shared_ptr<Subscriber<MessageT>>
     */
    template <typename MessageT>
    std::shared_ptr<Subscriber<MessageT>> CreateSubscriber(
        const std::string &topic,
        const CallbackFunc<MessageT> &cb = nullptr,
        const SubscriberConf &conf = DEFAULT_SUBCONF) {
        return impl_->CreateSubscriber<MessageT>(topic, cb, conf);
    }

    template <typename MessageT, typename CallbackT>
    std::shared_ptr<Subscriber<MessageT>> CreateSubscriber(
        const std::string &topic,
        void (CallbackT::*fp)(const std::shared_ptr<ReceivedMsg<MessageT>> &),
        CallbackT *obj,
        const SubscriberConf &conf = DEFAULT_SUBCONF) {
        return impl_->CreateSubscriber<MessageT>(
            topic, std::bind(fp, obj, std::placeholders::_1), conf);
    }

    template <typename MessageT, typename CallbackT>
    std::shared_ptr<Subscriber<MessageT>> CreateSubscriber(
        const std::string &topic,
        void (CallbackT::*fp)(const std::shared_ptr<ReceivedMsg<MessageT>> &)
            const,
        CallbackT *obj,
        const SubscriberConf &conf = DEFAULT_SUBCONF) {
        return impl_->CreateSubscriber<MessageT>(
            topic, std::bind(fp, obj, std::placeholders::_1), conf);
    }

    template <typename RsclType, typename ExternalType>
    std::shared_ptr<SubscriberGateway<RsclType, ExternalType>>
    CreateSubscriberGateway(
        const std::string &topic,
        const ExternalCallbackFunc<ExternalType> &external_cb = nullptr,
        const CallbackFunc<RsclType> &cb = nullptr,
        const SubscriberConf &conf = DEFAULT_SUBCONF) {
        return impl_->CreateSubscriberGateway<RsclType, ExternalType>(
            topic, external_cb, cb, conf);
    }

    template <typename MessageT>
    std::shared_ptr<ExternalSubscriberToInternalPublisher<MessageT>>
    CreateExternalSubscriberToInternalPublisher(
        const std::string &external_topic,
        const std::string &internal_topic,
        const CallbackFunc<MessageT> &cb = nullptr,
        const SubscriberConf &sub_conf = DEFAULT_SUBCONF,
        const PublisherConf &pub_conf = DEFAULT_PUBCONF) {
        return impl_->CreateExternalSubscriberToInternalPublisher<MessageT>(
            external_topic, internal_topic, cb, sub_conf, pub_conf);
    }

    // Used by internal_only
    template <typename MessageT>
    std::shared_ptr<Subscriber<MessageT>> _CreateSubscriber(
        const std::string &topic,
        std::shared_ptr<Subscriber<MessageT>> subscriber) {
        return impl_->_CreateSubscriber<MessageT>(topic, subscriber);
    }
    template <typename RsclType, typename ExternalType>
    std::shared_ptr<SubscriberGateway<RsclType, ExternalType>>
    _CreateSubscriberGateway(
        const std::string &topic,
        std::shared_ptr<SubscriberGateway<RsclType, ExternalType>> subscriber) {
        return impl_->_CreateSubscriber<RsclType, ExternalType>(topic,
                                                                subscriber);
    }

    /**
     * @brief Create a Service object
     *
     * @tparam Request Data Type of the request (Only Support Protobuf Right
     * Now)
     * @tparam Response Data Type of the Response (Only Support Protobuf Right
     * Now)
     * @param service_name Name of Service
     * @param cb Function to process the request and output the response
     * @param conf Configuration Parameter
     * @return std::shared_ptr<Service<Request, Response>>
     */
    template <typename Request, typename Response>
    std::shared_ptr<Service<Request, Response>> CreateService(
        const std::string &service_name,
        const typename Service<Request, Response>::ServiceCallback &cb,
        const ServiceConf &conf = ServiceConf()) {
        return impl_->CreateService<Request, Response>(service_name, cb, conf);
    }

    /**
     * @brief Create a Client object
     *
     * @tparam Request Data Type of the request (Only Support Protobuf Right
     * Now)
     * @tparam Response Data Type of the Response (Only Support Protobuf Right
     * Now)
     * @param service_name Name of Service
     * @param conf Configuration Parameter
     * @return std::shared_ptr<Client<Request, Response>>
     */
    template <typename Request, typename Response>
    std::shared_ptr<Client<Request, Response>> CreateClient(
        const std::string &service_name,
        const ClientConf &conf = ClientConf()) {
        return impl_->CreateClient<Request, Response>(service_name, conf);
    }

    inline MwNodeHandle *InnerHandle() const override {
        return impl_->InnerHandle();
    }

    inline senseAD::service_discovery::ServiceDiscovery *GetServiceDiscovery()
        const {
        return impl_->GetServiceDiscovery();
    }

    /**
     * @brief Get the Created Subscriber objects of same topic
     *
     * @tparam MessageT
     * @param topic
     * @return std::vector<std::shared_ptr<Subscriber<MessageT>>>
     */
    template <typename MessageT>
    inline std::vector<std::shared_ptr<Subscriber<MessageT>>> GetSubscribers(
        const std::string &topic) const {
        return impl_->GetSubscribers<Subscriber<MessageT>>(topic);
    }

    /**
     * @brief Get all Created Subscriber objects
     *
     * @param topic
     * @return std::vector<std::weak_ptr<SubscriberBase>>
     */
    std::vector<std::weak_ptr<SubscriberBase>> GetAllSubscribers() const {
        return impl_->GetAllSubscribers();
    }

    /**
     * @brief change default_conf_ default_disable_callback
     */
    void SetConfDisableCallback(const bool &enable);
    /**
     * @brief set conf.default_disable_callback
     * if conf.default_disable_callback not set
     */
    SubscriberConf ToDefaultConf(const SubscriberConf &in);

    inline void AdjustAllowedPriorityLevel(
        const senseAD::rscl::idl::PriorityLevel p) {
        impl_->AdjustAllowedPriorityLevel(p);
    }
    inline senseAD::rscl::idl::PriorityLevel GetPriorityLevel() const {
        return impl_->GetPriorityLevel();
    }

    inline void ClearEntities() { impl_->ClearEntities(); }

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
        const std::string &topic, const someip::SomeipEventConfig &conf) {
        return impl_->CreateSomeIPPublisher<MessageT>(topic, conf);
    }

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
        const someip::SomeipEventConfig &conf) {
        return impl_->CreateSomeIPSubscriber<MessageT>(topic, cb, conf);
    }

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
        const std::string &service_name,
        const someip::SomeipMethodConfig &conf) {
        return impl_->CreateSomeIPClient<RequestT, ResponseT>(service_name,
                                                              conf);
    }

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
        const someip::SomeipMethodConfig &conf) {
        return impl_->CreateSomeIPService<RequestT, ResponseT>(service_name,
                                                               handler, conf);
    }
#endif

 private:
    /**
     * @brief Construct a new Node. Only allow to be called from Runtime
     *
     * @param name node name
     * @param node_namespace namespace of the node
     */
    Node(const std::string &name, const std::string &node_namespace);
    inline std::weak_ptr<NodeImpl> ImplWeakPtr() { return impl_; }

 private:
    std::shared_ptr<NodeImpl> impl_;

    friend class senseAD::rscl::runtime::NodeManagement;
    /**
     * @brief Component depend on the mw-level handle, so need to give it a
     * privilege
     *
     */
    friend class senseAD::rscl::component::ComponentBase;
};
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
