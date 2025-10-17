/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Zhang Yi <zhangyi15@senseauto.com>
 */

#pragma once

namespace YAML {
class Node;
}
namespace senseAD {
namespace rscl {
namespace mainboard {

class YamlUnwrapper {
 public:
    void Unwrap(YAML::Node node) const;

 private:
};

}  // namespace mainboard
}  // namespace rscl
}  // namespace senseAD
