#include "behavior_tree/nodes/if_then.h"

namespace evo::behavior {

IfThen::IfThen(const std::string &description, BehaviorPtr if_node,
               BehaviorPtr then_node)
    : BehaviorNode("if_then", description), if_node_(std::move(if_node)),
      then_node_(std::move(then_node)) {}

Status IfThen::operator()() {
  Status condition_status = (*if_node_)();
  if (condition_status == Status::Success) {
    return (*then_node_)();
  }
  if (condition_status == Status::Running) {
    return Status::Running;
  }
  return Status::Success; // Returns success if the condition is not met
}

} // namespace evo::behavior
