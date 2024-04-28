#pragma once

#include "behavior_node.h"
#include "status.h" // Include the Status class header
#include <exception>
#include <functional>
#include <string>

namespace evo::behavior {

/**
 * @brief Represents a leaf node which executes some code when calling
 * operator() and reports success unless an exception occurs.
 *
 */
class Action : public BehaviorNode {
public:
  /**
   * @brief Constructs a new Action object.
   *
   * @param behavior A function to be executed by this node. This function does
   * not return a value.
   * @param description A text description for behavior tree viewer.
   */
  Action(std::function<void()> behavior, std::string const &description);

  /**
   * @brief Default constructor, deleted to enforce providing behavior and
   * description.
   */
  Action() = delete;

  /**
   * @brief Copy constructor.
   */
  Action(Action const &) = default;

  /**
   * @brief Copy assignment operator.
   */
  Action &operator=(Action const &) = default;

  /**
   * @brief Executes the node's logic.
   *
   * @return Status::Success if the behavior executes without throwing an
   * exception, otherwise Status::Failure.
   */
  Status operator()() override;

private:
  /// The node's logic to be executed, does not return a value.
  std::function<void()> behavior_;
};

} // namespace evo::behavior
