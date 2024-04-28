#include "behavior_tree/nodes/sequence_memory.h"
#include <iostream>

namespace evo::behavior {

SequenceMemory::SequenceMemory(const std::string &description,
                               const Children &children)
    : BehaviorNode("sequence_memory", description, children),
      current_child_(this->children().begin()) {}

Status SequenceMemory::operator()() {
  for (; current_child_ != children().end(); ++current_child_) {
    Status child_status = (**current_child_)();
    std::cout << child_status << std::endl;
    if (child_status != Status::SUCCESS) {
      // Return running or failure immediately
      return child_status;
    }
  }
  // If all children succeeded and we reached the end, reset for the next run
  // and return success
  reset();
  return Status::SUCCESS;
}

void SequenceMemory::reset() {
  current_child_ = children().begin();
  for (auto const &child : children()) {
    child->reset();
  }
}

} // namespace evo::behavior
