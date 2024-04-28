#include "behavior_tree/behavior_tree.h" // Include the BehaviorTree class declaration
#include "behavior_tree/nodes/behavior_node.h" // Ensure correct path to the behavior node header
#include "behavior_tree/nodes/status.h" // Include the Status class for handling node statuses
#include <memory>

namespace evo::behavior {

BehaviorTree::BehaviorTree(BehaviorPtr root) : root_(root) {}

void BehaviorTree::set_root(BehaviorPtr root) { root_ = root; }

Status BehaviorTree::run() {
  if (!root_) {
    return Status::Failure; // Return failure if there is no root node set
  }
  return (*root_)(); // Execute the root node and return its status
}

} // namespace evo::behavior