#pragma once

#include "behavior_node.h"
#include "status.h" // Ensure the Status class is included correctly
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a control node which functions like a classical
 * if-then-else operator in programming, executing child nodes based on the
 * condition evaluated by the 'if' node.
 *
 * This class provides a way to conditionally execute different branches of a
 * behavior tree, enhancing the decision-making capabilities of the system.
 */
class IfThenElse : public BehaviorNode {
public:
  /**
   * @brief Constructs a new IfThenElse object with specific child nodes for
   * 'if', 'then', and 'else' branches.
   *
   * @param description A text description for behavior tree viewer.
   * @param if_node The node that evaluates the condition.
   * @param then_node The node that is executed if the condition is true
   * (returns Status::Success).
   * @param else_node The node that is executed if the condition is false
   * (returns Status::Failure).
   */
  IfThenElse(const std::string &description, BehaviorPtr if_node,
             BehaviorPtr then_node, BehaviorPtr else_node);

  /**
   * @brief Executes the node's logic based on the 'if' condition's result.
   *
   * @return Status::Success or Status::Failure based on the execution outcome
   * of the 'then' or 'else' node.
   */
  Status operator()() override;

private:
  /// Pointer to the node that evaluates the "if" condition.
  BehaviorPtr if_node_;
  /// Pointer to the node that is executed if the "if" condition is true.
  BehaviorPtr then_node_;
  /// Pointer to the node that is executed if the "if" condition is false.
  BehaviorPtr else_node_;
};

} // namespace evo::behavior
