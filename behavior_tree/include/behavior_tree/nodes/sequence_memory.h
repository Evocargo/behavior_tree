#pragma once

#include "behavior_node.h"
#include "status.h" // Ensure the Status class is included correctly
#include <algorithm>
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a control node which contains child nodes and calls them
 * sequentially, remembering the last successfully executed child. Execution
 * resumes from the last successful child on the next call unless reset.
 *
 * If a child returns Status::Failure, the sequence stops and this status is
 * returned. If all children return Status::Success, then Status::Success is
 * returned. If any child returns Status::Running, the sequence pauses and will
 * continue from this child on the next tick.
 */
class SequenceMemory : public BehaviorNode {
public:
  /**
   * @brief Constructs a new SequenceMemory object with a list of child nodes.
   *
   * @param description A text description for behavior tree viewer.
   * @param children Child nodes that form the sequence.
   */
  SequenceMemory(const std::string &description, const Children &children);

  /**
   * @brief Executes the node's logic sequentially across child nodes.
   *
   * @return Status::Success if all children succeed, Status::Failure if any
   * fail, or Status::Running if the execution is paused.
   */
  Status operator()() override;

  /**
   * @brief Resets the sequence to start from the first child node.
   */
  void reset() override;

private:
  /// Iterator to track the current child being executed.
  Children::const_iterator current_child_;
};

} // namespace evo::behavior
