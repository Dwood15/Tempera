
add_subdirectory(objects)
add_subdirectory(units)

set(TEAMS_H
	${CMAKE_CURRENT_SOURCE_DIR}/teams/teams.h
	PARENT_SCOPE)

set(GAMESTATE_H
	${CMAKE_CURRENT_SOURCE_DIR}/camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/controls.h
	${CMAKE_CURRENT_SOURCE_DIR}/first_divisor.h
	${CMAKE_CURRENT_SOURCE_DIR}/game_globals.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects.h
	${CMAKE_CURRENT_SOURCE_DIR}/players.h
	${CMAKE_CURRENT_SOURCE_DIR}/player_types.h
	${G_OBJECTS_H}
	${UNITS_H}
	PARENT_SCOPE)

set(GAMESTATE_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/objects.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/objects/objectcontroller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/controls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/players.cpp
	${UNITS_CXX}
	${G_OBJECTS_CXX}
	PARENT_SCOPE)

add_custom_target("gamestate" SOURCES ${GAMESTATE_H})
