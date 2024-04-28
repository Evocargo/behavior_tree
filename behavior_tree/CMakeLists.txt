cmake_minimum_required(VERSION 3.10)
file(STRINGS VERSION CURRENT_VERSION)
project(behavior_tree VERSION ${CURRENT_VERSION})


file(GLOB_RECURSE SOURCES_CPP_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SOURCES_C_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

list(APPEND SOURCES_LIBRARY ${SOURCES_C_LIBRARY})
list(APPEND SOURCES_LIBRARY ${SOURCES_CPP_LIBRARY})

add_library(${PROJECT_NAME} SHARED ${SOURCES_LIBRARY})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include/evocargo>
)

####################################################################
##                        INSTALL LIBRARY                         ##
####################################################################

string(TOUPPER ${PROJECT_NAME} COMPONENT_NAME)
string(REPLACE "_" "" COMPONENT_NAME ${COMPONENT_NAME})

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib/evocargo
          COMPONENT ${COMPONENT_NAME}
  RUNTIME DESTINATION bin
          COMPONENT ${COMPONENT_NAME}
)

install(
  DIRECTORY include/${PROJECT_NAME}
  DESTINATION include/evocargo
  COMPONENT ${COMPONENT_NAME}
)

####################################################################
##                CREATE / INSTALL CMAKE CONFIGS                  ##
####################################################################

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/conf/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
  COMPONENT ${COMPONENT_NAME}
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
  COMPONENT ${COMPONENT_NAME}
)

####################################################################
##                     CPACK CONFIGURATION                        ##
####################################################################

set(CPACK_GENERATOR "DEB")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

string(REPLACE "_" "-" DASH_PROJECT_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_NAME ${DASH_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

set(CPACK_COMPONENTS_ALL ${COMPONENT_NAME})
set(CPACK_DEBIAN_${COMPONENT_NAME}_PACKAGE_NAME ${DASH_PROJECT_NAME})

set(CPACK_DEBIAN_PACKAGE_MAINTAINER
    "Evgeniy Safronov <evgeniy.safronov@evocargo.com>"
)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Behavior Tree framework."
)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

set(CPACK_OUTPUT_CONFIG_FILE
    "${CMAKE_BINARY_DIR}/configs/${PROJECT_NAME}_Config.cmake"
)

include(CPack)

####################################################################
##                  ASSEMBLE LIBRARY WITH TESTS                   ##
####################################################################

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
