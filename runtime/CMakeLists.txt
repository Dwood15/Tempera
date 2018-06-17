
# I put runtime/versions/versions.h into its own directory b/c I want to keep as little version-specific information inside the main codebase.
#Unfortunately, this may be an effort in vain.
#sure if cmake will make <runtime/pc/foobar.h> a valid include
#even though we want only to deal with <foobar.h> for version-specific headers.

set(STUBBZ runtime/stubbz)
set(HALO_TRIAL runtime/halo1/windows/trial)
set(HALO_FULL runtime/halo1/windows/full)

if (NOT DEFINED TARGET_GAME_VER)
	message(WARNING "TARGET_GAME_VER IS NOT DEFINED")
	message(FATAL_ERROR "BUILDING TEMPERA REQUIRES A VERSION OF THE 2000-2005 ERA (ie halo 1) BLAM ENGINE TO BE SELECTED.")
endif ()

if (TARGET_GAME_VER MATCHES "^STUBBZ$")
	#Is there more than one stubbz version? Maybe mac?
	#set(LEVELS ${STUBBZ})
	set(GAME ${STUBBZ})
	message(FATAL_ERROR "STUBBZ IS NOT AN IMPLEMENTED TARGET. SORRY.")
endif ()

if (NOT TARGET_GAME_VER MATCHES "^(HALO)_(TRIAL|PC|CE)")
	message(FATAL_ERROR "UNKNOWN VERSION ${TARGET_GAME_VER} SELECTED.")
endif ()

#This is deliberately _not_ an elseif.
if (TARGET_GAME_VER MATCHES "^(HALO)_(TRIAL|PC|CE)")
	set(GAME runtime/halo1)
endif ()

if (TARGET_GAME_VER MATCHES "^HALO_TRIAL$")
	#set(LEVELS ${HALO_TRIAL})
	set(GAME ${HALO_TRIAL})
	message(FATAL_ERROR "HALO TRIAL IS NOT AN IMPLEMENTED LIBRARY TARGET. SORRY.")
endif ()

if (NOT TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_[0-1][0-9])$")
	message(FATAL_ERROR "UNKNOWN VERSION ${TARGET_GAME_VER} SELECTED.")
endif ()

#This is deliberately _not_ an elseif.
if (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_[0-1][0-9])$")
	#TODO: Find a better way of doing this garbage.
	if (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_0[0-9])$")
		message(FATAL_ERROR "ONLY 1_10 IS AN IMPLEMENTED LIBRARY TARGET. SORRY.")

		if (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_00)$")
			set(MINOR_VER "1_00")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_01)$")
			set(MINOR_VER "1_01")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_02)$")
			set(MINOR_VER "1_02")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_03)$")
			set(MINOR_VER "1_03")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_04)$")
			set(MINOR_VER "1_04")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_05)$")
			set(MINOR_VER "1_05")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_06)$")
			set(MINOR_VER "1_06")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_07)$")
			set(MINOR_VER "1_07")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_08)$")
			set(MINOR_VER "1_08")
		elseif (TARGET_GAME_VER MATCHES "^(HALO)_(PC|CE)_(1_09)$")
			set(MINOR_VER "1_09")
		endif ()
	else ()
		set(MINOR_VER "1_10")
	endif ()

	if (TARGET_GAME_VER MATCHES "^(HALO)_PC_(1_[0-1][0-9])$")
		set(MAJOR_VER ${HALO_FULL}/pc)
		find_path(GAME_PATH FindHaloCE.cmake PATHS ./cmake REQUIRED)

		message(FATAL_ERROR "HALO PC IS NOT YET AN IMPLEMENTED LIBRARY TARGET. SORRY.")
	else ()
		set(MAJOR_VER ${HALO_FULL}/ce CACHE INTERNAL "")
		find_path(GAME_PATH FindHaloCE.cmake PATHS ./cmake REQUIRED)
	endif ()

	set(LEVELS ${HALO_FULL} CACHE INTERNAL "")
	set(GAME ${MAJOR_VER}/${MINOR_VER} CACHE INTERNAL "")
endif ()

set(GAME_DIR ${PROJECT_SOURCE_DIR}/${GAME})
set(LEVELS_DIR ${PROJECT_SOURCE_DIR}/${LEVELS})
set(RUNTIME_DIR ${PROJECT_SOURCE_DIR}/runtime)

include_directories(AFTER ${GAME_DIR} ${LEVELS_DIR})

set(R_ENGINELAYOUT_INL
#	${GAME_DIR}/enginelayout/Game.inl
#	${GAME_DIR}/enginelayout/Networking.inl
#	${GAME_DIR}/enginelayout/Objects.inl
#	${GAME_DIR}/enginelayout/OpenSauce.inl
#	${GAME_DIR}/enginelayout/Rasterizer.DX9.inl
#	${GAME_DIR}/enginelayout/Rasterizer.inl
#	${GAME_DIR}/enginelayout/Rasterizer.ShaderDraw.inl
#	${GAME_DIR}/enginelayout/Rasterizer.ShaderExtension.inl
#	${GAME_DIR}/enginelayout/TagGroups.inl
	)

set(RUNTIME_H
	${GAME_DIR}/memory_locations.h
	${GAME_DIR}/memory_map.h
	${GAME_DIR}/engine_functions.h
	${GAME_DIR}/current_engine.h
	${LEVELS_DIR}/levels.h
	${R_ENGINELAYOUT_INL}
	${RUNTIME_DIR}/versions/cache_files_win.hpp
	${RUNTIME_DIR}/versions/precompile.h
	${RUNTIME_DIR}/versions/versions.h
	PARENT_SCOPE)

set(GAME_LIB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/versions ${GAME_DIR} ${LEVELS_DIR} CACHE INTERNAL "")
