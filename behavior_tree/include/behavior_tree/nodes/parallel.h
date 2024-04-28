#pragma once

#include "behavior_node.h"
#include "status.h" // Ensure the Status class is included correctly
#include <string>
#include <vector>

namespace evo::behavior {

/**
 * @brief Represents a control node which contains child nodes and calls ALL of
 * them sequentially, regardless of the result. The node returns the most
 * significant result among children, prioritizing Status::Running over other
 * statuses.
 *
 * This node behaves like a sequence node but evaluates all children every tick,
 * and it may return Status::Running if any child is still running, otherwise it
 * returns Status::Success only if all children succeed.
 */
class Parallel : public BehaviorNode {
public:
  /**
   * @brief Constructs a new Parallel object with a descriptive label and a set
   * of child nodes.
   *
   * @param description A text description for behavior tree viewer.
   * @param children Child nodes to be executed in parallel.
   */
  Parallel(const std::string &description, const Children &children);

  /**
   * @brief Executes the node's logic.
   *
   * @return Status::Running if any child is still running, Status::Success if
   * all children succeed, or Status::Failure if any child fails but none are
   * running.
   */
  Status operator()() override;
};

} // namespace evo::behavior
