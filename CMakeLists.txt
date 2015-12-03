cmake_minimum_required(VERSION 2.8)
project(mathlibra CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
include(GetGitRevisionDescription)
git_describe(GIT_VERSION)
string(REGEX MATCH "^([^-])+" VERSION_NAME ${GIT_VERSION})
string(REGEX MATCH "([^-])+$" VERSION_SHA ${GIT_VERSION})
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  if (CMAKE_BUILD_TYPE EQUAL "Debug")
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -Wno-long-long -pedantic")
  else()
	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -Wall -std=c++11 -Wno-long-long -pedantic")
  endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(C_API_TEST CACHE BOOL "Enable cmake testing of the C api")
set(NON_STRICT_PARANTHESES CACHE BOOL "Skip check for unbalanced parantheses")
if(${NON_STRICT_PARANTHESES})
	add_definitions(-DNON_STRICT_PARANTHESES)	
endif()
if(${C_API_TEST})
	enable_testing()
endif()
set(PLUGIN_API_INCLUDE ${PROJECT_BINARY_DIR}/plugin_include)
file(MAKE_DIRECTORY ${PLUGIN_API_INCLUDE})
if (CMAKE_BUILD_TYPE EQUAL "Debug")
    add_definitions(-DDEBUG)
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build.txt.in ${PROJECT_BINARY_DIR}/build.txt )

set(CXX_LINK_TYPE SHARED)

set(C_API True)
set(PY_WRAPPER True)

if(WIN32)
	set(CXX_LINK_TYPE STATIC)
	add_definitions(-DWINDOWS)
elseif(UNIX)
	add_definitions(-DLINUX)
endif()



#Core library
add_subdirectory(core)

#Build the tester executable
add_subdirectory(tester)

#Interfaces and language bindings for core
add_subdirectory(wrapper)
