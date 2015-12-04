cmake_minimum_required(VERSION 2.8)
file(GLOB_RECURSE CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
include_directories(${CMAKE_SOURCE_DIR}/core)
include_directories(${PROJECT_BINARY_DIR})
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/GIT_VERSION.h.in ${PROJECT_BINARY_DIR}/GIT_VERSION.h )
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC" )
endif()
					  
add_library(core_lib STATIC ${CORE_SRC})

if(UNIX)
	target_link_libraries(core_lib -ldl)
endif()


#Copy files needed for the plugin api
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/plugins/api_version.h  ${PLUGIN_API_INCLUDE}/api_version.h COPYONLY )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ptr_protect.h  ${PLUGIN_API_INCLUDE}/ptr_protect.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/main.h ${PLUGIN_API_INCLUDE}/main.h COPYONLY )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core/tree_interface.h  ${PLUGIN_API_INCLUDE}/tree_interface.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core/mathNode_interface.h  ${PLUGIN_API_INCLUDE}/mathNode_interface.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core/object_counter.h  ${PLUGIN_API_INCLUDE}/object_counter.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/modules/function_helper.h  ${PLUGIN_API_INCLUDE}/function_helper.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/plugins/functions_external_interface.h  ${PLUGIN_API_INCLUDE}/functions_external_interface.h COPYONLY)
