#pragma once

#include "behavior_node.h"
#include "status.h" // Include the Status class for status handling
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a control node which contains child nodes and calls them
 * sequentially. It skips over children that are still running and returns the
 * first definitive success or failure status it encounters. If all children are
 * running, it returns Status::Running.
 *
 * @details This node is designed for scenarios where child nodes may be
 * operations that need to be attempted but can be skipped if not immediately
 * successful or ready.
 */
class Skipper : public BehaviorNode {
public:
  /**
   * @brief Constructs a new Skipper object with a descriptive label and a set
   * of child nodes.
   *
   * @param description A text description for behavior tree viewer.
   * @param children Child nodes that form the sequence.
   */
  Skipper(const std::string &description, const Children &children);

  /**
   * @brief Executes the node's logic sequentially across the child nodes,
   * skipping those that are running.
   *
   * @return Status::Success if any child node returns Status::Success,
   *         Status::Failure if any child node returns Status::Failure,
   *         and Status::Running if all children are running.
   */
  Status operator()() override;
};

} // namespace evo::behavior
