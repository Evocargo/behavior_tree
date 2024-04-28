#include "behavior_tree/nodes/if_then_else.h"

namespace evo::behavior {

IfThenElse::IfThenElse(const std::string &description, BehaviorPtr if_node,
                       BehaviorPtr then_node, BehaviorPtr else_node)
    : BehaviorNode("if_then_else", description), if_node_(std::move(if_node)),
      then_node_(std::move(then_node)), else_node_(std::move(else_node)) {}

Status IfThenElse::operator()() {
  Status condition_status = (*if_node_)();
  if (condition_status == Status::Success) {
    return (*then_node_)();
  } else if (condition_status == Status::Failure) {
    return (*else_node_)();
  } else {
    return Status::Running;
  }
}

} // namespace evo::behavior
