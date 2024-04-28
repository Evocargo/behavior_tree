#include "../include/behavior_tree/bt_base.h"
#include <gtest/gtest.h>


using namespace ::testing;
using namespace evo::behavior;

// Test the Status class constructors, state checks, and assignment operators
TEST(StatusTest, ConstructorAndState) {
  // Test the constructor that takes a boolean
  Status success_status(true);
  Status failure_status(false);

  ASSERT_TRUE(success_status == Status::SUCCESS);
  ASSERT_TRUE(failure_status == Status::FAILURE);

  // Test the constructor that takes a State
  Status running_status(Status::RUNNING);

  ASSERT_TRUE(running_status == Status::RUNNING);

  // Test returning Status from a function and saving it into a variable before
  // assertion
  auto return_success = []() -> Status { return Status::SUCCESS; };
  auto return_failure = []() -> Status { return Status::FAILURE; };
  auto return_running = []() -> Status { return Status::RUNNING; };

  Status success_result = return_success();
  Status failure_result = return_failure();
  Status running_result = return_running();
  ASSERT_TRUE(success_result == Status::SUCCESS);
  ASSERT_FALSE(success_result == Status::FAILURE);
  ASSERT_FALSE(success_result == Status::RUNNING);

  ASSERT_TRUE(failure_result == Status::FAILURE);
  ASSERT_FALSE(failure_result == Status::SUCCESS);
  ASSERT_FALSE(failure_result == Status::RUNNING);

  ASSERT_TRUE(running_result == Status::RUNNING);
  ASSERT_FALSE(running_result == Status::SUCCESS);
  ASSERT_FALSE(running_result == Status::FAILURE);

  // Test assignment operators
  Status assigned_status(false);
  assigned_status = success_status;
  ASSERT_TRUE(assigned_status == Status::SUCCESS);

  assigned_status = failure_status;
  ASSERT_TRUE(assigned_status == Status::FAILURE);

  assigned_status = running_status;
  ASSERT_TRUE(assigned_status == Status::RUNNING);
}

// Test the Status class conversion to boolean
TEST(StatusTest, ConversionToBoolean) {
  Status success_status(true);
  Status failure_status(false);
  Status running_status(Status::RUNNING);

  ASSERT_TRUE(static_cast<bool>(success_status));
  ASSERT_FALSE(static_cast<bool>(failure_status));
  ASSERT_FALSE(static_cast<bool>(
      running_status)); // Assuming RUNNING should evaluate to false
}

// Test the Status class stream output
TEST(StatusTest, StreamOutput) {
  Status success_status(true);
  Status failure_status(false);
  Status running_status(Status::RUNNING);

  std::stringstream ss_success;
  ss_success << success_status;
  ASSERT_EQ(ss_success.str(), "SUCCESS");

  std::stringstream ss_failure;
  ss_failure << failure_status;
  ASSERT_EQ(ss_failure.str(), "FAILURE");

  std::stringstream ss_running;
  ss_running << running_status;
  ASSERT_EQ(ss_running.str(), "RUNNING");
}
