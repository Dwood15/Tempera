add_subdirectory(units)

set(GAMESTATE_H
	${CMAKE_CURRENT_SOURCE_DIR}/game_globals.h
	${CMAKE_CURRENT_SOURCE_DIR}/camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/controls.h
	${CMAKE_CURRENT_SOURCE_DIR}/first_divisor.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects.h
	${CMAKE_CURRENT_SOURCE_DIR}/players.h
	${CMAKE_CURRENT_SOURCE_DIR}/player_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects/units/unit_camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects/objectcontroller.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects/weapons.h
	PARENT_SCOPE)

set(GAMESTATE_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/objects.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/objects/objectcontroller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/controls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/players.cpp
	${UNITS_SRC}
	PARENT_SCOPE)

add_custom_target("gamestate" SOURCES ${GAMESTATE_H})
