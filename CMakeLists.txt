#/*
# * Insert GPLv3 license
# */

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

set(PROJECT_NAME tempera)
set(INSTALL_DRIVE D:)
project(${PROJECT_NAME} CXX C)

message(STATUS "MSVC VERSION --- ${MSVC_VERSION}")
message(STATUS "BINARY OUT DIR --- ${PROJECT_BINARY_DIR}")

find_path(DX_PATH FindDirectX.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)
find_path(HCE_PATH FindHaloCE.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${DX_PATH} ${HCE_PATH})

find_package(HaloCE REQUIRED)
find_package(DirectX REQUIRED)

#TODO: JSON SERIALIZATION/DESERIALIZATION
#https://github.com/nlohmann/json#serialization--deserialization
include_directories(BEFORE ${DirectX_INCLUDE_DIRS})
message(STATUS "DX include Dirs: ${DirectX_INCLUDE_DIRS}")

# Defines Release by default.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
	message(STATUS "Build type not specified: Using Release by default.")
endif (NOT CMAKE_BUILD_TYPE)

add_definitions(-D_WINDOWS -D_USRDLL)
if (MINGW)
	if ($ENV{CLION_IDE})
		#convenience function for CLion.
		return()
	endif ()

	message(STATUS "Mingw detected.")
	set(COMPILER_REL_FLDR mingw)
	set(CMAKE_CXX_FLAGS "-std=c++17\ -fpermissive")
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
	file(GLOB DBG_CLIENT_G ./client/debug/budget.cpp
		 ./client/debug/devmode.cpp
		 ./client/debug/eep.cpp
		 ./client/debug/wireframe.cpp
		 )
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./bin/${COMPILER_REL_FLDR}-debug")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	if ($ENV{CLION_IDE})
		message(STATUS "convenience exit function for CLion building")
		return()
	endif ()
	set(OUTPUT_DIR ./bin/${COMPILER_REL_FLDR}-Release)
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/ar")

file(GLOB LUA_C "./client/lua/lua/*.c")

#add_definitions(-DLUA -D_CRT_SECURE_NO_WARNINGS)
#add_library(LUA_LIB STATIC ${LUA_C})
#set_target_properties(LUA_LIB PROPERTIES OUTPUT_NAME "lua")
#Lua needs to be linked before the linker flags are set because some of what we use are _not_ compatible with building it.
#TODO: find out why//what flags, and fix them.

#TODO: REMOVE THIS STUFF BEFORE PUBLISHING
set(GAMEMAP_H
	./ceinternal/haloforge/game_map/bsp.h
	./ceinternal/haloforge/game_map/halo_types.h
	./ceinternal/haloforge/game_map/map.h
	./ceinternal/haloforge/game_map/scenario.h
	./ceinternal/haloforge/game_map/tags.h
	)

set(HALOFORGE_H ./ceinternal/haloforge/headers/addlog.h
	./ceinternal/haloforge/headers/blam.h
	./ceinternal/haloforge/headers/camera.h
	./ceinternal/haloforge/headers/core.h
	./ceinternal/haloforge/headers/detours.h
	./ceinternal/haloforge/headers/halo_types.h
	./ceinternal/haloforge/headers/forgeMain.h
	./ceinternal/haloforge/headers/math.h
	./ceinternal/haloforge/headers/objectcontroller.h
	./ceinternal/haloforge/headers/objects.h
	./ceinternal/haloforge/headers/players.h
	./ceinternal/haloforge/Direct3D/textures.h
	./ceinternal/haloforge/Direct3D/d3d9hook.h
	)

set(CEINTERNAL_H
	./ceinternal/ce_base_types.h
	./ceinternal/shitty_tag_data_defs.h
	./ceinternal/shitty_enums.h
	./ceinternal/gamestate_headers/player_types.h
	./ceinternal/shitty_macros.h
	./ceinternal/shitty_header_ports.h
	./ceinternal/exceptions/exception_handler.h
	)

set(HALOFORGE_CPP
	./ceinternal/haloforge/Direct3D/d3d9hook.cpp
	./ceinternal/haloforge/addlog.cpp
	./ceinternal/haloforge/blam.cpp
	./ceinternal/haloforge/camera.cpp
	./ceinternal/haloforge/core.cpp
	./ceinternal/haloforge/math.cpp
	./ceinternal/haloforge/objectcontroller.cpp
	./ceinternal/haloforge/forgeMain.cpp
	)

set(CEINTERNAL_CPP ${CEINTERNAL_H} ${HALOFORGE_CPP}
	./ceinternal/function_rewrite.cpp
	./ceinternal/ceinternal.cpp
	./ceinternal/exceptions/exception_handler.cpp
	./main.cpp
	)

#CHIMERA
set(CHIMERA_G ./client/client.cpp
	./client/client_signature.cpp
	./client/hac2.cpp
	./client/keystone.cpp
	./client/settings.cpp
	./client/path.cpp
	./client/open_sauce.cpp
	./main.cpp
	)

set(DTA_CLIENT_G ./client/halo_data/chat.cpp
	./client/halo_data/resolution.cpp
	./client/halo_data/global.cpp
	./client/halo_data/keyboard.cpp
	./client/halo_data/map.cpp
	./client/halo_data/server.cpp
	./client/halo_data/spawn_object.cpp
	./client/halo_data/script.cpp
	./client/halo_data/table.cpp
	./client/halo_data/tag_data.cpp
	)

set(HUD_CLIENT_G ./client/hud_mod/offset_hud_elements.cpp)
set(MSG_CLIENT_G ./client/messaging/messaging.cpp)
set(CMD_CLIENT_G ./client/command/command.cpp ./client/command/console.cpp)
set(STT_CLIENT_G ./client/startup/fast_startup.cpp ./client/startup/crc32.c)
set(TIA_CLIENT_G ./client/halo_data/tiarace/hce_tag_class_int.cpp)

set(ENHANCE_CLIENT_G
	./client/enhancements/auto_center.cpp
	./client/enhancements/disable_buffering.cpp
	./client/enhancements/firing_particle.cpp
	./client/enhancements/gamepad_vertical_scale.cpp
	./client/enhancements/mouse.cpp
	./client/enhancements/multitexture_overlay.cpp
	./client/enhancements/server_messages.cpp
	./client/enhancements/show_spawn.cpp
	./client/enhancements/skip_loading.cpp
	./client/enhancements/throttle_fps.cpp
	./client/enhancements/uncap_cinematic.cpp
	./client/enhancements/zoom_blur.cpp
	)

#CHIMERA
file(GLOB MATH_G ./math/data_types.cpp)
file(GLOB CLI_OW_LUA_G ./client/lua/*.cpp)
file(GLOB LUA_OW_LUA_H ./client/lua/*.hpp ./client/lua/*.h)

file(GLOB VISUALS_CLIENT_G
	 ./client/visuals/anisotropic_filtering.cpp
	 ./client/visuals/gametype_indicator.cpp
	 ./client/visuals/letterbox.cpp
	 ./client/visuals/server_ip.cpp
	 ./client/visuals/vertical_field_of_view.cpp
	 )

file(GLOB HOOKS_CLIENT_G
	 ./client/hooks/camera.cpp
	 ./client/hooks/frame.cpp
	 ./client/hooks/map_load.cpp
	 ./client/hooks/rcon_message.cpp
	 ./client/hooks/tick.cpp
	 )

file(GLOB XBOX_CLIENT_G
	 ./client/xbox/hud_kill_feed.cpp
	 ./client/xbox/safe_zone.cpp
	 ./client/xbox/simple_score_screen.cpp
	 ./client/xbox/split_screen_hud.cpp
	 )

file(GLOB FIX_CLIENT_G
	 ./client/fix/camo_fix.cpp
	 ./client/fix/descope_fix.cpp
	 ./client/fix/fov_fix.cpp
	 ./client/fix/magnetism_fix.cpp
	 ./client/fix/scope_fix.cpp
	 ./client/fix/sniper_hud.cpp
	 ./client/fix/widescreen_fix.cpp)

file(GLOB HAC2_INJECT_G ./code_injection/hacclient/codefinder.cpp)
file(GLOB INTERPOL_CLIENT_G
	 ./client/interpolation/camera.cpp
	 ./client/interpolation/fp.cpp
	 ./client/interpolation/widget.cpp
	 ./client/interpolation/particle.cpp
	 ./client/interpolation/light.cpp
	 ./client/interpolation/interpolation.cpp
	 )

#EXTERNAL LIBS
file(GLOB DETOURS_GLOB ./libs/detours.lib)
file(GLOB CAPSTONE_STATIC_GLOB ./libs/capstone_static.lib)

if (MSVC)
	set(LINKER_FLAGS "/MANIFEST\ /NXCOMPAT\ /DEBUG\ /MACHINE:X86\ /SAFESEH:NO\ /INCREMENTAL:NO\ /SUBSYSTEM:WINDOWS\ /MANIFESTUAC:NO\ /OPT:NOICF\ /NOLOGO\ /ASSEMBLYDEBUG\ /ALIGN:1") #rip alignment on win32
	set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})

elseif (NOT MSVC)
	set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} $ENV{LDFLAGS} "-lws2_32 -static-libgcc -static-libstdc++ -static -luserenv -static -static -ladvapi32")
endif ()
#some weirdness surrounding version.h and version.rc, so I'm gonna be leaving them out until further notice.
add_library(${PROJECT_NAME} SHARED ${CEINTERNAL_CPP})

target_link_libraries(${PROJECT_NAME} ${DETOURS_GLOB} ${CAPSTONE_STATIC_GLOB} ${DirectX_LIBRARIES})

message(STATUS "Outputting lib to: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

SET(CMAKE_PDB_OUTPUT_DIRECTORY  "${HaloCE_ROOT_DIR}/controls")
#SET(EXECUTABLE_OUTPUT_PATH   "${HaloCE_ROOT_DIR}/controls")
SET(LIBRARY_OUTPUT_PATH   "${HaloCE_ROOT_DIR}/controls")

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "tempera")
