#include "../include/behavior_tree/bt_base.h"
#include <gtest/gtest.h>


using namespace ::testing;
using namespace evo::behavior;
using namespace evo::behavior::bt_factory;


// Test the behavior of the FallbackMemory node when conditions are initially
// not met
TEST(BehaviorTreeTest, FallbackMemory) {
  // Create a node that always succeeds
  auto true_action_node = action([]() {}, "returns Success");

  // Setup a condition node that initially returns Running to simulate ongoing
  // operations
  bool flag = false;
  auto condition_node =
      condition([&flag] { return flag ? Status::FAILURE : Status::RUNNING; });

  // Create a fallback memory node with both nodes
  auto fallback_memory_node =
      fallback_memory("", condition_node, true_action_node);

  // Initially, the condition node is not satisfied, so the fallback should
  // return Running
  ASSERT_EQ(Status::State((*fallback_memory_node)()), Status::RUNNING);

  // Change condition to true, expecting fallback to succeed now
  flag = true;
  Status result = (*fallback_memory_node)();
  ASSERT_EQ(Status::State(result), Status::SUCCESS);
}
// Test the behavior of the FallbackMemory node when conditions are initially met
TEST(BehaviorTreeTest, FallbackMemoryConditionsMet) {
  // Create a node that always succeeds
  auto true_action_node = action([]() {}, "returns Success");

  // Setup three condition nodes that initially return Success, Running, and Failure
  bool flag1 = false;
  bool flag2 = true;
  bool flag3 = false;
  auto condition_node1 =
      condition([&flag1] { return flag1 ? Status::FAILURE : Status::RUNNING; });
  auto condition_node2 =
      condition([&flag2] { return flag2 ? Status::FAILURE : Status::RUNNING; });
  auto condition_node3 =
      condition([&flag3] { return flag3 ? Status::SUCCESS : Status::FAILURE; });

  // Create a fallback memory node with all three nodes
  auto fallback_memory_node =
      fallback_memory("", condition_node1, condition_node2, condition_node3);

  // Initially, the condition nodes are satisfied, so the fallback should
  // return Success
  ASSERT_EQ(Status::State((*fallback_memory_node)()), Status::RUNNING);

  flag1 = true;
  flag2 = false;
  flag3 = false;

  ASSERT_EQ(Status::State((*fallback_memory_node)()), Status::RUNNING);

  flag1 = true;
  flag2 = true;
  flag3 = false;

  ASSERT_EQ(Status::State((*fallback_memory_node)()), Status::FAILURE);
  ASSERT_EQ(Status::State((*fallback_memory_node)()), Status::FAILURE);
  ASSERT_EQ(Status::State((*fallback_memory_node)()), Status::FAILURE);

}


// Test the behavior of the FallbackMemory node with multiple action nodes
TEST(BehaviorTreeTest, FallbackMemoryMultipleActions) {
  // Create three action nodes, two that always fail and one that always
  // succeeds
  auto fail_condition_node1 =
      condition([]() { return Status::Failure; }, "Condition 1 Fail");
  auto fail_condition_node2 =
      condition([]() { return Status::Failure; }, "Condition 2 Fail");
  auto success_action_node = action([]() {}, "Action Success");

  // Create a fallback memory node with the condition nodes and the action node
  auto fallback_memory_node = fallback_memory(
      "", fail_condition_node1, fail_condition_node2, success_action_node);

  // The fallback should succeed because the last action node succeeds
  ASSERT_EQ((*fallback_memory_node)(), Status::Success);
}
