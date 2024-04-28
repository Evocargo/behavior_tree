#pragma once

#include "behavior_node.h"
#include "status.h" // Include the Status class for standardized status handling
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a control node which contains child nodes and executes them
 * sequentially. The execution stops and returns Status::Failure if any child
 * node returns Status::Failure. If a child returns Status::Running, the
 * sequence will pause and resume from that child on the next execution call.
 *
 * @details This node is designed to mimic the behavior of a logical AND
 * operation where all children must succeed for the sequence to succeed.
 */
class Sequence : public BehaviorNode {
public:
  /**
   * @brief Constructs a new Sequence object with a list of child nodes.
   *
   * @param description A text description for behavior tree viewer.
   * @param children Child nodes that form the sequence.
   */
  Sequence(const std::string &description, const Children &children);

  /**
   * @brief Executes the node's logic sequentially across the child nodes.
   *
   * @return Status::Success if all child nodes return Status::Success,
   *         Status::Failure if any child node returns Status::Failure,
   *         and Status::Running if execution is paused on a running child.
   */
  Status operator()() override;
};

} // namespace evo::behavior
