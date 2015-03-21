set(${PROJECT_NAME}_LIB_SOURCES
	  ${${PROJECT_NAME}_LIB_SOURCES}
	   ${CMAKE_CURRENT_LIST_DIR}/CoraxVM_interface.cpp
	   ${CMAKE_CURRENT_LIST_DIR}/interpreter_interface.cpp
	PARENT_SCOPE
)
	