#include "../include/behavior_tree/bt_base.h"
#include <gmock/gmock.h>

using namespace ::testing;
using namespace evo::behavior;
using namespace evo::behavior::bt_factory;

// Test the behavior of the SequenceMemory node when conditions are initially
// not met
TEST(BehaviorTreeTest, SequenceMemory) {
  // Create a node that always succeeds
  auto true_action_node = action([]() {}, "returns Success");

  // Setup a condition node that initially returns Running to simulate ongoing
  // operations
  bool flag = false;
  auto condition_node =
      condition([&flag] { return flag ? Status::SUCCESS : Status::RUNNING; });

  // Create a sequence memory node with both nodes
  auto sequence_memory_node =
      sequence_memory("", condition_node, true_action_node);

  // Initially, the condition node is not satisfied, so the sequence should
  // return Running
  ASSERT_EQ(Status::State((*sequence_memory_node)()), Status::RUNNING);

  // Change condition to true, expecting sequence to succeed now
  flag = true;
  Status result = (*sequence_memory_node)();
  ASSERT_EQ(Status::State(result), Status::SUCCESS);
}

// Test the behavior of the FallbackMemory node when the primary condition fails
TEST(BehaviorTreeTest, FallbackMemory) {
  // Create a node that always returns Success
  auto true_action_node = action([]() {}, "returns Success");

  // Setup a condition node that initially returns Failure when the flag is
  // true, and Running otherwise
  bool flag = false;
  auto condition_node =
      condition([&flag] { return flag ? Status::FAILURE : Status::RUNNING; });

  // Create a fallback memory node that tries the condition node first, then the
  // true action node
  auto fallback_memory_node =
      fallback_memory("", condition_node, true_action_node);
  // Initially, the condition node is running, so the fallback should also
  // return Running
  ASSERT_EQ((*fallback_memory_node)(), Status::RUNNING);

  // Change condition to true, then the condition node will fail and fallback to
  // the true action node
  flag = true;
  Status result = (*fallback_memory_node)();
  ASSERT_EQ(result, Status::SUCCESS);
}

// Test the reset behavior of the SequenceMemory node when nodes initially
// return Running
TEST(BehaviorTreeTest, SequenceMemoryReset) {

  // Create a node that always returns Success
  auto true_action_node = action([]() {}, "returns Success");

  // Setup two condition nodes that depend on flags, initially both returning
  // Running
  bool flag1 = false, flag2 = false;
  auto condition_node1 =
      condition([&flag1] { return flag1 ? Status::Success : Status::Running; });
  auto condition_node2 =
      condition([&flag2] { return flag2 ? Status::Success : Status::Running; });

  // Create a sequence memory node that includes both condition nodes and the
  // true action node
  auto sequence_memory_node =
      sequence_memory("", condition_node1, condition_node2, true_action_node);

  // Initially, neither condition is met, so the sequence should return Running
  ASSERT_EQ((*sequence_memory_node)(), Status::Running);

  // Set the first flag to true; the sequence should still return Running
  // because the second flag is false
  flag1 = true;
  ASSERT_EQ((*sequence_memory_node)(), Status::Running);

  // Reset the first flag and set the second flag; the sequence should now
  // succeed
  flag1 = false;
  flag2 = true;
  Status result = (*sequence_memory_node)();
  ASSERT_EQ(result, Status::Success);

  // Reset the sequence memory node, which should revert it to initial
  // conditions
  sequence_memory_node->reset();
  ASSERT_EQ((*sequence_memory_node)(), Status::Running);
}

// Test the behavior of the SequenceMemory node with multiple action nodes
TEST(BehaviorTreeTest, SequenceMemoryMultipleActions) {
  // Create three action nodes, two that always succeed and one that always
  // fails
  auto success_action_node1 = action([]() {}, "Action 1 Success");
  auto success_action_node2 = action([]() {}, "Action 2 Success");
  auto fail_condition_node =
      condition([]() { return Status::Failure; }, "Condition Fail");

  // Create a sequence memory node with the action nodes
  auto sequence_memory_node = sequence_memory(
      "", success_action_node1, fail_condition_node, success_action_node2);

  // The sequence should fail because the condition node fails
  ASSERT_EQ((*sequence_memory_node)(), Status::Failure);
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

// Test the behavior of the SequenceMemory node when all conditions are met from
// the start
TEST(BehaviorTreeTest, SequenceMemoryAllConditionsMet) {
  // Create a node that always succeeds
  auto true_action_node = action([]() {}, "returns Success");

  // Setup a condition node that always returns Success
  auto condition_node = condition([]() { return Status::Success; });

  // Create a sequence memory node with the condition node and the action node
  auto sequence_memory_node =
      sequence_memory("", condition_node, true_action_node);

  // Since the condition is met from the start, the sequence should return
  // Success
  ASSERT_EQ((*sequence_memory_node)(), Status::Success);
}
