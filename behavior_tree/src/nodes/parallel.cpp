#include "behavior_tree/nodes/parallel.h"
#include "behavior_tree/nodes/status.h" // Ensure the Status class is included correctly
namespace evo::behavior {

Parallel::Parallel(const std::string &description, const Children &children)
    : BehaviorNode("parallel", description, children) {}

Status Parallel::operator()() {
  bool all_success = true;
  for (const auto &child : children()) {
    Status result = (*child)();
    if (result == Status::Running) {
      return Status::Running; // Immediate return if any child is running
    }
    if (result == Status::Failure) {
      all_success = false; // Mark failure but continue evaluating all children
    }
  }
  return all_success ? Status::Success : Status::Failure;
}

} // namespace evo::behavior
