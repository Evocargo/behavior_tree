#include "behavior_tree/nodes/sequence.h"

namespace evo::behavior {

Sequence::Sequence(const std::string &description, const Children &children)
    : BehaviorNode("sequence", description, children) {}

Status Sequence::operator()() {
  for (auto &child : children()) {
    Status result = (*child)();
    if (result != Status::Success) {
      return result;
    }
  }
  return Status::Success; // All children succeeded
}

} // namespace evo::behavior
