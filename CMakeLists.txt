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

#this file is currently heavily inconsistent. Sorry.
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

unset(CMAKE_SHARED_LIBRARY_PREFIX)
unset(CMAKE_CL_64)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

set(LUA_Debug true)

set(PROJECT_NAME tempera)
project(${PROJECT_NAME} C CXX)

set(TARGET_GAME_VER "HALO_CE_1_10")

add_subdirectory(runtime)

set(COMMON ${CMAKE_CURRENT_SOURCE_DIR}/common/)
include_directories(AFTER ${GAME_LIB_DIRS}  ${COMMON})

find_path(DX_PATH FindDirectX.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)
find_path(MDB_PATH FindMariaDB.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)
find_path(LUA_PATH FindLua.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LUA_PATH} ${DX_PATH} ${GAME_PATH} ${MDB_PATH})

find_package(HaloCE REQUIRED)
find_package(Lua REQUIRED)

if (LUA_Debug)
	message(STATUS "Post-Find debug: LUA_LIBRARY: ${LUA_LIBRARY}\ LUA_LIBRARIES: ${LUA_LIBRARIES}\ LUA_INCLUDE_DIR: ${LUA_INCLUDE_DIR}")
endif ()

find_package(DirectX REQUIRED)
find_package(MariaDB REQUIRED)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include_directories(BEFORE ${DirectX_INCLUDE_DIRS} ${INCLUDE}/detours ${LUA_INCLUDE_DIR} ${MARIADB_INCLUDE_DIR})

# Defines Debug by default.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
	message(STATUS "Build type not specified: Using Debug by default.")
endif (NOT CMAKE_BUILD_TYPE)

add_definitions(-D_WINDOWS -D_USRDLL)
if (NOT MSVC OR NOT MSVC_VERSION)
	return(0)
	#convenience for CLion.
	message(FATAL_ERROR "Unrecognized compiler/suite: ${CMAKE_CXX_COMPILER_ID}")
elseif (MSVC)
	#vc++ 2017 toolset. Available from microsoft here: http://landinghub.visualstudio.com/visual-cpp-build-tools
	if (NOT CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION OR NOT "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}" STREQUAL "10.0.17134.0")
		#message(WARNING "CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION is empty! Could not find applicable windows 10 sdk.")
		set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 10.0.17134.0)
		#TODO: CHECK FOR AVAILABLE SDK VERSIONS IF NONE SET
	endif ()

	set(COMPILER_REL_FLDR msvc)
	set(CMAKE_C_FLAGS '')
	set(CMAKE_CXX_FLAGS /std:c++latest\ /MTd\ )
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd\ /Gy- /W3 /Od /Zi /MP /EHsc /fp:fast\ /std:c++latest\ /arch:SSE2 /D\ \"WIN32\" /D \"_WINDOWS\" /D \"_USRDLL\" /D \"_WINDLL\"\ /D_CRT_SECURE_NO_WARNINGS")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	return(0)
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
	if ($ENV{CLION_IDE})
		message(STATUS "convenience hack for making CLion show the correct cmake tab")
	endif ()

	#DO DEBUG STUFF HERE
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./bin/${COMPILER_REL_FLDR}-debug")
endif ()

#EXTERNAL LIBS
file(GLOB DETOURS_GLOB libs/detours.lib)
file(GLOB LUA_GLOB libs/LUA.lib)

if (NOT MSVC)
	return()
elseif (MSVC)
	# 4070 : Silences `OUT directive differs from filename ignoring directive` (warning)
#		4099 :
	set(BASE_FLAGS "/MANIFEST\ /NXCOMPAT\ /DEBUG\ /MACHINE:X86\ /SAFESEH:NO\ /INCREMENTAL:NO\ /SUBSYSTEM:WINDOWS\ /MANIFESTUAC:NO\ /OPT:NOICF\ /NOLOGO\ /ignore:4099\ /ignore:4070")

	set(LINKER_FLAGS "${BASE_FLAGS} /ASSEMBLYDEBUG\ /DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/Tempera.def\"")
	#rip alignment on win32
	set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})
endif ()
if (MARIADB_FOUND)
	add_library(mdb STATIC IMPORTED)
	set_target_properties(mdb PROPERTIES IMPORTED_LOCATION ${MARIADB_LIBRARY})
endif ()

add_subdirectory(src)

set(COMMON_H
	common/addlog.h
	common/engine_interface.h
	common/macros_generic.h
	common/struct_helper.h
	)

add_library(dinput8 SHARED
			common/addlog.cpp
			${COMMON_H}
			${LUA_INCLUDE_DIR}
			${SRC_H}
			${RUNTIME_H}
			${RUNTIME_CXX}
			${SRC_CXX}
			main.cpp
			)

#The compiler is defaulting to the Windows 10 SDK, which requires external DLL's (xinput1_4.dll is not found error)
#I manually force it to use my directx sdk xinput.
#TODO: Use DX_LIBS or whatever the variable is, to specify the 2010 dx XInput.lib explicitly.
set(XINPUTLIB D:\\Program\ Files\ \(x86\)\\Microsoft\ DirectX\ SDK\ \(June\ 2010\)\\Lib\\x86\\XInput.lib)
#D:\Program Files (x86)\Windows Kits\10\Lib\10.0.17134.0\um\x86\Version.Lib
#Version.Lib is required for looking up current EXE's runtime information.
target_link_libraries(dinput8 ${MARIADB_LIBRARY} ${LUA_GLOB} ${DirectX_LIBRARIES} dxguid.lib ${XINPUTLIB} Version.Lib ${DETOURS_GLOB})

set(CMAKE_PDB_OUTPUT_DIRECTORY "${HaloCE_ROOT_DIR}")
set(LIBRARY_OUTPUT_PATH "${HaloCE_ROOT_DIR}")

set_target_properties(dinput8 PROPERTIES OUTPUT_NAME "dinput8")

