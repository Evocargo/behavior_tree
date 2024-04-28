#pragma once

#include "action.h"
#include "behavior_node.h"
#include "status.h"
#include <memory>
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a control node which contains one child node and maintains
 * its state until explicitly unlatched.
 *
 * This node calls its child node once and retains the result until it is
 * unlatched. This behavior is useful in scenarios where a condition needs to
 * persist until an external change occurs.
 */
class Latch : public BehaviorNode {
public:
  /**
   * @brief Constructs a new Latch object with an initial latched state.
   *
   * @param child The child node to be executed.
   * @param latched Indicates if the node starts in a latched state, defaulting
   * to false.
   */
  Latch(BehaviorPtr child, bool latched = false);

  /**
   * @brief Executes the child's logic once until the latch is released.
   *
   * @return Status The status of the child node if it runs, otherwise the last
   * result while latched.
   */
  Status operator()() override;

  /**
   * @brief Creates an action which unlatches the node, allowing it to
   * re-evaluate its child node.
   *
   * @return BehaviorPtr An action node that unlatches this node.
   */
  BehaviorPtr make_unlatcher();

private:
  /// Indicates whether the node is currently latched.
  bool latched_;
  /// Stores the last result of the child node.
  Status last_result_ =
      Status::Failure; // Default to failure unless proven otherwise
};

} // namespace evo::behavior
