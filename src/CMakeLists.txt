add_subdirectory(engine)
add_subdirectory(math)
add_subdirectory(gamestate)

add_subdirectory(lua)
add_subdirectory(memory)
add_subdirectory(tags)

set(SRC_CXX
	${CMAKE_CURRENT_SOURCE_DIR}/Direct3D/d3d9hook.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/cseries/random.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Dinput/dinput.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/engine_functions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CurrentEngine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/forge.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/math/math.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/exceptions/exception_handler.cpp
	${LUA_SRC}
	${GAMESTATE_SRC}
	${RENDER_CXX}
	${TAGS_CXX}
	PARENT_SCOPE)
