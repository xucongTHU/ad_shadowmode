/**
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 *
 */
#pragma once

#include <functional>
#include <unordered_map>
#include <string>

#include "ad_bag/schema/schema.h"
#include "ad_base/std/utils.h"

namespace senseAD {
namespace bag {

/**
 * @brief Read the bag without preloading the bag header and overall information
 * Support reading a accidently closed bag
 *
 */
class BagSectionVisitor {
 public:
    using HandlerFn = std::function<void(uint8_t version,
                                         const SectionIdentifier identifier,
                                         const char *data,
                                         uint64_t size)>;

    BagSectionVisitor() = default;
    ~BagSectionVisitor() = default;

    /**
     * @brief Register a handler function that will be trigger when found targer
     * section at the IterateUntillEnd call
     *
     * @param identifier target section
     * @param fn handler function
     */
    void RegisterSectionHandler(const SectionIdentifier identifier,
                                HandlerFn fn);
    /**
     * @brief Visit all section in the bag sequentially
     *
     * @param bag_path
     * @return Whether iterate success
     */
    bool IterateUntillEnd(const std::string &bag_path) const;

 private:
    std::unordered_map<SectionIdentifier,
                       HandlerFn,
                       senseAD::base::EnumClassHash>
        handlers_;
};

}  // namespace bag
}  // namespace senseAD
