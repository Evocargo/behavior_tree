#pragma once

#include "behavior_node.h"
#include "status.h"
#include <string>

namespace evo::behavior {

class IfThen : public BehaviorNode {
public:
  IfThen(const std::string &description, BehaviorPtr if_node,
         BehaviorPtr then_node);

  Status operator()() override;

private:
  BehaviorPtr if_node_;
  BehaviorPtr then_node_;
};

} // namespace evo::behavior
