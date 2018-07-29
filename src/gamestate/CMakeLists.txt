

set(GAMESTATE_H
	${CMAKE_CURRENT_SOURCE_DIR}/game_globals.h
	${CMAKE_CURRENT_SOURCE_DIR}/camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/controls.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects.h
	${CMAKE_CURRENT_SOURCE_DIR}/players.h
	${CMAKE_CURRENT_SOURCE_DIR}/player_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects/units/unit_camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects/objectcontroller.h
	${CMAKE_CURRENT_SOURCE_DIR}/unit_controls.h
	${CMAKE_CURRENT_SOURCE_DIR}/unit_speech.h
	${CMAKE_CURRENT_SOURCE_DIR}/first_divisor.h
	PARENT_SCOPE)

set(GAMESTATE_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/objects.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/objects/objectcontroller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/controls.cpp
	PARENT_SCOPE)
