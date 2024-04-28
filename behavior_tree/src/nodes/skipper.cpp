#include "behavior_tree/nodes/skipper.h"

namespace evo::behavior {

Skipper::Skipper(const std::string &description, const Children &children)
    : BehaviorNode("skipper", description, children) {}

Status Skipper::operator()() {
  for (const auto &child : children()) {
    Status result = (*child)();
    if (result != Status::Running) {
      return result;
    }
  }
  return Status::Running; // Return running if all children are running, else
                          // return failure
}

} // namespace evo::behavior
