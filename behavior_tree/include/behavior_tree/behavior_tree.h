#pragma once

#include "nodes/behavior_node.h" // Ensure correct path to the behavior node header
#include "nodes/status.h" // Include the Status class for handling node statuses
#include <memory>

namespace evo::behavior {

/**
 * @brief Represents a behavior tree, managing the execution of a hierarchical
 * structure of nodes that control decision making and behavior.
 */
class BehaviorTree {
public:
  /**
   * @brief Constructs a new BehaviorTree object with a specified root node.
   *
   * @param root The root node of the behavior tree, where execution starts.
   */
  explicit BehaviorTree(BehaviorPtr root);

  /**
   * @brief Default constructor for an empty BehaviorTree, intended for delayed
   * initialization.
   */
  BehaviorTree() = default;

  /**
   * @brief Sets the root node of the behavior tree.
   *
   * @param root The root node to set, starting point for tree execution.
   */
  virtual void set_root(BehaviorPtr root);

  /**
   * @brief Runs the behavior tree starting from the root node.
   *
   * @return Status The status of the behavior tree execution. Returns
   * Status::Failure if no root is set.
   */
  Status run();

  /**
   * @brief Virtual destructor to ensure proper cleanup in derived classes.
   */
  virtual ~BehaviorTree() = default;

private:
  /// The root node of the behavior tree.
  BehaviorPtr root_;
};

} // namespace evo::behavior
