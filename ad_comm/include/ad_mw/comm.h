/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <chrono>
#include <future>
#include <memory>
#include <string>
#include <utility>

#include "ad_base/common/shutdownable.h"
#include "ad_mw/types.h"
#include "ad_mw/visibility_control.h"
#include "ad_mw/wrapped_handle.h"
#include "ad_mw/errcode.h"
#include "ad_mw/exception.h"

namespace senseAD {
namespace mw {
namespace comm {

static constexpr const char* INTERNAL_NODE_SPACE = "internal_node";
static const std::string INTERNAL_CHANNEL_PREFIX =  // NOLINT
    "/_internal_channel";

/**
 * @brief comm namespace contain all the API related to communation
 * The underlaying model is using the (ros-like) publisher/subscriber model with
 * the node graph. Detail can be find in any ros tutorial
 *
 */

using senseAD::mw::proto::ClientConf;
using senseAD::mw::proto::PublisherConf;
using senseAD::mw::proto::ServiceConf;
using senseAD::mw::proto::SubscriberConf;

class SwitchableWrapperdPtrInnerHandleS : public WrapperdPtrInnerHandleS {
 public:
    explicit SwitchableWrapperdPtrInnerHandleS(SharedVoidPtr internal_handle)
        : WrapperdPtrInnerHandleS(internal_handle) {}
    virtual ~SwitchableWrapperdPtrInnerHandleS() {}

    void Enable() { enabled_ = true; }
    void Disable() { enabled_ = false; }
    bool IsEnabled() const { return enabled_; }

 private:
    bool enabled_{true};
};

/**
 * @brief Basic unit. All the other candinate is created from node. ad_mw node
 * store the node hanlde in its belowing implementation
 *
 */
class ADMW_PUBLIC Node : public WrapperdPtrInnerHandleU, public ::Shutdownable {
 public:
    /**
     * @brief Create a new Node
     *
     * @param name
     * @param node_namespace
     * @return std::unique_ptr<Node>
     */
    static std::unique_ptr<Node> New(const std::string& name,
                                     const std::string& node_namespace);
    virtual ~Node();

    void ShutdownImpl() override;

 protected:
    explicit Node(UniqueVoidPtr&& internal_handle);
};

/**
 * @brief Base class used to store the internal and avoid the template type
 * issue while used by other class
 *
 */
#define DEFINE_BASE(ClassName, ConfType)                               \
    class ClassName : public SwitchableWrapperdPtrInnerHandleS {       \
     public:                                                           \
        ClassName(const ConfType& conf, SharedVoidPtr internal_handle) \
            : SwitchableWrapperdPtrInnerHandleS(internal_handle),      \
              conf_(conf) {}                                           \
        virtual ~ClassName() {}                                        \
        inline const ConfType& Get##ConfType() const { return conf_; } \
                                                                       \
     protected:                                                        \
        ConfType conf_;                                                \
    };

DEFINE_BASE(PublisherBase, PublisherConf)

/**
 * @brief Define the buffer for message, to save buffer hanlder.
 */
class Buffer {
 public:
    Buffer(uint8_t* data, std::shared_ptr<void> buffer_handler)
        : data(data), buffer_handler(buffer_handler) {}
    ~Buffer() = default;

    uint8_t* GetData() const { return data; }
    std::shared_ptr<void> GetBufferHandler() const { return buffer_handler; }

 private:
    uint8_t* data;
    std::shared_ptr<void> buffer_handler;
};

/**
 * @brief The message publication api
 *
 */
class ADMW_PUBLIC Publisher : public PublisherBase, public ::Shutdownable {
 public:
    using Ptr = std::shared_ptr<Publisher>;

    std::unique_ptr<Buffer> LoanBuffer(int size = 0);

    static Publisher::Ptr New(Node* const node, const PublisherConf& conf);
    virtual ~Publisher();
    void ShutdownImpl() override;
    std::error_code Publish(const std::shared_ptr<BorrowedRawMessage>& msg);
    std::error_code PublishLoanBuffer(std::unique_ptr<Buffer> buffer);

 protected:
    Publisher(const PublisherConf& conf, SharedVoidPtr internal_handle)
        : PublisherBase(conf, internal_handle),
          ::Shutdownable("MwPublisher", conf.channel_name()) {}
};

DEFINE_BASE(SubscriberBase, SubscriberConf)
/**
 * @brief Message subscription Api
 *
 */

class ADMW_PUBLIC Subscriber : public SubscriberBase, public ::Shutdownable {
 public:
    using Ptr = std::shared_ptr<Subscriber>;
    using CallbackFunc =
        std::function<void(const std::shared_ptr<BorrowedRawMessage>&)>;

    static Subscriber::Ptr New(Node* const node,
                               const CallbackFunc& cb,
                               const SubscriberConf& conf);
    virtual ~Subscriber();
    void ShutdownImpl() override;

    //  void Observe() const;
    //  std::shared_ptr<BorrowedRawMessage> GetLatestObserved() const;

 protected:
    Subscriber(const SubscriberConf& conf, SharedVoidPtr internal_handle)
        : SubscriberBase(conf, internal_handle),
          ::Shutdownable("MwSubscriber", conf.channel_name()) {}
};

DEFINE_BASE(ClientBase, ClientConf)

/**
 * @brief Client API
 *
 */
class ADMW_PUBLIC Client : public ClientBase, public ::Shutdownable {
 public:
    using Ptr = std::shared_ptr<Client>;

    static Client::Ptr New(Node* const node, const ClientConf& conf);
    virtual ~Client();
    void ShutdownImpl() override;
    std::shared_ptr<BorrowedRawMessage> SendRequest(
        std::shared_ptr<BorrowedRawMessage> request,
        const std::chrono::milliseconds timeout_ms);
    std::shared_future<std::shared_ptr<BorrowedRawMessage>> AsyncSendRequest(
        std::shared_ptr<BorrowedRawMessage> request);
    bool IsServiceReady() const;

 protected:
    Client(const ClientConf& conf, SharedVoidPtr internal_handle)
        : ClientBase(conf, internal_handle),
          ::Shutdownable("MwClient", conf.service_name()) {}

 private:
};

DEFINE_BASE(ServiceBase, ServiceConf)

/**
 * @brief Service Api
 * Suppose service callback should be async.
 * Otherwise would lead to multiple client response blocked
 */
class ADMW_PUBLIC Service : public ServiceBase, public ::Shutdownable {
 public:
    using Ptr = std::shared_ptr<Service>;
    using ServiceCallback =
        std::function<void(const std::shared_ptr<BorrowedRawMessage>&,
                           std::shared_ptr<BorrowedRawMessage>&)>;
    static Service::Ptr New(Node* const node,
                            const ServiceCallback& cb,
                            const ServiceConf& conf);
    virtual ~Service();
    void ShutdownImpl() override;

 protected:
    Service(const ServiceConf& conf, SharedVoidPtr internal_handle)
        : ServiceBase(conf, internal_handle),
          ::Shutdownable("MwService", conf.service_name()) {}
    std::function<void()> shutdown_handler_;
};

}  // namespace comm
}  // namespace mw
}  // namespace senseAD
