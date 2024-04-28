#include "../include/behavior_tree/bt_base.h"
#include <gtest/gtest.h>
#include <optional>
#include <queue>

using namespace ::testing;
using namespace evo::behavior;
using namespace evo::behavior::bt_factory;

struct Task {
  double x;
  double y;
  double z;
};

class PickAndPlaceArmBehavior : public BehaviorNode {

public:
  bool is_pick_successful;
  bool is_place_successful;
  bool is_gripper_free;
  std::optional<Task> pick;
  std::optional<Task> place;

  virtual void pickObject(const Task &task) = 0;
  virtual void placeObject(const Task &task) = 0;

  PickAndPlaceArmBehavior()
      : BehaviorNode("PickAndPlaceArmBehavior", "Handles the behavior of a robotic arm for picking and placing tasks."),
        is_pick_successful(false), is_place_successful(false),
        is_gripper_free(true), pick(std::nullopt), place(std::nullopt) {
    children_.push_back(make_subtree());
  }

  Status operator()() override {
    return (*children_.front())();
  }

  virtual ~PickAndPlaceArmBehavior() {}

  void setPickTask(const Task &task) {
    pick = task;
    is_pick_successful = false; // Reset pick success state
  }

  void setPlaceTask(const Task &task) {
    place = task;
    is_place_successful = false; // Reset place success state
  }
  void clearPickTask() { pick = std::nullopt; }

  void clearPlaceTask() { place = std::nullopt; }
  
  BehaviorPtr make_subtree() {
    // clang-format off
    
    auto pick_object_action = 
    action(
      [this]() { pickObject(*pick);},
      "Pick Object Action"
    );

    auto is_object_picked = condition([this]() { return is_pick_successful; },
                                      "Check Pick Successful");

    auto place_object_action = 
    action(
      [this]() {
        if (place.has_value()) {
          placeObject(*place);
        }
      },
      "Place Object Action"
    );

    auto is_object_placed = condition([this]() { return is_place_successful; },
                                      "Check Place Successful");

    auto is_gripper_free_condition =
        condition([this]() { return is_gripper_free; }, "Check Gripper Free");
    auto has_pick_task = condition([this]() { return pick.has_value(); },
                                   "Check Pick Task Available");

    auto has_place_task = condition([this]() { return place.has_value(); },
                                    "Check Place Task Available");

    auto clear_pick_task_action =
        action([this]() { clearPickTask(); }, "Clear Pick Task");

    auto clear_place_task_action =
        action([this]() { clearPlaceTask(); }, "Clear Place Task");

    return 
    fallback(
        sequence(
            not_(has_pick_task),
            not_(has_place_task)
        ),
        sequence(
            fallback(
                is_object_picked,
                sequence_memory(
                    has_pick_task,
                    is_gripper_free_condition,
                    pick_object_action,
                    is_object_picked,
                    clear_pick_task_action
                )
            ),
            fallback(
                is_object_placed,
                sequence_memory(
                    has_place_task,
                    place_object_action,
                    is_object_placed,
                    clear_place_task_action
                )
            )
        )
    );
    // clang-format on
  }

};
class PickAndPlaceArmBehaviorMock : public PickAndPlaceArmBehavior {
public:
  PickAndPlaceArmBehaviorMock() {
    is_pick_successful = false;
    is_place_successful = false;
    is_gripper_free = true;
  }
  void pickObject([[maybe_unused]] const Task &task) override {
    is_pick_successful = true; // Simulate successful pick
  }

  void placeObject([[maybe_unused]] const Task &task) override {
    is_place_successful = true; // Simulate successful place
  }
};
// Unit tests for PickAndPlaceArmBehaviorMock
TEST(PickAndPlaceArmBehaviorMockTest, PickObjectSuccess) {
  PickAndPlaceArmBehaviorMock arm;
  Task pick_task{1.0, 2.0, 3.0}; // Coordinates for the task
  arm.setPickTask(pick_task);
  arm.pickObject(*arm.pick);
  ASSERT_TRUE(arm.is_pick_successful);
}

TEST(PickAndPlaceArmBehaviorMockTest, PlaceObjectSuccess) {
  PickAndPlaceArmBehaviorMock arm;
  Task place_task{4.0, 5.0, 6.0}; // Coordinates for the task
  arm.setPlaceTask(place_task);
  arm.placeObject(*arm.place);
  ASSERT_TRUE(arm.is_place_successful);
}

class TwoArmBehaviorsRobot {
public:
  std::shared_ptr<PickAndPlaceArmBehavior> left_arm;
  std::shared_ptr<PickAndPlaceArmBehavior> right_arm;
  std::queue<std::pair<Task, Task>> tasks;

  BehaviorTree bt;

public:
  TwoArmBehaviorsRobot(std::shared_ptr<PickAndPlaceArmBehavior> left,
                       std::shared_ptr<PickAndPlaceArmBehavior> right)
      : left_arm(left), right_arm(right), bt(make_tree()) {}

  void addTask(const Task &pickTask, const Task &placeTask) {
    tasks.push(std::make_pair(pickTask, placeTask));
  }

  void run() {
    bt.run(); // Run the behavior tree
  }

protected:
  BehaviorPtr make_tree() {
    auto check_queue_not_empty = condition([this]() { return !tasks.empty(); },
                                           "Check if Task Queue is Not Empty");
  // clang-format off
   
    auto assign_task_to_arm = [this](std::shared_ptr<PickAndPlaceArmBehavior> arm) {
      return action(
          [this, arm]() {
            const auto &task_pair = tasks.front();
            arm->setPickTask(task_pair.first);
            arm->setPlaceTask(task_pair.second);
            tasks.pop();
          },
          "Assign Task to Arm");
    };

    auto root = 
    parallel(
      sequence(
        left_arm,
        check_queue_not_empty,
        assign_task_to_arm(left_arm)
      ),
      sequence(
        right_arm,
        check_queue_not_empty,
        assign_task_to_arm(right_arm)
      )
    );
    // clang-format on

    return root;
  }
};

TEST(TwoArmBehaviorsRobotTest, OneTask) {
  auto left_arm_mock = std::make_shared<PickAndPlaceArmBehaviorMock>();
  auto right_arm_mock = std::make_shared<PickAndPlaceArmBehaviorMock>();
  TwoArmBehaviorsRobot robot(left_arm_mock, right_arm_mock);

  Task pickTask{1.0, 2.0, 3.0};
  Task placeTask{4.0, 5.0, 6.0};
  robot.addTask(pickTask, placeTask);

  ASSERT_TRUE(left_arm_mock->pick == std::nullopt);
  ASSERT_TRUE(left_arm_mock->place == std::nullopt);
  ASSERT_FALSE(left_arm_mock->is_pick_successful);
  ASSERT_FALSE(left_arm_mock->is_place_successful);
  ASSERT_TRUE(left_arm_mock->is_gripper_free);

  ASSERT_TRUE(right_arm_mock->pick == std::nullopt);
  ASSERT_TRUE(right_arm_mock->place == std::nullopt);
  ASSERT_FALSE(right_arm_mock->is_pick_successful);
  ASSERT_FALSE(right_arm_mock->is_place_successful);
  ASSERT_TRUE(right_arm_mock->is_gripper_free);

  ASSERT_NO_THROW(robot.run());

  // Add checks after verifying that pick is not yet done
  ASSERT_FALSE(left_arm_mock->is_pick_successful);
  ASSERT_FALSE(right_arm_mock->is_pick_successful);

  ASSERT_TRUE(left_arm_mock->pick.has_value());
  ASSERT_TRUE(left_arm_mock->place.has_value());

  ASSERT_NO_THROW(robot.run());

  ASSERT_TRUE(left_arm_mock->is_pick_successful);
  ASSERT_TRUE(left_arm_mock->is_place_successful);
}

TEST(TwoArmBehaviorsRobotTest, TwoTasks) {
  auto left_arm_mock = std::make_shared<PickAndPlaceArmBehaviorMock>();
  auto right_arm_mock = std::make_shared<PickAndPlaceArmBehaviorMock>();
  TwoArmBehaviorsRobot robot(left_arm_mock, right_arm_mock);

  Task pickTask1{1.0, 2.0, 3.0};
  Task placeTask1{4.0, 5.0, 6.0};
  Task pickTask2{7.0, 8.0, 9.0};
  Task placeTask2{10.0, 11.0, 12.0};
  robot.addTask(pickTask1, placeTask1);
  robot.addTask(pickTask2, placeTask2);

  ASSERT_TRUE(left_arm_mock->pick == std::nullopt);
  ASSERT_TRUE(left_arm_mock->place == std::nullopt);
  ASSERT_FALSE(left_arm_mock->is_pick_successful);
  ASSERT_FALSE(left_arm_mock->is_place_successful);
  ASSERT_TRUE(left_arm_mock->is_gripper_free);

  ASSERT_TRUE(right_arm_mock->pick == std::nullopt);
  ASSERT_TRUE(right_arm_mock->place == std::nullopt);
  ASSERT_FALSE(right_arm_mock->is_pick_successful);
  ASSERT_FALSE(right_arm_mock->is_place_successful);
  ASSERT_TRUE(right_arm_mock->is_gripper_free);

  ASSERT_NO_THROW(robot.run());

  // Check that each arm has picked up one task
  ASSERT_TRUE(left_arm_mock->pick.has_value());
  ASSERT_TRUE(right_arm_mock->pick.has_value());

  ASSERT_NO_THROW(robot.run());

  // Check that each arm has completed its task
  ASSERT_TRUE(left_arm_mock->is_pick_successful);
  ASSERT_TRUE(left_arm_mock->is_place_successful);
  ASSERT_TRUE(right_arm_mock->is_pick_successful);
  ASSERT_TRUE(right_arm_mock->is_place_successful);
}
