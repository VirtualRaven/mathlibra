cmake_minimum_required(VERSION 3.0)
file(GLOB_RECURSE CXX_CLIENT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(mathlibra_client ${CXX_CLIENT_SRC})
target_link_libraries(mathlibra_client mathlibra_CXX)
