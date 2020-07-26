
set(HALO_TRIAL runtime/halo1/windows/trial)
set(HALO_FULL runtime/halo1/windows/full)

if (NOT DEFINED TARGET_GAME_VER)
	message(WARNING "TARGET_GAME_VER IS NOT DEFINED")
	message(FATAL_ERROR "BUILDING TEMPERA REQUIRES A VERSION OF THE 2000-2005 ERA (ie halo 1) BLAM ENGINE TO BE SELECTED.")
endif ()

if (NOT TARGET_GAME_VER MATCHES "^(HALO)_(TRIAL|PC|CE)")
	message(FATAL_ERROR "UNKNOWN VERSION ${TARGET_GAME_VER} SELECTED.")
endif ()

#This is deliberately _not_ an elseif.
if (TARGET_GAME_VER MATCHES "^(HALO)_(TRIAL|PC|CE)")
	set(GAME runtime/halo1)
endif ()

if (TARGET_GAME_VER MATCHES "^HALO_TRIAL$")
	set(GAME ${HALO_TRIAL})
	message(FATAL_ERROR "HALO TRIAL IS NOT AN IMPLEMENTED LIBRARY TARGET. SORRY.")
endif ()

if (NOT TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_[0-1][0-9])$")
	message(FATAL_ERROR "UNKNOWN VERSION ${TARGET_GAME_VER} SELECTED.")
endif ()

set(MINOR_VER 1_10)
set(MAJOR_VER ${HALO_FULL}/ce PARENT_SCOPE)

find_path(GAME_PATH FindHaloCE.cmake PATHS ./cmake REQUIRED)


set(RUNTIME_DIR ${PROJECT_SOURCE_DIR}/runtime)


set(SUPPORTED_RUNTIMES_H
#	${RUNTIME_DIR}/ce/1_10/function_map.txt
	${RUNTIME_DIR}/ce/1_10/function_rewrite.h
	${RUNTIME_DIR}/ce/1_10/110EngineManager.h
#	${RUNTIME_DIR}/hek/sapien/function_map.txt
	${RUNTIME_DIR}/hek/sapien/sapienEngineManager.h
	)

set(RUNTIME_CXX
	${RUNTIME_DIR}/ce/1_10/110EngineManager.cpp
	${RUNTIME_DIR}/hek/sapien/sapienEngineManager.cpp
	${RUNTIME_DIR}/ce/1_10/function_rewrite.cpp
	PARENT_SCOPE)

set(RUNTIME_H
	${SUPPORTED_RUNTIMES_H}
	${RUNTIME_DIR}/versions.h
	${RUNTIME_DIR}/memory_map.h
	PARENT_SCOPE)

#what should be accessible - just memory_map.h and versions.h via <versions.h>, etc etc
set(GAME_LIB_DIRS ${RUNTIME_DIR} PARENT_SCOPE)
