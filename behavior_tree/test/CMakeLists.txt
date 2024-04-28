####################################################################
##                    ASSEMBLE ALL THE TESTS                      ##

include(GoogleTest)

set(TEST_PROJECT ${PROJECT_NAME}_test)

file(GLOB_RECURSE SOURCES_CPP_TEST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(
  ${TEST_PROJECT}
  ${SOURCES_CPP_TEST}
)


target_include_directories(
  ${TEST_PROJECT}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)


target_link_libraries(
  ${TEST_PROJECT}
  PRIVATE
    ${PROJECT_NAME}
  PUBLIC
    GTest::gtest
    GTest::gmock
)

message(STATUS "Discovering tests.")
gtest_discover_tests(${TEST_PROJECT})
