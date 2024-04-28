#include "behavior_tree/nodes/fallback.h"
#include <iostream>
namespace evo::behavior {

Fallback::Fallback(const std::string &description, const Children &children)
    : BehaviorNode("fallback", description, children) {}

Status Fallback::operator()() {
  int child_index = 0;
  for (const auto &child : children()) {
    Status result = (*child)();
    std::cout << "Child index: " << child_index << ", Status: " << result
              << std::endl;
    if (result != Status::Failure) {
      return result; // Early exit if any child succeeds
    }
    ++child_index;
  }
  return Status::Failure; // Return failure if no child succeeds
}

} // namespace evo::behavior
