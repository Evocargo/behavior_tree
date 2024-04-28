#include <gtest/gtest.h>


#include "../include/behavior_tree/bt_base.h"

using namespace ::testing;
using namespace evo::behavior;
using namespace evo::behavior::bt_factory;

// Testing action nodes to ensure they return appropriate Status values.
TEST(BehaviorTreeTest, Action) {
  auto true_action_node = action([]() {}, "returns Success");
  ASSERT_EQ((*true_action_node)(), Status::Success);
  ASSERT_EQ(true_action_node->type(), "action");
  ASSERT_EQ(true_action_node->description(), "returns Success");
  ASSERT_TRUE(true_action_node->children().empty());
}

// Testing condition nodes to ensure they return appropriate Status values.
TEST(BehaviorTreeTest, Condition) {
  auto true_condition_node =
      condition([]() { return Status::Success; }, "returns Success");
  ASSERT_EQ((*true_condition_node)(), Status::Success);
  ASSERT_EQ(true_condition_node->type(), "condition");
  ASSERT_EQ(true_condition_node->description(), "returns Success");
  ASSERT_TRUE(true_condition_node->children().empty());

  auto false_condition_node =
      condition([]() { return Status::Failure; }, "returns Failure");
  ASSERT_EQ((*false_condition_node)(), Status::Failure);
}

// Testing sequence nodes with clear descriptions and checks on execution.
TEST(BehaviorTreeTest, Sequence) {
  size_t visit_counter = 0;
  auto node = sequence("Sequence Node Description", condition([&visit_counter] {
                         visit_counter++;
                         return Status::Failure;
                       }), // This will stop the sequence
                       condition([&visit_counter] {
                         visit_counter++;
                         return Status::Success;
                       }) // This won't be reached
  );
  ASSERT_EQ(node->description(), "Sequence Node Description");
  ASSERT_EQ(node->children().size(), 2);
  std::cout << "Visit counter " << visit_counter << std::endl;
  ASSERT_EQ((*node)(), Status::Failure);
  ASSERT_EQ(visit_counter, 1); // Only the first node should have been visited
}

// Testing fallback nodes to simulate decision-making branches.
TEST(BehaviorTreeTest, Fallback) {
  size_t visit_counter = 0;
  auto node = fallback("Fallback Node Description", condition([&visit_counter] {
                         visit_counter++;
                         return Status::Failure;
                       }), // First fails
                       condition([&visit_counter] {
                         visit_counter++;
                         return Status::Success;
                       }) // Second succeeds
  );
  ASSERT_EQ(node->description(), "Fallback Node Description");
  ASSERT_EQ(node->children().size(), 2);
  ASSERT_EQ((*node)(), Status::Success);
  ASSERT_EQ(visit_counter, 2); // Both nodes should have been visited
}

// Testing parallel nodes to simulate concurrent checks.
TEST(BehaviorTreeTest, Parallel) {
  size_t visit_counter = 0;
  auto node = parallel("Parallel Node Description", condition([&visit_counter] {
                         visit_counter++;
                         return Status::Failure;
                       }), // Both will run
                       condition([&visit_counter] {
                         visit_counter++;
                         return Status::Running;
                       }) // Simulate ongoing operation
  );
  ASSERT_EQ(node->description(), "Parallel Node Description");
  ASSERT_EQ(node->children().size(), 2);
  ASSERT_EQ((*node)(), Status::Running); // Running takes precedence
  ASSERT_EQ(visit_counter, 2);           // Both nodes should have been visited
}
