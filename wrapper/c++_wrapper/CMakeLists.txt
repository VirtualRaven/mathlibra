cmake_minimum_required(VERSION 3.2)
file(GLOB_RECURSE CXX_WRP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}})

add_library(mathlibra_CXX ${CXX_LINK_TYPE} ${CXX_WRP_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../common/native_wrapper.cpp  )
target_link_libraries(mathlibra_CXX core_lib)