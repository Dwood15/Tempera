set(LUA_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/script_manager.cpp
	PARENT_SCOPE)

set(LUA_H
	${CMAKE_CURRENT_SOURCE_DIR}/memory_interface.h
	${CMAKE_CURRENT_SOURCE_DIR}/players_interface.h
	${CMAKE_CURRENT_SOURCE_DIR}/script_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate_interface.h
	PARENT_SCOPE)
