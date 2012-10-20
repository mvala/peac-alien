project(peac-utils)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

find_package(Threads REQUIRED)
find_package(ALIEN REQUIRED)

configure_file(
  "${PROJECT_SOURCE_DIR}/alien/PeacAliEnConfig.h.in"
  "${PROJECT_BINARY_DIR}/alien/PeacAliEnConfig.h"
)

include_directories("${PROJECT_BINARY_DIR}/alien/")


add_subdirectory(alien)

# uninstall
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
