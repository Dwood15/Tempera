cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

set(PROJECT_NAME tempera)
set(INSTALL_DRIVE D:)
project(${PROJECT_NAME} CXX C)
#include_directories(BEFORE "C:/Program\ Files\ \(x86\)/Windows\ Kits/10/Include/10.0.16299.0")

unset(CMAKE_SHARED_LIBRARY_PREFIX)

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
	message(AUTHOR_WARNING "Building Chimera with ${CMAKE_CXX_COMPILER_ID} is incomplete. Use at your own risk!")

	set(COMPILER_REL_FLDR msvc)
	set(CMAKE_CXX_FLAGS /std:c++latest)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /permissive- /Gy- /W3 /Od /Zi /EHsc /fp:fast /arch:SSE2 /D\ \"WIN32\" /D \"_WINDOWS\" /D \"_USRDLL\" /D \"_WINDLL\"\ /D_CRT_SECURE_NO_WARNINGS")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ /permissive")
	message(FATAL_ERROR "Chimera does not yet support clang, if it ever will.")
else ()
	message(FATAL_ERROR "Unrecognized compiler/suite: ${CMAKE_CXX_COMPILER_ID}")
endif ()

# Output Variables
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	file(GLOB DBG_CLIENT_G ./client/debug/*.cpp)
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

add_library(LUA_LIB STATIC ${LUA_C})
set_target_properties(LUA_LIB PROPERTIES OUTPUT_NAME "lua")

set(DTA_CLIENT_G ./client/halo_data/chat.cpp
	./client/halo_data/resolution.cpp
	./client/halo_data/global.cpp
	./client/halo_data/keyboard.cpp
	./client/halo_data/map.cpp
	./client/halo_data/server.cpp
	./client/halo_data/spawn_object.cpp
	./client/halo_data/script.cpp
	./client/halo_data/table.cpp
	./client/halo_data/tag_data.cpp)

set(SIGSCANNER_G ./ceinternal/ceinternal.h
	./ceinternal/shitty_macros.h
	./ceinternal/shitty_header_ports.h
	./ceinternal/ceinternal.h
	./ceinternal/ce_base_types.h
	./ceinternal/queued_sigscanner.h
	./ceinternal/function_rewrite.h
	./ceinternal/shitty_tag_data_defs.h)

file(GLOB SHITTY_TESTS ${SIGSCANNER_G})

file(GLOB LUA_INCLUDE ./client/lua/lua.hpp)
file(GLOB CLI_OW_LUA_G ./client/lua/*.cpp)
file(GLOB LUA_OW_LUA_H ./client/lua/*.hpp ./client/lua/*.h)
file(GLOB TIA_CLIENT_G ./client/halo_data/tiarace/hce_tag_class_int.cpp)
file(GLOB HAC_INJECT_G ./code_injection/hacclient/codefinder.cpp)
file(GLOB POL_CLIENT_G ./client/interpolation/*.cpp)
file(GLOB ENH_CLIENT_G ./client/enhancements/*.cpp)
file(GLOB MSG_CLIENT_G ./client/messaging/messaging.cpp)
file(GLOB CMD_CLIENT_G ./client/command/command.cpp ./client/command/console.cpp)
file(GLOB HUD_CLIENT_G ./client/hud_mod/offset_hud_elements.cpp)
file(GLOB STT_CLIENT_G ./client/startup/fast_startup.cpp ./client/startup/crc32.c)
file(GLOB VIS_CLIENT_G ./client/visuals/*.cpp)
file(GLOB HKS_CLIENT_G ./client/hooks/*.cpp)
file(GLOB XBX_CLIENT_G ./client/xbox/*.cpp)
file(GLOB FIX_CLIENT_G ./client/fix/*.cpp)
file(GLOB INJECT_G ./code_injection/signature.cpp)
file(GLOB CLIENT_G ./client/*.cpp main.cpp)
file(GLOB MATH_G ./math/data_types.cpp)

if (MSVC)
	set(LINKER_FLAGS "/MANIFEST\ /NXCOMPAT\ /DEBUG\ /MACHINE:X86\ /OPT:NOREF\ /SAFESEH:NO\ /INCREMENTAL:NO\ /SUBSYSTEM:WINDOWS\ /MANIFESTUAC:NO\ /OPT:NOICF\ /NOLOGO")#\ /ALIGN:1") rip alignment on win32
	set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})

elseif (NOT MSVC)
	set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} $ENV{LDFLAGS} "-lws2_32 -static-libgcc -static-libstdc++ -static -luserenv -static -static -ladvapi32")
#	message(AUTHOR_WARNING "MSVC warning hack for CLion b/c CLion is dumb.")
	#set(DTA_CLIENT STATIC ${DTA_CLIENT_G})
endif ()
#some weirdness surrounding version.h and version.rc, so I'm gonna be leaving them out until further notice.
add_library(${PROJECT_NAME} SHARED ${LUA_INCLUDE} ${SHITTY_TESTS}
			${CLIENT_G}
			${INJECT_G}
			${MATH_G}
			${DTA_CLIENT_G}
			${DBG_CLIENT_G}
			${CMD_CLIENT_G}
			${ENH_CLIENT_G}
			${FIX_CLIENT_G}
			${TIA_CLIENT_G}
			${HKS_CLIENT_G}
			${HUD_CLIENT_G}
			${POL_CLIENT_G}
			${MSG_CLIENT_G}
			${STT_CLIENT_G}
			${VIS_CLIENT_G}
			${XBX_CLIENT_G}
			${HAC_INJECT_G}
			${CLI_OW_LUA_G}
			${LUA_OW_LUA_H}
			)

target_link_libraries(${PROJECT_NAME} LUA_LIB)

message(STATUS "asdf ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${HALOCE_FOLDER} PROPERTIES OUTPUT_NAME chimera)
