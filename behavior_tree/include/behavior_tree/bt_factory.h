#pragma once

#include "nodes/action.h"
#include "nodes/condition.h"
#include "nodes/decorators/not.h"
#include "nodes/fallback.h"
#include "nodes/fallback_memory.h"
#include "nodes/if_then.h"
#include "nodes/if_then_else.h"
#include "nodes/latch.h"
#include "nodes/parallel.h"
#include "nodes/sequence.h"
#include "nodes/sequence_memory.h"
#include "nodes/skipper.h"
#include "nodes/try_else.h"

/**
 * @brief This namespace contains functions for creating all types of behavior
 * tree nodes.
 *
 */

namespace evo::behavior::bt_factory {

/**
 * @brief Function overload that takes all the arguments as BehaviorPtr.  Only
 * for use within this namespace.
 *
 * @tparam Args BehaviorPtr.
 * @param first_The first child node.
 * @param args Other child nodes.
 * @return auto A pair: parent node description - child nodes.
 */
template <class... Args>
[[nodiscard]] auto make_children_array(BehaviorPtr first_behavior,
                                       Args... args) {
  return std::make_pair("", BehaviorNode::Children{first_behavior, args...});
}

/**
 * @brief Function overload that takes the first argument as a description and
 * the other arguments as BehaviorPtr. Only for use within this namespace.
 *
 * @tparam Args BehaviorPtr
 * @param description A text description of a parent node.
 * @param args Child nodes.
 * @return auto A pair: parent node description - child nodes.
 */
template <class... Args>
[[nodiscard]] auto make_children_array(const std::string &description,
                                       Args... args) {
  return std::make_pair(description, BehaviorNode::Children{args...});
}

/**
 * @brief Creates an action node.
 *
 * @param behavior An action the node should execute.
 * @param description A text description.
 * @return BehaviorPtr An action node.
 */
[[nodiscard]] inline BehaviorPtr action(std::function<void()> behavior,
                                        std::string const &description = "") {
  return std::make_shared<Action>(behavior, description);
}

/**
 * @brief Creates a condition node with a Status-returning behavior.
 *
 * @param behavior A condition the node should check, returning a Status.
 * @param description A text description.
 * @return BehaviorPtr A condition node.
 */
[[nodiscard]] inline BehaviorPtr
condition(std::function<Status()> behavior,
          std::string const &description = "") {
  return std::make_shared<Condition>(behavior, description);
}

/**
 * @brief Creates a behavior node with a Status-returning behavior.
 *
 * @param behavior A behavior the node should execute, returning a Status.
 * @param description A text description.
 * @return BehaviorPtr A behavior node.
 */
[[nodiscard]] inline BehaviorPtr
behavior(std::function<Status()> behavior,
         std::string const &description = "") {
  return std::make_shared<Condition>(behavior, description);
}
/**
 * @brief Creates a sequence node.
 *
 * @tparam Args BehaviorPtr.
 * @param args A description and/or child nodes.
 * @return BehaviorPtr A sequence node.
 */
template <class... Args>
[[nodiscard]] BehaviorPtr sequence(Args... args) {
  auto [description, children] = make_children_array(args...);
  return std::make_shared<Sequence>(description, children);
}

/**
 * @brief Creates a fallback node.
 *
 * @tparam Args BehaviorPtr.
 * @param args A description and/or child nodes.
 * @return BehaviorPtr A fallback node.
 */
template <class... Args>
[[nodiscard]] BehaviorPtr fallback(Args... args) {
  auto [description, children] = make_children_array(args...);
  return std::make_shared<Fallback>(description, children);
}

/**
 * @brief Creates a sequence with memory node.
 *
 * @tparam Args BehaviorPtr.
 * @param args A description and/or child nodes.
 * @return BehaviorPtr A sequence with memory node.
 */
template <class... Args>
[[nodiscard]] BehaviorPtr sequence_memory(Args... args) {
  auto [description, children] = make_children_array(args...);
  return std::make_shared<SequenceMemory>(description, children);
}

/**
 * @brief Creates a fallback with memory node.
 *
 * @tparam Args BehaviorPtr.
 * @param args A description and/or child nodes.
 * @return BehaviorPtr A fallback with memory node.
 */
template <class... Args>
[[nodiscard]] BehaviorPtr fallback_memory(Args... args) {
  auto [description, children] = make_children_array(args...);
  return std::make_shared<FallbackMemory>(description, children);
}

/**
 * @brief Creates a skipper node.
 *
 * @tparam Args BehaviorPtr.
 * @param args A description and/or child nodes.
 * @return BehaviorPtr A skipper node.
 */
template <class... Args>
[[nodiscard]] BehaviorPtr skipper(Args... args) {
  auto [description, children] = make_children_array(args...);
  return std::make_shared<Skipper>(description, children);
}

/**
 * @brief Creates a parallel node.
 *
 * @tparam Args BehaviorPtr.
 * @param args A description and/or child nodes.
 * @return BehaviorPtr A parallel node.
 */
template <class... Args>
[[nodiscard]] BehaviorPtr parallel(Args... args) {
  auto [description, children] = make_children_array(args...);
  return std::make_shared<Parallel>(description, children);
}

    /**
     * @brief Creates a latch node and the relevant unlatch one.
     *
     * @param child Latch node's child node.
     * @return std::pair<BehaviorPtr, BehaviorPtr> A pair of latch and unlatch
     * nodes.
     */
        [[nodiscard]] inline std::pair<
            BehaviorPtr, BehaviorPtr> latch_and_unlatch(BehaviorPtr child) {
  auto latch = std::make_shared<Latch>(child, false);
  return {latch, latch->make_unlatcher()};
}

/**
 * @brief Creates a not node.
 *
 * @param child The child node whose result will be inverted.
 * @return BehaviorPtr A not node.
 */
[[nodiscard]] inline BehaviorPtr not_(BehaviorPtr child) {
  return std::make_shared<Not>(child);
}

/**
 * @brief Creates a "try else" node.
 *
 * @param description A text description.
 * @param try_node The node try branch starts with.
 * @param else_node The node else branch starts with.
 * @return BehaviorPtr A "try else" node
 */
[[nodiscard]] inline BehaviorPtr try_else(const std::string &description,
                                          BehaviorPtr try_node,
                                          BehaviorPtr else_node) {
  return std::make_shared<TryElse>(description, try_node, else_node);
}

/**
 * @brief Creates an "if then" node.
 *
 * @param description A text description.
 * @param if_node The node if branch starts with.
 * @param then_node The node then branch stars with.
 * @return BehaviorPtr An "if then" node.
 */
[[nodiscard]] inline BehaviorPtr if_then(const std::string &description,
                                         BehaviorPtr if_node,
                                         BehaviorPtr then_node) {
  return std::make_shared<IfThen>(description, if_node, then_node);
}

/**
 * @brief Creates an "if then else" node.
 *
 * @param description A text description.
 * @param if_node The node if branch starts with.
 * @param then_node The node then branch starts with.
 * @param else_node The node else branch starts with.
 * @return BehaviorPtr An "if then else" node starts with.
 */
[[nodiscard]] inline BehaviorPtr if_then_else(const std::string &description,
                                              BehaviorPtr if_node,
                                              BehaviorPtr then_node,
                                              BehaviorPtr else_node) {
  return std::make_shared<IfThenElse>(description, if_node, then_node,
                                      else_node);
}

} // namespace evo::behavior::bt_factory
