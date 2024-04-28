#include "../include/behavior_tree/bt_base.h"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace evo::behavior;
using namespace evo::behavior::bt_factory;

class ApproachObject : public BehaviorNode {
public:
  ApproachObject(const std::string& description) : BehaviorNode("Action", description) {}

  void approach() {
    std::cout << "Approaching object..." << std::endl;
  }

  Status operator()() override {
    try {
      approach();
      return Status::Success;
    } catch (const std::exception& e) {
      std::cerr << "Failed to approach object: " << e.what() << std::endl;
      return Status::Failure;
    }
  }
};

// Factory function for creating an ApproachObject node
std::shared_ptr<BehaviorNode> approach_object(const std::string& description = "") {
  return std::make_shared<ApproachObject>(description);
}


class GripperControl {
public:
  GripperControl() : battery_level(100), gripper_open(false) {
    constructBehaviorTree();
  }

  void setBatteryLevel(int level) {
    battery_level = level;
  }

  int getBatteryLevel() const {
    return battery_level;
  }

  bool checkBattery() {
    return battery_level > 20; // Assume battery is ok if above 20%
  }

  Status openGripper() {
    if (checkBattery()) {
      gripper_open = true;
      battery_level--;
      return Status::Success;
    }
    return Status::Failure;
  }

  Status closeGripper() {
    if (checkBattery()) {
      gripper_open = false;
      battery_level--;
      return Status::Success;
    }
    return Status::Failure;
  }

  void run() { bt.run(); }

protected:
  int battery_level;
  bool gripper_open;

  void constructBehaviorTree() {
    auto check_battery = behavior([this]() { return checkBattery(); }, "Check Battery");
    auto open_gripper = behavior([this]() { return openGripper(); }, "Open Gripper");
    auto close_gripper = behavior([this]() { return closeGripper(); }, "Close Gripper");

    // clang-format off
    auto root = 
      sequence(
        check_battery,
        open_gripper,
        approach_object(),
        close_gripper
      );
    // clang-format on
    bt.set_root(root);
  }

  BehaviorTree bt;
};

TEST(GripperControlTest, BatteryCheck) {
  GripperControl gripper_control;
  gripper_control.setBatteryLevel(30);
  ASSERT_TRUE(gripper_control.checkBattery());
}

TEST(GripperControlTest, OpenGripperSuccess) {
  GripperControl gripper_control;
  gripper_control.setBatteryLevel(50);
  ASSERT_EQ(gripper_control.openGripper(), Status::Success);
}

TEST(GripperControlTest, CloseGripperFailure) {
  GripperControl gripper_control;
  gripper_control.setBatteryLevel(10);
  ASSERT_EQ(gripper_control.closeGripper(), Status::Failure);
}

TEST(GripperControlTest, BatteryLevelAfterMultipleOperations) {
  GripperControl gripper_control;
  gripper_control.setBatteryLevel(25);
  gripper_control.run(); // Should open and close gripper, reducing battery by 2
  ASSERT_EQ(gripper_control.getBatteryLevel(), 23);
  gripper_control.run(); // Should open and close gripper, reducing battery by 2
  ASSERT_EQ(gripper_control.getBatteryLevel(), 21);
  gripper_control.run(); // Should fail to open and close gripper due to low battery
  ASSERT_EQ(gripper_control.getBatteryLevel(), 20); // Battery level should remain the same
}

