#include "../include/behavior_tree/bt_base.h"
#include <gmock/gmock.h>

using namespace ::testing;
using namespace evo::behavior;
using namespace evo::behavior::bt_factory;

class PickAndPlace {
public:
  PickAndPlace()
      : is_pick_successful(false), is_place_successful(false),
        is_robot_initialized(false), pick_position_x(0.0), pick_position_y(0.0),
        pick_position_z(0.0), place_position_x(0.0), place_position_y(0.0),
        place_position_z(0.0) {
    constructBehaviorTree();
  }

  void setPickPosition(double x, double y, double z) {
    pick_position_x = x;
    pick_position_y = y;
    pick_position_z = z;
  }

  void setPlacePosition(double x, double y, double z) {
    place_position_x = x;
    place_position_y = y;
    place_position_z = z;
  }

  void setPickSuccess(bool successful) { is_pick_successful = successful; }

  void setPlaceSuccess(bool successful) { is_place_successful = successful; }

  void setRobotInitialization(bool initialized) {
    is_robot_initialized = initialized;
  }

  void run() { bt.run(); }

  virtual void initializeRobot() = 0;
  virtual void pickObject(double x, double y, double z) = 0;
  virtual void placeObject(double x, double y, double z) = 0;
  virtual void resetRobot(){};

  bool is_pick_successful;
  bool is_place_successful;
  bool is_robot_initialized;

  virtual ~PickAndPlace() {
    resetRobot(); // Ensure the robot is reset when the State object is
                  // destroyed
  }

protected:
  double pick_position_x;
  double pick_position_y;
  double pick_position_z;
  double place_position_x;
  double place_position_y;
  double place_position_z;

  void constructBehaviorTree() {
    auto init_robot_action =
        action([this]() { initializeRobot(); }, "Initialize Robot Action");

    auto is_robot_inited = condition([this]() { return is_robot_initialized; },
                                     "Check Robot Initialized");

    auto pick_object_action = action(
        [this]() {
          pickObject(pick_position_x, pick_position_y, pick_position_z);
        },
        "Pick Object Action");

    auto is_object_picked = condition([this]() { return is_pick_successful; },
                                      "Check Pick Successful");

    auto place_object_action = action(
        [this]() {
          placeObject(place_position_x, place_position_y, place_position_z);
        },
        "Place Object Action");

    auto is_object_placed = condition([this]() { return is_place_successful; },
                                      "Check Place Successful");

    // clang-format off
        auto root = 
        sequence(
            fallback(
                is_robot_inited,
                init_robot_action
            ),
            sequence_memory(
                fallback(
                    is_object_picked,
                    sequence(
                        pick_object_action,
                        is_object_picked
                    )
                ),
                fallback(
                    is_object_placed,
                    sequence(
                        place_object_action,
                        is_object_placed
                    )
                )
            )
        );
    // clang-format on
    bt.set_root(root);
  }

  BehaviorTree bt;
};
class PickAndPlaceMock : public PickAndPlace {
public:
  PickAndPlaceMock() {
    is_robot_initialized = false;
    is_pick_successful = false;
    is_place_successful = false;
  }

  void initializeRobot() override { is_robot_initialized = true; }

  void pickObject(double x, double y, double z) override {
    if (x != 0 && y != 0 && z != 0) { // Dummy condition to simulate success
      is_pick_successful = true;
    }
  }

  void placeObject(double x, double y, double z) override {
    if (x != 0 && y != 0 && z != 0) { // Dummy condition to simulate success
      is_place_successful = true;
    }
  }

  void resetRobot() {}
};

TEST(PickAndPlaceMockTest, InitializeRobot) {
  PickAndPlaceMock pick_and_place;
  pick_and_place.initializeRobot();
  ASSERT_TRUE(pick_and_place.is_robot_initialized);
}

TEST(PickAndPlaceMockTest, PickObjectSuccess) {
  PickAndPlaceMock pick_and_place;
  pick_and_place.pickObject(1.0, 1.0, 1.0); // Coordinates that simulate success
  ASSERT_TRUE(pick_and_place.is_pick_successful);
}

TEST(PickAndPlaceMockTest, PickObjectFailure) {
  PickAndPlaceMock pick_and_place;
  pick_and_place.pickObject(0, 0, 0); // Coordinates that simulate failure
  ASSERT_FALSE(pick_and_place.is_pick_successful);
}

TEST(PickAndPlaceMockTest, PlaceObjectSuccess) {
  PickAndPlaceMock pick_and_place;
  pick_and_place.placeObject(1.0, 1.0,
                             1.0); // Coordinates that simulate success
  ASSERT_TRUE(pick_and_place.is_place_successful);
}

TEST(PickAndPlaceMockTest, PlaceObjectFailure) {
  PickAndPlaceMock pick_and_place;
  pick_and_place.placeObject(0, 0, 0); // Coordinates that simulate failure
  ASSERT_FALSE(pick_and_place.is_place_successful);
}

TEST(PickAndPlaceMockTest, Logic1) {
  PickAndPlaceMock pick_and_place;
  ASSERT_FALSE(pick_and_place.is_pick_successful);
  ASSERT_FALSE(pick_and_place.is_place_successful);
  ASSERT_FALSE(pick_and_place.is_robot_initialized);

  pick_and_place.run();

  ASSERT_FALSE(pick_and_place.is_pick_successful);
  ASSERT_FALSE(pick_and_place.is_place_successful);
  ASSERT_TRUE(pick_and_place.is_robot_initialized);

  pick_and_place.run();

  // coords are zero and dummy action does not set success for picking
  ASSERT_FALSE(pick_and_place.is_pick_successful);
  ASSERT_FALSE(pick_and_place.is_place_successful);
  ASSERT_TRUE(pick_and_place.is_robot_initialized);

  pick_and_place.setPickPosition(1.0, 1.0, 1.0);

  pick_and_place.run();

  // picking is ok
  // placing coords are zero and dummy action does not set success for picking
  ASSERT_TRUE(pick_and_place.is_pick_successful);
  ASSERT_FALSE(pick_and_place.is_place_successful);
  ASSERT_TRUE(pick_and_place.is_robot_initialized);

  pick_and_place.setPlacePosition(2.0, 2.0, 2.0);

  pick_and_place.run();

  // picking is ok
  // placing is ok
  ASSERT_TRUE(pick_and_place.is_pick_successful);
  ASSERT_TRUE(pick_and_place.is_place_successful);
  ASSERT_TRUE(pick_and_place.is_robot_initialized);
}
