#include <gtest/gtest.h>
int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
