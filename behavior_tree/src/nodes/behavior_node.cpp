#include <behavior_tree/nodes/behavior_node.h>

namespace evo::behavior {

void BehaviorNode::reset() {
  for (auto const &child : children()) {
    child->reset();
  }
}

const std::string &BehaviorNode::type() const { return type_; }

const std::string &BehaviorNode::description() const { return description_; }

const BehaviorNode::Children &BehaviorNode::children() const {
  return children_;
}

} // namespace evo::behavior
