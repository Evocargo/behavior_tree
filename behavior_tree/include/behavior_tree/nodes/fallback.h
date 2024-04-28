#pragma once

#include "behavior_node.h"
#include "status.h"
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a control node which contains child nodes and calls them
 * sequentially until one of them returns Status::Success.
 *
 * If a child returns Status::Success, the Fallback node immediately returns
 * Status::Success. If all children return Status::Failure, the Fallback node
 * returns Status::Failure. This node is designed for use in behavior trees
 * where fallback (selector) logic is required.
 */
class Fallback : public BehaviorNode {
public:
  /**
   * @brief Constructs a new Fallback object.
   *
   * @param description A text description for behavior tree viewer.
   * @param children Child nodes.
   */
  Fallback(const std::string &description, const Children &children);

  /**
   * @brief Executes the node's logic.
   *
   * @return Status::Success if any child node returns Status::Success.
   * @return Status::Failure if all child nodes return Status::Failure.
   */
  Status operator()() override;
};

} // namespace evo::behavior
