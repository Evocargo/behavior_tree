#pragma once

#include "behavior_node.h"
#include "status.h" // Ensure the Status class is included correctly
#include <string>
#include <vector>

namespace evo::behavior {

/**
 * @brief Represents a control node which contains child nodes and calls them
 * sequentially until any of the child nodes returns Status::Success. If a child
 * returns Status::Failure, it will not be called again in the next tick. The
 * FallbackMemory node resets to start from the first child after every tick,
 * regardless of the outcome. You can manually reset the iteration sequence by
 * calling the reset() method.
 *
 */
class FallbackMemory : public BehaviorNode {
public:
  /**
   * @brief Constructs a new FallbackMemory object.
   *
   * @param description A text description for behavior tree viewer.
   * @param children Child nodes.
   */
  FallbackMemory(const std::string &description, const Children &children);

  /**
   * @brief Executes the node's logic.
   *
   * @return Status::Success if any of the child nodes returns Status::Success.
   * @return Status::Failure if all child nodes return Status::Failure.
   */
  Status operator()() override;

  /**
   * @brief Resets the iteration over child nodes to the beginning.
   */
  void reset() override;

private:
  /// Iterator to keep track of the current child being processed.
  Children::const_iterator current_child_;
};

} // namespace evo::behavior
