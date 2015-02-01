add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/core/)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/interface)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/modules)
set(${PROJECT_NAME}_LIB_HEADERS
	  ${${PROJECT_NAME}_LIB_HEADERS}
	  ${CMAKE_CURRENT_LIST_DIR}/exception.h
	  ${CMAKE_CURRENT_LIST_DIR}/main.h
	  ${CMAKE_CURRENT_LIST_DIR}/ptr_protect.h
	  ${CMAKE_CURRENT_LIST_DIR}/stdErr.h
	PARENT_SCOPE
)
	