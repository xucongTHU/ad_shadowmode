#include "RsclChannelManager.h"
#include "common/logger/Logger.h"

namespace shadow {
namespace channel {

bool RsclChannelManager::Init
        (const std::shared_ptr<senseAD::rscl::comm::Node>& node, 
         const strategy::StrategyConfig& config,
         const std::shared_ptr<TriggerFactory>& trigger_factory,
         const std::shared_ptr<RsclRecorder>& rscl_recorder,
         const std::shared_ptr<DataReporter>& data_reporter) {
    node_ = node;
    strategy_config_ = config;
    trigger_factory_ = trigger_factory;
    rscl_recorder_ = rscl_recorder;
    data_reporter_ = data_reporter;

    message_subject_ = std::make_unique<Subject>();

    bool ret = InitSubscribers();
    CHECK_AND_RETURN(ret, RsclChannelManager, "InitSubscribers failed", false);

    ret = InitObservers();
    CHECK_AND_RETURN(ret, RsclChannelManager, "InitObservers failed", false);

    return ret;
}

bool RsclChannelManager::InitSubscribers() {
    senseAD::rscl::idl::SubscriberConf conf;
    conf.mutable_qos_profile()->set_depth(20); 
    conf.mutable_qos_profile()->set_reliability(
        senseAD::rscl::idl::QosReliabilityPolicy::RELIABILITY_BEST_EFFORT);

    for (const auto& strategy : strategy_config_.strategies) {
        if (!strategy.trigger.enabled)  continue;
        for (const auto& channel : strategy.dds.channels) {
            std::string topic = channel.topic;
            if (subscribers_.find(topic) != subscribers_.end()) {
                continue;
            }

            auto subscriber = node_->CreateSubscriber<senseAD::rscl::comm::RawMessage>(
                topic,
                [this, topic](const TRawMessagePtr& raw_message) {
                    this->Notify(topic, raw_message);
                },
                conf
            );

            if (!subscriber) {
                LOG_ERROR("Create subscriber failed for topic: %s", topic.c_str());
                return false;
            }
            LOG_INFO("Init subscriber for topic: %s, node: %p, subscriber: %p", topic.c_str(), node_.get(), subscriber.get());
            subscribers_[topic] = subscriber;
        }
    }
    return true;

}

bool RsclChannelManager::InitObservers() {
    if (rscl_recorder_) {
        AddObserver(rscl_recorder_);
        LOG_INFO("Added RsclRecorder as observer");
    }

    if (data_reporter_) {
        AddObserver(data_reporter_);
        LOG_INFO("Added DataReporter as observer");
    }

    
    if (trigger_factory_) {
        for (const auto& strategy : strategy_config_.strategies) {
            auto trigger = trigger_factory_->GetTrigger(strategy.trigger.triggerName);
            if (trigger) {
                AddObserver(trigger);
                LOG_INFO("Added %s as observer", strategy.trigger.triggerName.c_str());
            }

        }
    }

    LOG_INFO("InitObservers ok");
    return true;
}

void RsclChannelManager::OnMessageReceived(const std::string& topic, const TRawMessagePtr& msg) {
    auto cur_clock_mode = senseAD::base::time::ClockMode::SYSTEM_TIME;
    uint64_t message_time = senseAD::base::time::Time::Now(&cur_clock_mode).ToMicrosecond();
    auto header = msg->Header();
    if (header.is_enabled)
    {
        // message_time = msg->Header().stamp;
    }
    else
    {
        LOG_ERROR("OnMessageReceived, topic: %s, header parse error", topic.c_str());
        return;
    }

    // in start of replay mode rscl timestamp is zero
    if (message_time == 0) {
        LOG_ERROR("OnMessageReceived, topic: %s, message_time is zero", topic.c_str());
        return;
    }
}

void RsclChannelManager::AddObserver(std::shared_ptr<Observer> observer) {
    message_subject_->AddObserver(observer);
}

void RsclChannelManager::RemoveObserver(std::shared_ptr<Observer> observer) {
    message_subject_->RemoveObserver(observer);
}

void RsclChannelManager::Notify(const std::string& topic, const TRawMessagePtr& msg) {
    message_subject_->NotifyAll(topic, msg);
}


} // namespace channel
} // namespace shadow