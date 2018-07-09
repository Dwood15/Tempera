
set(GAME_TIME_H
	${CMAKE_CURRENT_SOURCE_DIR}/game_time/gametime.h
	)

set(GAMESTATE_H
	${GAME_TIME_H}
	${CMAKE_CURRENT_SOURCE_DIR}/game_globals.h
	${CMAKE_CURRENT_SOURCE_DIR}/camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/controls.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects/objects_enums.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects.h
	${CMAKE_CURRENT_SOURCE_DIR}/objectcontroller.h
	${CMAKE_CURRENT_SOURCE_DIR}/players.h
	${CMAKE_CURRENT_SOURCE_DIR}/player_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects/units/unit_camera.h
	PARENT_SCOPE)

set(GAME_TIME_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/game_time/gametime.cpp
	)

set(GAMESTATE_SRC
	${GAME_TIME_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/objects.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/objectcontroller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/controls.cpp
	PARENT_SCOPE)
