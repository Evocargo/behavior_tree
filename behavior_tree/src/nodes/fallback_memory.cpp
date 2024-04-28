#include "behavior_tree/nodes/fallback_memory.h"

namespace evo::behavior {

FallbackMemory::FallbackMemory(const std::string &description,
                               const Children &children)
    : BehaviorNode("fallback_memory", description, children),
      current_child_(this->children().begin()) {}

Status FallbackMemory::operator()() {
  // Start from the current child and evaluate until one succeeds or all are
  // evaluated
  while (current_child_ != children().end()) {
    Status child_status = (**current_child_)();
    if (child_status != Status::FAILURE) {
      return child_status;
    }
    current_child_++;
  }
  reset();                // Ensure the next call starts from the first child
  return Status::FAILURE; // All children failed, return Failure
}

void FallbackMemory::reset() {
  current_child_ = children().begin();
  for (auto const &child : children()) {
    child->reset(); // Reset each child's state
  }
}

} // namespace evo::behavior
