cmake_minimum_required(VERSION 3.2)
file(GLOB_RECURSE CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
include_directories(${CMAKE_SOURCE_DIR}/core)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/GIT_VERSION.h.in ${PROJECT_BINARY_DIR}/GIT_VERSION.h )

add_library(core_lib STATIC ${CORE_SRC})