set(D3D
	src/Direct3D/d3d9hook.h
	src/Direct3D/textures.h
	)

set(DINPUT_H
	${CMAKE_CURRENT_SOURCE_DIR}/Dinput/dinput.h
	)

set(DINPUT_CXX
	${CMAKE_CURRENT_SOURCE_DIR}/Dinput/dinput.cpp
	)

add_subdirectory(math)
add_subdirectory(cache)
add_subdirectory(cseries)
add_subdirectory(effects)
add_subdirectory(exceptions)
add_subdirectory(lua)
add_subdirectory(tags)
add_subdirectory(memory)
add_subdirectory(gamestate)
add_subdirectory(render)

set(SRC_CXX
	src/Direct3D/d3d9hook.cpp
	${DINPUT_CXX}
	${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CurrentEngine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/math/math.cpp
	${EXCEPTIONS_SRC}
	${LUA_SRC}
	${GAMESTATE_SRC}
	PARENT_SCOPE)

set(SRC_H
	${CACHE_H}
	${CSERIES_H}
	${EFFECTS_H}
	${MATH_H}
	${MEMORY_H}
	${RENDER_H}
	${D3D}
	${DINPUT_H}
	${LUA_H}
	${CMAKE_CURRENT_SOURCE_DIR}/core.h
	${CMAKE_CURRENT_SOURCE_DIR}/CurrentEngine.h
	${CMAKE_CURRENT_SOURCE_DIR}/ce_base_types.h
	${TAGS_H}
	${GAMESTATE_H}
	${EXCEPTIONS_H}
	PARENT_SCOPE)
