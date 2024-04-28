cmake_minimum_required(VERSION 3.11)
project(behavior_tree)

if(BUILD_TESTS)
  message(STATUS "Tests building is enabled.")
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
