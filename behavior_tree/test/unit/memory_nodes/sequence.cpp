#include "../include/behavior_tree/bt_base.h"
#include <gtest/gtest.h>


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

// Test the behavior of the SequenceMemory node when conditions are initially met
TEST(BehaviorTreeTest, SequenceMemoryConditionsMet) {
  // Create a node that always succeeds
  auto true_action_node = action([]() {}, "returns Success");

  // Setup three condition nodes that initially return Success, Running, and Failure
  bool flag1 = true;
  bool flag2 = false;
  bool flag3 = true;
  auto condition_node1 =
      condition([&flag1] { return flag1 ? Status::SUCCESS : Status::RUNNING; });
  auto condition_node2 =
      condition([&flag2] { return flag2 ? Status::SUCCESS : Status::RUNNING; });
  auto condition_node3 =
      condition([&flag3] { return flag3 ? Status::SUCCESS : Status::FAILURE; });

  // Create a sequence memory node with all three nodes
  auto sequence_memory_node =
      sequence_memory("", condition_node1, condition_node2, condition_node3);

  // Initially, the condition nodes are satisfied, so the sequence should
  // return Success
  ASSERT_EQ(Status::State((*sequence_memory_node)()), Status::RUNNING);

  flag1 = false;
  flag2 = true;
  flag3 = false;

  ASSERT_EQ(Status::State((*sequence_memory_node)()), Status::FAILURE);

  flag1 = false;
  flag2 = true;
  flag3 = true;

  ASSERT_EQ(Status::State((*sequence_memory_node)()), Status::SUCCESS);
  ASSERT_EQ(Status::State((*sequence_memory_node)()), Status::RUNNING);

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
