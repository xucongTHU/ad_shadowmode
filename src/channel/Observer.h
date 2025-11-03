//
// Created by xucong on 25-9-24.
// © 2025 Synaptix AI. All rights reserved.
// Tsung Xu<xucong@synaptix.ai>
//

#ifndef SHADOW_MODE_OBSERVER_H
#define SHADOW_MODE_OBSERVER_H

#include "ad_rscl/ad_rscl.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <string>
#include <functional>

using TRawMessagePtr = std::shared_ptr<ReceivedMsg<senseAD::rscl::comm::RawMessage>>;

class Observer {
public:
    virtual ~Observer() = default;

    virtual void OnMessageReceived(const std::string& topic, const TRawMessagePtr& subject) = 0;
};

class Subject {
public:
    virtual~Subject() = default;

    void AddObserver(std::shared_ptr<Observer> observer) {
        observers_.emplace_back(observer);
    }


    void RemoveObserver(std::shared_ptr<Observer> observer) {
        auto iter = std::find(observers_.begin(), observers_.end(), observer);
        if (iter != observers_.end()) {
            observers_.erase(iter);
        }
    }

    void NotifyAll(const std::string& topic, const TRawMessagePtr& subject) {
        for (const auto& observer : observers_) {
            observer->OnMessageReceived(topic, subject);
        }
        // std::cout << "notify all observers, topic: " << topic << std::endl;
    }

    const std::vector<std::shared_ptr<Observer>>& GetObservers() const { return observers_; }

protected:
    std::vector<std::shared_ptr<Observer>> observers_;
};

#endif //SHADOW_MODE_OBSERVER_H
