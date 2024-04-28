#include "../include/behavior_tree/bt_base.h"
#include <gmock/gmock.h>

using namespace ::testing;
using namespace evo::behavior;
using namespace evo::behavior::bt_factory;

TEST(BehaviorTreeTest, ActionNodeSuccess) {
  bool action_executed = false;
  auto action_node =
      action([&]() { action_executed = true; }, "Test Action Success");

  ASSERT_EQ((*action_node)(), Status::Success);
  ASSERT_TRUE(action_executed);
}

TEST(BehaviorTreeTest, ActionNodeFailure) {
  auto action_node2 =
      action([]() { throw std::runtime_error("Intentional Failure"); },
             "Test Action Failure");

  ASSERT_EQ((*action_node2)(), Status::Failure);
}

TEST(BehaviorTreeTest, ConditionNodeSuccess) {
  auto condition_node =
      condition([]() { return Status::Success; }, "Test Condition Success");

  ASSERT_EQ((*condition_node)(), Status::Success);
}

TEST(BehaviorTreeTest, ConditionNodeFailure) {
  auto condition_node =
      condition([]() { return Status::Failure; }, "Test Condition Failure");

  ASSERT_EQ((*condition_node)(), Status::Failure);
}

TEST(BehaviorTreeTest, ConditionNodeException) {
  auto condition_node = condition(
      []() -> Status { throw std::runtime_error("Condition Exception"); },
      "Test Condition Exception");

  ASSERT_EQ((*condition_node)(), Status::Failure);
}
