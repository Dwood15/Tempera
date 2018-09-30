#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/devices)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engines)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/globals)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/objects)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/player)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/system)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/time)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/yelo_extra)

set(GAME_SRC
	${OBJECTS_CXX}
	PARENT_SCOPE)

set(GAME_H
#	${G_DEVICE_H}
#	${G_GLOBALS_H}
	${OBJECTS_H}
#	${G_PLAYER_H}
	${TIME_H}
#	${G_ENGINE_H}
#	${G_SYSTEMS_H}
#	${G_YELO_EXTRA_H}
#	${CMAKE_CURRENT_SOURCE_DIR}/action_queue.h
#	${CMAKE_CURRENT_SOURCE_DIR}/allegiance.h
#	${CMAKE_CURRENT_SOURCE_DIR}/cheats.h
#	${CMAKE_CURRENT_SOURCE_DIR}/configuration.h
#	${CMAKE_CURRENT_SOURCE_DIR}/lights.h
#	${CMAKE_CURRENT_SOURCE_DIR}/multiplayer_sounds.h
#	${CMAKE_CURRENT_SOURCE_DIR}/physics.h
#	${CMAKE_CURRENT_SOURCE_DIR}/point_physics.h
#	${CMAKE_CURRENT_SOURCE_DIR}/state.h
	PARENT_SCOPE)
