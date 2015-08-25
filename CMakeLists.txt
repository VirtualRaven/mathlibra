cmake_minimum_required(VERSION 3.0)
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -Wno-long-long -pedantic")
endif()

set(CXX_LINK_TYPE SHARED)

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
