add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/core)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/interface)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/modules)
set(${PROJECT_NAME}_LIB_SOURCES
	  ${${PROJECT_NAME}_LIB_SOURCES}
	   ${CMAKE_CURRENT_LIST_DIR}/exception.cpp
	PARENT_SCOPE
)
	