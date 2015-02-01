
set(${PROJECT_NAME}_LIB_HEADERS
	  ${${PROJECT_NAME}_LIB_HEADERS}
	  ${CMAKE_CURRENT_LIST_DIR}/CoraxVM_interface.h
	  ${CMAKE_CURRENT_LIST_DIR}/interface.h
	  ${CMAKE_CURRENT_LIST_DIR}/interpreter_interface.h
	PARENT_SCOPE
)
	