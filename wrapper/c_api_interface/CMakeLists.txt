cmake_minimum_required(VERSION 3.0)
file(GLOB_RECURSE C_API_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}})

add_library(mathlibra_C ${CXX_LINK_TYPE} ${C_API_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../common/native_wrapper.cpp  )
target_link_libraries(mathlibra_C core_lib)

add_custom_command(TARGET mathlibra_C
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/mathlibra.py ${CMAKE_BINARY_DIR}/lib/mathlibra.py
       	)


