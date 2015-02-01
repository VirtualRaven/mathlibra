cmake_minimum_required(VERSION 3.1)
PROJECT(calculator_backend CXX)

add_subdirectory(headers/)
add_subdirectory(source/)
add_library(libcalc STATIC  ${${PROJECT_NAME}_LIB_HEADERS} ${${PROJECT_NAME}_LIB_SOURCES})
TARGET_COMPILE_OPTIONS(libcalc PUBLIC -std=c++11)
TARGET_INCLUDE_DIRECTORIES(libcalc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/headers PUBLIC ${CMAKE_CURRENT_LIST_DIR}/headers/core)

