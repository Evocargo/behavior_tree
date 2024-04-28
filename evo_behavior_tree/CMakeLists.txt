cmake_minimum_required(VERSION 3.10.0)
file(STRINGS VERSION CURRENT_VERSION)
project(evo_behavior_tree VERSION ${CURRENT_VERSION})
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW) # Suppress GTest warnings.

####################################################################
##              PREPARE MULTIPLE PACKAGES WORKAROUND              ##
####################################################################

string(TOUPPER ${PROJECT_NAME} COMPONENT_NAME)
string(REPLACE "_" "" COMPONENT_NAME ${COMPONENT_NAME})

# Catkin hack to join the cmake config files into the target component
# debian package. Used to rename the
# 'Unspecified' catkin target into the real one
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME ${COMPONENT_NAME})

####################################################################
##                  ADD COMPILE TIME DEFINITIONS                  ##
####################################################################
if(BUILD_TESTS)
  message(STATUS "Tests building is enabled.")
  find_package(GTest CONFIG REQUIRED)
  enable_testing()
endif()

option(ENABLE_LOGGING "Build with logs enabled" ON)

if(ENABLE_LOGGING)
  add_compile_definitions(ALLOW_GENERAL_LOGGING)
  add_compile_definitions(ENABLE_LOGGING)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug"
   AND ENABLE_LOGGING
)
  add_compile_definitions(ALLOW_DEBUG_LOGGING)
endif()

####################################################################
##               FIND ALL PACKAGES & PREPARE CATKIN               ##
####################################################################

find_package(
  catkin REQUIRED
  COMPONENTS
    evo_service_manager
    roscpp
    std_msgs
    rostest
    actionlib_msgs
)
find_package(yaml-cpp REQUIRED)

#catkin_package()
catkin_package(CATKIN_DEPENDS actionlib_msgs)

####################################################################
##       WRAP IMPORTED PACKAGES TO PREVENT WARNINGS LEAKAGE       ##
####################################################################

add_library(catkin_wrapper INTERFACE)
target_include_directories(
  catkin_wrapper SYSTEM
  INTERFACE ${catkin_INCLUDE_DIRS}
)
target_link_libraries(
  catkin_wrapper
  INTERFACE ${catkin_LIBRARIES}
)

####################################################################
##         ASSEMBLE ROS WRAPPER FOR BT AS A STATIC LIBRARY        ##
####################################################################

file(GLOB_RECURSE COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${COMMON_SOURCES})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
    behavior_tree
    yaml-cpp
    catkin_wrapper
)

add_dependencies(${PROJECT_NAME} behavior_tree)

####################################################################
##                        INSTALL LIBRARY                         ##
####################################################################

install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
          COMPONENT ${COMPONENT_NAME}
)

install(
  DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  COMPONENT ${COMPONENT_NAME}
)

####################################################################
##                     CPACK CONFIGURATION                        ##
####################################################################

set(CPACK_GENERATOR "DEB")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/ros/melodic")

string(REPLACE "_" "-" DASH_PROJECT_NAME ${PROJECT_NAME})
set(DASH_PROJECT_NAME ros-melodic-${DASH_PROJECT_NAME})

set(CPACK_PACKAGE_NAME ${DASH_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

set(CPACK_COMPONENTS_ALL ${COMPONENT_NAME})
set(CPACK_DEBIAN_${COMPONENT_NAME}_PACKAGE_NAME ${DASH_PROJECT_NAME})

set(CPACK_DEBIAN_PACKAGE_MAINTAINER
    "Evgeniy Safronov <evgeniy.safronov@evocargo.com>"
)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "ROS-dependent Behavior Tree library wrapper."
)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "behavior-tree")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

set(CPACK_OUTPUT_CONFIG_FILE
    "${CMAKE_BINARY_DIR}/configs/${PROJECT_NAME}_Config.cmake"
)

include(CPack)

