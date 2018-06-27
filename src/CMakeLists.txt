set(D3D
	src/Direct3D/d3d9hook.h
	src/Direct3D/textures.h
	)

add_subdirectory(math)
add_subdirectory(exceptions)
add_subdirectory(lua)
add_subdirectory(gamestate)

set(SRC_CXX
	src/Direct3D/d3d9hook.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/math/math.cpp
	${EXCEPTIONS_SRC}
	${LUA_SRC}
	${GAMESTATE_SRC}
	PARENT_SCOPE)

set(SRC_H
	${MATH_H}
	${D3D}
	${LUA_H}
	${CMAKE_CURRENT_SOURCE_DIR}/core.h
	${CMAKE_CURRENT_SOURCE_DIR}/ce_base_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/tags/tags.h
	${CMAKE_CURRENT_SOURCE_DIR}/tags/map.h
	${GAMESTATE_H}
	${EXCEPTIONS_H}
	PARENT_SCOPE)
