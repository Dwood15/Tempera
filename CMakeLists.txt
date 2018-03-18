cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

set(PROJECT_NAME tempera)
set(INSTALL_DRIVE D:)
project(${PROJECT_NAME} CXX C)

#until I figure out how finding modules works, this will have to do.

message(STATUS "FINDING DIRECTX")

find_path(DX_PATH FindDirectX.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${DX_PATH})

#this file is currently heavily inconsistent.

find_package(DirectX REQUIRED)

unset(CMAKE_SHARED_LIBRARY_PREFIX)

unset(CMAKE_CL_64)

## Folders files
#set(CMAKE_CXX_STANDARD 17)
# Define Release by default.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
	message(STATUS "Build type not specified: Using Release by default.")
endif (NOT CMAKE_BUILD_TYPE)

add_definitions(-D_WINDOWS -D_USRDLL)
#check for msvc -> not currently supported.
if (MINGW)
	if ($ENV{CLION_IDE})
		#convenience function for CLion
		return()
	endif ()

	message(STATUS "Mingw detected.")
	set(COMPILER_REL_FLDR mingw)
	set(CMAKE_CXX_FLAGS "-std=c++17\ -fpermissive")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}\ -g\ -m32\ -masm=intel")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -g -m32 -masm=intel")
elseif (MSVC)
	message(AUTHOR_WARNING "Building Chimera with ${CMAKE_CXX_COMPILER_ID} is experimental. Use at your own risk!")

	#vc++ 2017 toolset. Available from microsoft here: http://landinghub.visualstudio.com/visual-cpp-build-tools
	if (NOT CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION OR NOT "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}" STREQUAL "10.0.16299.0")
		message(WARNING "CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION is empty! Could not find applicable windows 10 sdk. ${CMAKE_GENERATOR_TOOLSET} ${CMAKE_HOST_WIN32}")
		set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 10.0.16299.0)
		#include_directories(<somekeyword?> "C:/Program\ Files\ \(x86\)/Windows\ Kits/10/Include/10.0.16299.0")
		#TODO: CHECK FOR AVAILABLE SDK VERSIONS IF NONE SET
	endif ()

	set(COMPILER_REL_FLDR msvc)
	set(CMAKE_CXX_FLAGS /std:c++17)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Gy- /W3 /Od /Zi /MP /EHsc /fp:fast /arch:SSE2 /D\ \"WIN32\" /D \"_WINDOWS\" /D \"_USRDLL\" /D \"_WINDLL\"\ /D_CRT_SECURE_NO_WARNINGS")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ /permissive")
	message(FATAL_ERROR "Chimera does not yet support clang, if it ever will.")
else ()
	message(FATAL_ERROR "Unrecognized compiler/suite: ${CMAKE_CXX_COMPILER_ID}")
endif ()

# Output Variables
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	file(GLOB DBG_CLIENT_G ./client/debug/budget.cpp
		 ./client/debug/devmode.cpp
		 ./client/debug/eep.cpp
		 ./client/debug/wireframe.cpp
		 )

	set(OUTPUT_DIR ./bin/${COMPILER_REL_FLDR}-debug)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	if ($ENV{CLION_IDE})
		message(STATUS "convenience exit function for CLion building")
		return()
	endif ()

	set(OUTPUT_DIR ./bin/${COMPILER_REL_FLDR}-Release)
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/ar")
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/exe")

#add_library (lualib STATIC lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c linit.c)
file(GLOB LUA_C "./client/lua/lua/*.c")

#add_definitions(-DLUA -D_CRT_SECURE_NO_WARNINGS)
include_directories(BEFORE ${DirectX_INCLUDE_DIRS})

message(STATUS "DX include Dirs: ${DirectX_INCLUDE_DIRS}")

add_library(LUA_LIB STATIC ${LUA_C})
set_target_properties(LUA_LIB PROPERTIES OUTPUT_NAME "lua")
#Lua needs to be linked before the linker flags are set because some of we use are _not_ compatible with building it.
#TODO: find out why//what flags, and fix them.

#TODO: REMOVE THIS STUFF BEFORE PUBLISHING

set(GAMEMAP_H
	./ceinternal/haloforge/game_map/bsp.h
	./ceinternal/haloforge/game_map/halo_types.h
	./ceinternal/haloforge/game_map/map.h
	./ceinternal/haloforge/game_map/scenario.h
	./ceinternal/haloforge/game_map/tags.h)

set(HALOFORGE_H ./ceinternal/haloforge/headers/addlog.h
	./ceinternal/haloforge/headers/blam.h
	./ceinternal/haloforge/headers/camera.h
	./ceinternal/haloforge/headers/core.h
	./ceinternal/haloforge/headers/detours.h
	./ceinternal/haloforge/headers/halo_types.h
	ceinternal/haloforge/headers/forgeMain.h
	./ceinternal/haloforge/headers/math.h
	./ceinternal/haloforge/headers/objectcontroller.h
	./ceinternal/haloforge/headers/objects.h
	./ceinternal/haloforge/headers/players.h
	./ceinternal/haloforge/Direct3D/textures.h
	./ceinternal/haloforge/Direct3D/d3d9hook.h)

set(CEINTERNAL_H
	./ceinternal/ce_base_types.h
	./ceinternal/shitty_tag_data_defs.h
	./ceinternal/shitty_enums.h
	./ceinternal/gamestate_headers/player_types.h
	./ceinternal/shitty_macros.h
	./ceinternal/shitty_header_ports.h
	)

set(CEINTERNAL_CPP ${CEINTERNAL_H}
	./client/client.cpp
	./client/client_signature.cpp
	./client/hac2.cpp
	./client/keystone.cpp
	./client/settings.cpp
	./client/path.cpp
	./client/open_sauce.cpp
	./ceinternal/haloforge/Direct3D/d3d9hook.cpp
	./ceinternal/haloforge/addlog.cpp
	./ceinternal/haloforge/blam.cpp
	./ceinternal/haloforge/camera.cpp
	./ceinternal/haloforge/core.cpp
	./ceinternal/haloforge/math.cpp
	./ceinternal/haloforge/objectcontroller.cpp
	./ceinternal/haloforge/forgeMain.cpp
	./ceinternal/function_rewrite.cpp
	./ceinternal/ceinternal.cpp
	./ceinternal/patching_hooking.cpp
	./main.cpp
	)

#TODO: END PRIVATE

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

file(GLOB INJECT_G ./code_injection/signature.cpp)
file(GLOB MATH_G ./math/data_types.cpp)

#A lot of this was just trying random crap to see what works and what won't.
#The trick here was literally just trying things and figuring out what worked and what didn't.
#So there's an issue with FILE GLOBS that do *.cpp, *.h - CMAKE will load them initially, but if you change one of these files,
# MAKE doesn't know they need to be changed.
# The solution is: run clean before rebuilding, or fix this so it refers to each file explicitly like above.
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

file(GLOB DETOURS_GLOB ./detours.lib)

if (MSVC)
	set(LINKER_FLAGS "/MANIFEST\ /NXCOMPAT\ /DEBUG\ /MACHINE:X86\ /SAFESEH:NO\ /INCREMENTAL:NO\ /SUBSYSTEM:WINDOWS\ /MANIFESTUAC:NO\ /OPT:NOICF\ /NOLOGO\ /ASSEMBLYDEBUG")#\ /ALIGN:1") rip alignment on win32
	set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})

elseif (NOT MSVC)
	set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} $ENV{LDFLAGS} "-lws2_32 -static-libgcc -static-libstdc++ -static -luserenv -static -static -ladvapi32")
	#message(AUTHOR_WARNING "MSVC warning hack b/c CLion is dumb.")
	#set(DTA_CLIENT STATIC ${DTA_CLIENT_G})
endif ()
#some weirdness surrounding version.h and version.rc, so I'm gonna be leaving them out until further notice.
add_library(${PROJECT_NAME} SHARED ${INJECT_G} ${MATH_G} ${DTA_CLIENT_G} ${DBG_CLIENT_G} ${CMD_CLIENT_G}
			${ENHANCE_CLIENT_G}
			${FIX_CLIENT_G}
			${TIA_CLIENT_G}
			${HOOKS_CLIENT_G}
			${HUD_CLIENT_G}
			${INTERPOL_CLIENT_G}
			${MSG_CLIENT_G}
			${STT_CLIENT_G}
			${VISUALS_CLIENT_G}
			${XB0X_CLIENT_G}
			${HAC2_INJECT_G}
			${CLI_OW_LUA_G}
			${LUA_OW_LUA_H}
			${CEINTERNAL_CPP}
			)

#add_library(DETOURS STATIC IMPORTED)
message(STATUS detours_globa ${DETOURS_GLOB} DETOURS_GLOB})
target_link_libraries(${PROJECT_NAME} LUA_LIB ${DETOURS_GLOB} ${DirectX_LIBRARIES})

message(STATUS "asdf ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${HALOCE_FOLDER} PROPERTIES OUTPUT_NAME chimera)
