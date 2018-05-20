#Project: Tempera
#File: CMakeLists.txt
#Copyright � 2018 dwood
#
#this file is part of Tempera.
#
#Tempera is free software: you can redistribute it and/or modify
#it under the terms of the gnu general public license as published by
#the free software foundation, either version 3 of the license, or
#(at your option) any later version.
#
#Tempera is distributed in the hope that it will be useful,
#but without any warranty; without even the implied warranty of
#merchantability or fitness for a particular purpose. See the
#gnu general public license for more details.
#
#You should have received a copy of the gnu general public license
#along with Tempera.  if not, see <http://www.gnu.org/licenses/>.

#this file is currently heavily inconsistent. I'm sorry.
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

unset(CMAKE_SHARED_LIBRARY_PREFIX)
unset(CMAKE_CL_64)

#capstone variables for debugging...
set(CAPSTONE_ARM_SUPPORT 0)
set(CAPSTONE_ARM64_SUPPORT 0)
set(CAPSTONE_MIPS_SUPPORT 0)
set(CAPSTONE_PPC_SUPPORT 0)
set(CAPSTONE_SPARC_SUPPORT 0)
set(CAPSTONE_SYSZ_SUPPORT 0)
set(CAPSTONE_XCORE_SUPPORT 0)
set(CAPSTONE_X86_SUPPORT 1)
#fuck att syntax.
set(CAPSTONE_X86_ATT_DISABLE ON)
#end capstone

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

set(PROJECT_NAME tempera)
set(INSTALL_DRIVE D:)
project(${PROJECT_NAME} CXX C)

message(STATUS "MSVC VERSION --- ${MSVC_VERSION}")
message(STATUS "BINARY OUT DIR --- ${PROJECT_BINARY_DIR}")

find_path(DX_PATH FindDirectX.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)
find_path(HCE_PATH FindHaloCE.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)
find_path(MDB_PATH FindMariaDB.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${DX_PATH} ${HCE_PATH} ${MDB_PATH})

find_package(HaloCE REQUIRED)
find_package(DirectX REQUIRED)
find_package(MariaDB REQUIRED)
#TODO: JSON SERIALIZATION/DESERIALIZATION
#https://github.com/nlohmann/json#serialization--deserialization
include_directories(BEFORE ${DirectX_INCLUDE_DIRS} ${MARIADB_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/common/)
message(STATUS "DX include Dirs: ${DirectX_INCLUDE_DIRS}")

# Defines Release by default.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
	message(STATUS "Build type not specified: Using Release by default.")
endif (NOT CMAKE_BUILD_TYPE)

add_definitions(-D_WINDOWS -D_USRDLL)
if (NOT MSVC OR NOT MSVC_VERSION)
	return(0)
	if ($ENV{CLION_IDE})
		#convenience function for CLion.
		return(0)
	endif ()

	message(STATUS "Mingw detected.")
	set(COMPILER_REL_FLDR mingw)
	set(CMAKE_CXX_`FLAGS "-std=c++17\ -fpermissive")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}\ -g\ -m32\ -masm=intel")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -g -m32 -masm=intel")
elseif (MSVC)
	#vc++ 2017 toolset. Available from microsoft here: http://landinghub.visualstudio.com/visual-cpp-build-tools
	if (NOT CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION OR NOT "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}" STREQUAL "10.0.16299.0")
		#message(WARNING "CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION is empty! Could not find applicable windows 10 sdk.")
		set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 10.0.16299.0)
		#TODO: CHECK FOR AVAILABLE SDK VERSIONS IF NONE SET
	endif ()

	set(COMPILER_REL_FLDR msvc)
	set(CMAKE_C_FLAGS '')
	set(CMAKE_CXX_FLAGS /std:c++latest\ /MTd\ )
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd\ /Gy- /W3 /Od /Zi /MP /EHsc /fp:fast\ /std:c++latest\ /arch:SSE2 /D\ \"WIN32\" /D \"_WINDOWS\" /D \"_USRDLL\" /D \"_WINDLL\"\ /D_CRT_SECURE_NO_WARNINGS")
else ()
	message(FATAL_ERROR "Unrecognized compiler/suite: ${CMAKE_CXX_COMPILER_ID}")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	if ($ENV{CLION_IDE})
		message(STATUS "convenience hack for making CLion less annoying.")
	endif ()

	#DO DEBUG STUFF HERE
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./bin/${COMPILER_REL_FLDR}-debug")

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	#holy crap cmake's stopped listening to my return() calls. WTF???
	#	add_custom_command(UGH COMMAND exit 0)
	return(0)
	set(OUTPUT_DIR ./bin/${COMPILER_REL_FLDR}-Release)
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/ar")

set(GAMEMAP_H
	./gamestate/objects.h
	./headers/colors.h
	tags/bsp.h
	./gamestate/halo_types.h
	tags/map.h
	tags/tags.h
	gamestate/scenario.h
	enums/ControlTypes.h
	gamestate/controls.h
	dinput/dinput.h
	common/players_loop_related.h
	gamestate/experimentation/player_controller_to_index_array.h gamestate/experimentation/render_window.h)

set(TAGS_H
	tags/scenario.h
	tags/map.h
	tags/bsp.h
	tags/tags.h
	)

set(HALOFORGE_H
	${TAGS_H} ${GAMEMAP_H}
	./haloforge/headers/addlog.h
	./haloforge/headers/blam.h
	./haloforge/headers/camera.h
	./haloforge/headers/core.h
	./haloforge/headers/detours.h
	./haloforge/headers/forge.h
	./haloforge/headers/objectcontroller.h
	./haloforge/headers/objects.h
	./haloforge/headers/players.h
	./Direct3D/textures.h
	./Direct3D/d3d9hook.h
	haloforge/headers/forge.h
	headers/weapons.h common/function_map.h)

set(CEINTERNAL_H ${HALOFORGE_H}
	./headers/real_math.h
	./headers/int_math.h
	./ce_base_types.h
	headers/enums_generic.h
	./gamestate/player_types.h
	headers/macros_generic.h
	./exceptions/exception_handler.h
	headers/animations.h
	gamestate/sbsp.h
	gamestate/networking.h
	gamestate/game_globals.h
	gamestate/render.h
	gamestate/tags_generic.h
	common/target_version.h
	common/open_sauce_mem_locs.h
	common/precompile.h
	common/eng_functions.h)

set(HALOFORGE_CPP
	./Direct3D/d3d9hook.cpp
	./haloforge/addlog.cpp
	./haloforge/blam.cpp
	./haloforge/camera.cpp
	./haloforge/core.cpp
	./haloforge/math.cpp
	./haloforge/objectcontroller.cpp
	./forge.cpp
	)

set(CEINTERNAL_CPP ${CEINTERNAL_H} ${HALOFORGE_CPP}
	./function_rewrite.cpp
	./exceptions/exception_handler.cpp
	./main.cpp
	gamestate/controls.cpp
	gamestate/experimentation/hud_related.h)

#EXTERNAL LIBS
file(GLOB DETOURS_GLOB ./libs/detours.lib)
file(GLOB CAPSTONE_STATIC_GLOB ./libs/capstone_static.lib)

if (MSVC)
	set(BASE_FLAGS "/MANIFEST\ /NXCOMPAT\ /DEBUG\ /MACHINE:X86\ /SAFESEH:NO\ /INCREMENTAL:NO\ /SUBSYSTEM:WINDOWS\ /MANIFESTUAC:NO\ /OPT:NOICF\ /NOLOGO\ /ignore:4070")

	set(LINKER_FLAGS "${BASE_FLAGS} /ASSEMBLYDEBUG\ /DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/Tempera.def\"")
	#rip alignment on win32
	set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})

elseif (NOT MSVC)
	return()
	set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} $ENV{LDFLAGS} "-lws2_32 -static-libgcc -static-libstdc++ -static -luserenv -static -static -ladvapi32")
endif ()

if (MARIADB_FOUND)
	add_library(mdb STATIC IMPORTED)
	message(STATUS "WE HAVE MARIADB ${MARIADB_LIBRARY}")
	set_target_properties(mdb PROPERTIES IMPORTED_LOCATION ${MARIADB_LIBRARY})
endif ()

add_library(dinput8 SHARED ${CEINTERNAL_CPP})

target_link_libraries(dinput8 ${DETOURS_GLOB} ${MARIADB_LIBRARY} ${CAPSTONE_STATIC_GLOB} ${DirectX_LIBRARIES})

message(STATUS "Outputting libmariadb to: ${MARIADB_LIBRARY}")

message(STATUS "Outputting lib to: ${HaloCE_ROOT_DIR}")

set(CMAKE_PDB_OUTPUT_DIRECTORY "${HaloCE_ROOT_DIR}")
set(LIBRARY_OUTPUT_PATH "${HaloCE_ROOT_DIR}")

set_target_properties(dinput8 PROPERTIES OUTPUT_NAME "dinput8")
