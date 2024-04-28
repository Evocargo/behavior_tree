#pragma once

#include "status.h" // Include the Status class header
#include <memory>
#include <string>
#include <vector>

namespace evo::behavior {

class BehaviorNode;
using BehaviorPtr = std::shared_ptr<BehaviorNode>;

/**
 * @brief Base class for all the node types of behavior tree.
 *
 * This class represents a behavior node that can execute a specific behavior
 * and return a status indicating the outcome of that behavior.
 */
class BehaviorNode {
public:
  using Children = std::vector<BehaviorPtr>;

  /**
   * @brief Constructs a new Behavior Node object with detailed specifications.
   *
   * @param type The type of the node.
   * @param description A text description for behavior tree viewer.
   * @param args Child nodes.
   */
  template <class... Args>
  BehaviorNode(const std::string &type, const std::string &description,
               Args... args)
      : type_(type), description_(description), children_{args...} {}

  /**
   * @brief Default constructor for Behavior Node, deleted to enforce explicit
   * initialization.
   */
  BehaviorNode() = delete;

  /**
   * @brief Copy constructor.
   */
  BehaviorNode(const BehaviorNode &) = default;

  /**
   * @brief Copy assignment operator.
   */
  BehaviorNode &operator=(const BehaviorNode &) = default;

  /**
   * @brief Virtual destructor for safe polymorphic use.
   */
  virtual ~BehaviorNode() = default;

  /**
   * @brief Executes the derived node's logic.
   *
   * @return Status indicating the outcome of the behavior.
   */
  virtual Status operator()() = 0;

  /**
   * @brief Returns the node's type.
   *
   * @return const std::string& The node's type.
   */
  const std::string &type() const;

  /**
   * @brief Returns the node's description.
   *
   * @return const std::string& The node's description.
   */
  const std::string &description() const;

  /**
   * @brief Returns the node's child nodes.
   *
   * @return const Children& A vector of child nodes.
   */

  const Children &children() const;

  /**
   * @brief Resets node state to its initial condition, if applicable.
   *
   * This function resets the state of the node and its children, which is
   * particularly useful in behavior trees where nodes may be executed multiple
   * times with stateful behavior.
   */
  virtual void reset();

private:
  /// The node's type.
  std::string type_;
  /// The node's description.
  std::string description_;
protected:
  /// The node's child nodes.
  Children children_;
};

} // namespace evo::behavior
