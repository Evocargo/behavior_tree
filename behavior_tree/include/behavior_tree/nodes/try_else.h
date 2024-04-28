#pragma once

#include "behavior_node.h"
#include "status.h" // Ensure the Status class is included correctly
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a control node which contains two child nodes. The node
 * attempts to execute the 'try_node' first; if the 'try_node' fails, it
 * executes the 'else_node'. This structure is similar to a try-catch logic in
 * programming, where the 'else_node' acts as an emergency or fallback
 * procedure.
 *
 * @details The node returns Status::Success if 'try_node' succeeds,
 * Status::Failure if both 'try_node' and 'else_node' fail, and Status::Running
 * if any child node is still in progress.
 */
class TryElse : public BehaviorNode {
public:
  /**
   * @brief Constructs a new TryElse object with a normal and an emergency
   * behavior subtree.
   *
   * @param description A text description for behavior tree viewer.
   * @param try_node The first node to attempt.
   * @param else_node The node to execute if the try_node fails.
   */
  TryElse(const std::string &description, BehaviorPtr try_node,
          BehaviorPtr else_node);

  /**
   * @brief Executes the node's logic.
   *
   * @return Status::Success if the try_node succeeds, Status::Failure if both
   * nodes fail, and Status::Running if execution is not complete.
   */
  Status operator()() override;

private:
  /// The child node representing the normal case.
  BehaviorPtr try_node_;
  /// The child node representing the emergency case.
  BehaviorPtr else_node_;
};

} // namespace evo::behavior
