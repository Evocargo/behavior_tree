#include "behavior_tree/nodes/latch.h"

namespace evo::behavior {

Latch::Latch(BehaviorPtr child, bool latched)
    : BehaviorNode("latch", "Latching " + child->description(), child),
      latched_(latched) {}

Status Latch::operator()() {
  if (!latched_) {
    auto &child_node = children().front();
    last_result_ = (*child_node)();
    latched_ = true; // Latch after the first successful execution
  }
  return last_result_;
}

BehaviorPtr Latch::make_unlatcher() {
  return std::make_shared<Action>([this] { latched_ = false; },
                                  "Unlatching " + description());
}

} // namespace evo::behavior
