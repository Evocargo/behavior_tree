#pragma once

#include "../behavior_node.h"
#include "../status.h"
#include <memory>

namespace evo::behavior {

/**
 * @brief A decorator node that inverts the result of its child node.
 *
 * This node has one child and inverts the result of this child node:
 * - SUCCESS becomes FAILURE
 * - FAILURE becomes SUCCESS
 * - RUNNING remains RUNNING
 */
class Not : public BehaviorNode {
public:
  /**
   * @brief Constructs a new Not decorator node with a child node.
   *
   * @param child The child node whose result will be inverted.
   */
  explicit Not(BehaviorPtr child);

  /**
   * @brief Executes the child node and inverts its result.
   *
   * @return Status The inverted status of the child node.
   */
  Status operator()() override;
};

} // namespace evo::behavior
