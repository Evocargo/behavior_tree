cmake_minimum_required(VERSION 3.10)
project(behavior_tree)

if(BUILD_TESTS)
  message(STATUS "Tests building is enabled.")
  include(FetchContent)
  FetchContent_Declare(gtest
    QUIET
    URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz
  )
  # configure build of googletest
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(gtest)

  find_package(GTest CONFIG REQUIRED)
  enable_testing()
endif()

include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

####################################################################
##                   ADD ALL THE SUB-PROJECTS                     ##
####################################################################

add_subdirectory(behavior_tree)
add_subdirectory(evo_behavior_tree)
