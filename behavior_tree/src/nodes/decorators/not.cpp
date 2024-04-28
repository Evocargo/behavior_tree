#include "behavior_tree/nodes/decorators/not.h"

namespace evo::behavior {

Not::Not(BehaviorPtr child)
    : BehaviorNode("not", "Inverting " + child->description(), child) {}

Status Not::operator()() {
  auto &child_node = children().front();
  Status result = (*child_node)();
  switch (Status::State(result)) {
  case Status::SUCCESS:
    return Status::Failure;
  case Status::FAILURE:
    return Status::Success;
  case Status::RUNNING:
    return Status::Running;
  default:
    return Status::Failure; // Default case to handle unexpected status
  }
}

} // namespace evo::behavior
