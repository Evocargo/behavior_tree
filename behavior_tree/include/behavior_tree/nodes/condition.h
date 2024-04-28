#pragma once

#include "behavior_node.h"
#include "status.h" // Ensure the Status class is included correctly
#include <functional>
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a leaf node which executes some logic when called and
 * returns a Status.
 *
 * This class embodies a condition within a behavior tree, typically used to
 * decide which path the tree should take.
 */
class Condition : public BehaviorNode {
public:
  /**
   * @brief Constructs a new Condition object.
   *
   * @param condition A function to be executed by this node. This function
   * should now return a Status to indicate the result more clearly.
   * @param description A text description for behavior tree viewer.
   */
  Condition(std::function<Status()> condition, const std::string &description);

  /**
   * @brief Executes the node's logic.
   *
   * @return Status depending on the condition logic.
   */
  Status operator()() override;

private:
  /// The node's logic to be executed, adjusted to return a Status.
  std::function<Status()> condition_;
};

} // namespace evo::behavior
