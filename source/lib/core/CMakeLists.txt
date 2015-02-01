set(${PROJECT_NAME}_LIB_SOURCES
	  ${${PROJECT_NAME}_LIB_SOURCES}
	   ${CMAKE_CURRENT_LIST_DIR}/build.cpp
	   ${CMAKE_CURRENT_LIST_DIR}/corax_virtual_machine.cpp
	   ${CMAKE_CURRENT_LIST_DIR}/interface.cpp
	   ${CMAKE_CURRENT_LIST_DIR}/interpreter.cpp
	   ${CMAKE_CURRENT_LIST_DIR}/mathNode.cpp
	   ${CMAKE_CURRENT_LIST_DIR}/tokens.cpp
	   ${CMAKE_CURRENT_LIST_DIR}/tree.cpp
	
	PARENT_SCOPE
)
	