####################################################################
##                    ASSEMBLE ALL THE TESTS                      ##
include(FetchContent)
FetchContent_Declare(gtest
  QUIET
  URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz
)
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)


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
