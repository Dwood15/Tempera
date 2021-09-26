#Project: Tempera
# Copyright � 2021 Dason Woodhouse
# - GNU GPLv3 Or Later

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
message(STATUS "cmake version ${CMAKE_VERSION}")

#Apparently having this set to 20 causes CMAKE to set std:c++17. GOD KNOWS WHY
#set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_EXTENSIONS TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#force cmake to set the name of the dll to dinput8.
unset(CMAKE_SHARED_LIBRARY_PREFIX)
#Guarantee that we're targeting 32-bit.
unset(CMAKE_CL_64)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

set(LUA_Debug true)

set(PROJECT_NAME tempera)

#Latest version of CMAKE... is a bitch.
#https://cmake.org/cmake/help/v3.15/policy/CMP0091.html#policy:CMP0091
#Have to do this in order to statically link the MT dll below
cmake_policy(SET CMP0091 NEW)

project(${PROJECT_NAME} C CXX)

set(TARGET_GAME_VER "HALO_CE_1_10")

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}/runtime ${CMAKE_CURRENT_SOURCE_DIR}/common/)

find_path(DX_PATH FindDirectX.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)

find_path(LUA_PATH FindLua.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LUA_PATH} ${DX_PATH} ${GAME_PATH})

find_package(HaloCE REQUIRED)
find_package(Lua REQUIRED)
find_package(DirectX REQUIRED)

message(STATUS "Post-Find LUA debug: LUA_LIBRARY: ${LUA_LIBRARY}\ LUA_LIBRARIES: ${LUA_LIBRARIES}\ LUA_INCLUDE_DIR: ${LUA_INCLUDE_DIR}")

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(BEFORE ${DirectX_INCLUDE_DIRS}
        ${LUA_INCLUDE_DIR}
        ${INCLUDE}/detours)

add_definitions(-D_WINDOWS -D_USRDLL)

if (MSVC)
    add_compile_options("/std:c++latest")

    #vc++ 2017 toolset. Available from microsoft here: http://landinghub.visualstudio.com/visual-cpp-build-tools
    if (NOT CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION OR NOT "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}" STREQUAL "10.0.17134.0")
        #message(WARNING "CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION is empty! Could not find applicable windows 10 sdk.")
        set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 10.0.17134.0)
        #TODO: CHECK FOR AVAILABLE SDK VERSIONS IF NONE SET
    endif ()

    set(CMAKE_CXX_COMPILER_TARGET i686-pc-win32-msvc)
    set(COMPILER_MACROS "/D\ \"WIN32\"\ /D\ \"_WINDOWS\"\ /D \"_USRDLL\"\ /D\ \"_WINDLL\"\ /D_CRT_SECURE_NO_WARNINGS")

    #/Zc flag tells C++ what version of cpp we're using/targeting.
    #/Qpar-report:2 - Tells us about the compiler's optimizations of our for loops.
    #/std:c++latest implies C++ 20. /Qpar-report:2 /Zp2\
    set(NEW_FEATURES "/std:c++latest\ /wo4068")

	#Apparently MTd is going out of fashion?
    set(CMAKE_CXX_FLAGS_DEBUG "/TP\ /MP\ /MTd\ /Gy- /W3\ /Od\ /Zi\ /MP\ /EHsc\ ${NEW_FEATURES}\ /fp:fast\ /arch:SSE2\ ${COMPILER_MACROS}")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})

    # 4070 : Silences `OUT directive differs from filename ignoring directive` (warning)
    #		4099 :
    #LIBCMT conflicts with another lib we're already using, so we have to NODEFAULTLIB it.
	# /NODEFAULTLIB:LIBCMT\
    set(BASE_FLAGS "/MANIFEST\ /NXCOMPAT\ /DEBUG\ /MACHINE:X86\ /NODEFAULTLIB:LIBCMT\ /SAFESEH:NO\ /INCREMENTAL:NO\ /SUBSYSTEM:WINDOWS\ /MANIFESTUAC:NO\ /OPT:NOICF\ /NOLOGO\ /ignore:4099\ /ignore:4070")
	message(WARNING "LINKER FLAGS: ${BASE_FLAGS}")

    set(LINKER_FLAGS "${BASE_FLAGS} /ASSEMBLYDEBUG\ /debug:fastlink\ /DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/Tempera.def\"")

	message(WARNING "LINKER FLAGS: ${LINKER_FLAGS}")

	#rip alignment on win32
    set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})
else ()
    message(STATUS "Unrecognized compiler/suite: ${CMAKE_CXX_COMPILER_ID}")
endif ()

#EXTERNAL LIBS
file(GLOB DETOURS_GLOB libs/detours.lib)
file(GLOB LUA_GLOB libs/lua53.lib)

#BEGIN RUNTIME Cmake

#there was intention at one point to support statically loading modules for different versions of the game
#it's mostly just been left alone.
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

find_path(GAME_PATH FindHaloCE.cmake PATHS ./cmake REQUIRED)
#END RUNTIME location

add_library(dinput8 SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/runtime/ce/1_10/110EngineManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/runtime/hek/sapien/sapienEngineManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/runtime/ce/1_10/function_rewrite.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/common/engine_interface.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/tags/group/base.cpp
        #	${CMAKE_CURRENT_SOURCE_DIR}/tags/group/groups_structures.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tags/group/tag_groups.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tags/group/c_tag_iterator.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/gamestate/units/units.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gamestate/objects.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gamestate/camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gamestate/objects/objectcontroller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gamestate/controls.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gamestate/players.cpp


        ${CMAKE_CURRENT_SOURCE_DIR}/src/Direct3D/d3d9hook.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cseries/random.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Dinput/dinput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/engine_functions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RuntimeManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/forge.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/math/math.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions/exception_handler.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    	${CMAKE_CURRENT_SOURCE_DIR}/common/addlog.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/memory/data.cpp)

#The compiler is defaulting to the Windows 10 SDK, which requires external DLL's (xinput1_4.dll is not found error)
#I manually force it to use my directx sdk xinput.
#TODO: Use DX_LIBS or whatever the variable is, to specify the 2010 dx XInput.lib explicitly.
#set(XINPUTLIB "C:/Program\ Files\ \(x86\)/Microsoft\ DirectX\ SDK\ \(June\ 2010\)/Lib/x86/XInput.lib")

#D:\Program Files (x86)\Windows Kits\10\Lib\10.0.17134.0\um\x86\Version.Lib
#Version.Lib is required for looking up current EXE's runtime information.

message(WARNING "DX Libs: ${DirectX_LIBRARIES}")

target_link_libraries(dinput8 ${LUA_GLOB} ${DirectX_LIBRARIES} Version.Lib ${DETOURS_GLOB})
set_property(TARGET dinput8 PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#Files we don't actually give a shit about are getting exported to the build directory.
#This will (at least) clean them out.
set(ADDITIONAL_CLEAN_FILES  "${HaloCE_ROOT_DIR}/dinput8.dll ${HaloCE_ROOT_DIR}/dinput8.ilk " ${ADDITIONAL_CLEAN_FILES})

set(CMAKE_PDB_OUTPUT_DIRECTORY "${HaloCE_ROOT_DIR}")
set(LIBRARY_OUTPUT_PATH "${HaloCE_ROOT_DIR}")

set_target_properties(dinput8 PROPERTIES OUTPUT_NAME "dinput8")
