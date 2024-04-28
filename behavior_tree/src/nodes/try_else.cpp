#include "behavior_tree/nodes/try_else.h"
#include "behavior_tree/nodes/status.h" // Ensure the Status class is included correctly
namespace evo::behavior {

TryElse::TryElse(const std::string &description, BehaviorPtr try_node,
                 BehaviorPtr else_node)
    : BehaviorNode("try_else", description, try_node, else_node),
      try_node_(try_node), else_node_(else_node) {}

Status TryElse::operator()() {
  Status try_status = (*try_node_)();
  if (try_status == Status::Success) {
    return Status::Success; // Return success immediately if try_node succeeds
  }
  if (try_status == Status::Running) {
    return Status::Running; // Return running if the try_node is still in
                            // progress
  }
  // Execute else_node if try_node fails
  Status else_status = (*else_node_)();
  if (else_status == Status::Running) {
    return Status::Running; // Return running if the else_node is still in
                            // progress
  }
  return else_status; // Return the status of the else_node (success or failure)
}

} // namespace evo::behavior
