/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Wang Xiantao <wangxiantao@senseauto.com>
 */

#pragma once

#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <functional>

#define ZMQ_HAS_RVALUE_REFS
#include "cppzmq/zmq.hpp"

namespace senseAD {
namespace rscl {
namespace comm {
namespace hetero {

enum class ZmqEvent : uint32_t {
    CONNECTED = 0,
    DISCONNECTED,
    CONNECT_RETRIED,
    CONNECT_DELAYED,
    LISTENING,
    ACCEPTED,
    ACCEPT_FAILED,
    CLOSED,
    BIND_FAILED
};

inline constexpr const char* ZmqEventName(ZmqEvent event) {
    switch (event) {
        case ZmqEvent::CONNECTED: return "CONNECTED";
        case ZmqEvent::DISCONNECTED: return "DISCONNECTED";
        case ZmqEvent::CONNECT_RETRIED: return "CONNECT_RETRIED";
        case ZmqEvent::CONNECT_DELAYED: return "CONNECT_DELAYED";
        case ZmqEvent::LISTENING: return "LISTENING";
        case ZmqEvent::ACCEPTED: return "ACCEPTED";
        case ZmqEvent::ACCEPT_FAILED: return "ACCEPT_FAILED";
        case ZmqEvent::CLOSED: return "CLOSED";
        case ZmqEvent::BIND_FAILED: return "BIND_FAILED";
        default: return "";
    }
}

struct ZmqEventInfo {
    ZmqEvent event_tag;
    std::string event_addr;
};

class ZmqMonitor : public zmq::monitor_t {
 public:
    using ZmqEventCallback = std::function<void(const ZmqEventInfo& info)>;
    /**
     * @brief Add(not cover) a event callback function.
     * Don't put blocking functions!
     *
     * @param event ZmqEvent enum key
     * @param func callback when event occur
     */
    void RegisterEventFunc(ZmqEvent event, const ZmqEventCallback& func) {
        event_callback_[event].push_back(func);
    }

    /**
     * @brief Add(not cover) a callback for all events.
     * Don't put blocking functions!
     *
     * @param func
     */
    void RegisterAllEventFunc(const ZmqEventCallback& func) {
        all_event_callback_.push_back(func);
    }

    /**
     * @brief clear a event callback function
     *
     * @param event
     */
    void UnregisterEventFunc(ZmqEvent event) { event_callback_.erase(event); }

    // listening for the on_event_connected event, notify user if successful.
    void on_event_connected(const zmq_event_t& event,
                            const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::CONNECTED, addr);
    }

    void on_event_disconnected(const zmq_event_t& event,
                               const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::DISCONNECTED, addr);
    }

    void on_event_connect_retried(const zmq_event_t& event,
                                  const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::CONNECT_RETRIED, addr);
    }

    void on_event_connect_delayed(const zmq_event_t& event,
                                  const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::CONNECT_DELAYED, addr);
    }

    void on_event_listening(const zmq_event_t& event,
                            const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::LISTENING, addr);
    }

    void on_event_accepted(const zmq_event_t& event,
                           const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::ACCEPTED, addr);
    }

    void on_event_accept_failed(const zmq_event_t& event,
                                const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::ACCEPT_FAILED, addr);
    }

    void on_event_closed(const zmq_event_t& event, const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::CLOSED, addr);
    }

    void on_event_bind_failed(const zmq_event_t& event,
                              const char* addr) override {
        (void)event;
        EventTriggerFuncImpl(ZmqEvent::BIND_FAILED, addr);
    }

    ZmqEventInfo GetCurrentEventInfo() const { return info_; }

 private:
    void EventTriggerFuncImpl(ZmqEvent event, const char* addr);

    std::mutex mu_;
    ZmqEventInfo info_;
    std::map<ZmqEvent, std::vector<ZmqEventCallback>> event_callback_;
    std::vector<ZmqEventCallback> all_event_callback_;
};

}  // namespace hetero
}  // namespace comm
}  // namespace rscl
}  // namespace senseAD
