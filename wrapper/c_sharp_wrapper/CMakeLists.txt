cmake_minimum_required(VERSION 3.2)
file(GLOB_RECURSE SHARP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(mathlibra_sharp SHARED ${SHARP_SRC})
target_link_libraries(mathlibra_sharp core_lib)