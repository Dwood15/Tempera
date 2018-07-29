#Project: Tempera
#File: CMakeLists.txt
#Copyright � 2018 dwood
# - GNU GPLv3

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
unset(CMAKE_SHARED_LIBRARY_PREFIX)

#To guarantee we're building for 32 bit windows.
unset(CMAKE_CL_64)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

set(LUA_Debug true)

set(PROJECT_NAME tempera)

project(${PROJECT_NAME} C CXX)

set(TARGET_GAME_VER "HALO_CE_1_10")

add_subdirectory(runtime)

set(COMMON ${CMAKE_CURRENT_SOURCE_DIR}/common/)

include_directories(AFTER ${GAME_LIB_DIRS} ${COMMON})

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

set(CMAKE_C_COMPILER C:\\Program\ Files\\LLVM\\bin\\clang-cl.exe)
set(CMAKE_CXX_COMPILER C:\\Program\ Files\\LLVM\\bin\\clang-cl.exe)

set(CMAKE_C_COMPILER C:\\Program\ Files\\LLVM\\bin\\clang)
set(CMAKE_C_FLAGS "-Wall -std=c11 -m32 -fms-extensions -fms-compatibility")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O4 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_CXX_COMPILER C:\\Program\ Files\\LLVM\\bin\\clang++)
#-fdelayed-template-parsing

#-wno-switch b/c of: warning: 19 enumeration values not handled in switch:
set(CLANG_DISABLED_WARNINGS "-Wno-deprecated -Wno-unused-function -Wno-microsoft-include -Wno-switch -Wno-overloaded-virtual")

set(CLANG_MS_WARNINGS " -Wmicrosoft-charize -Wextra-semi -Wmicrosoft-cpp-macro -Wmicrosoft-enum-value")
set(CLANG_EXTRAS "-Wc++2a-extensions -Wc++2a-compat ${CLANG_MS_WARNINGS}")
set(CLANG_ENABLED_WARNINGS "-Wall -Warray-bounds-pointer-arithmetic -Watomic-properties -Wassign-enum -Wc++11-extra-semi -Wcomma -Wpadded -Wmismatched-tags -Wfor-loop-analysis -Wmissing-braces -Wmissing-method-return-type  ")

set(CLANG_MACROS "-DDIRECTINPUT_VERSION=0x800 -DWIN32 -D_USRDLL -D_WINDLL -DCMAKE_LINK_WHAT_YOU_USE=TRUE")
set(CLANG_EXTRA_SETTINGS "-ferror-limit=1 -fms-compatibility -fms-extensions -fmodules -fmodules-cache-path=${CMAKE_SOURCE_DIR}/cache")

set(CMAKE_CXX_FLAGS "-M -std=c++17 -m32 ${CLANG_EXTRA_SETTINGS} ${CLANG_ENABLED_WARNINGS} ${CLANG_DISABLED_WARNINGS} ${CLANG_MACROS}")

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_AR C:\\Program\ Files\\LLVM\\bin\\llvm-ar)
set(CMAKE_LINKER C:\\Program\ Files\\LLVM\\bin\\lld-link)
set(CMAKE_NM C:\\Program\ Files\\LLVM\\bin\\llvm-nm)
set(CMAKE_OBJDUMP C:\\Program\ Files\\LLVM\\bin\\llvm-objdump)
set(CMAKE_RANLIB C:\\Program\ Files\\LLVM\\bin\\llvm-ranlib)

include_directories(BEFORE ${DirectX_INCLUDE_DIRS} ${INCLUDE}/detours ${LUA_INCLUDE_DIR} ${MARIADB_INCLUDE_DIR})

# Defines Debug by default.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
	message(STATUS "Build type not specified: Using Debug by default.")
endif (NOT CMAKE_BUILD_TYPE)

add_definitions(-D_WINDOWS -D_USRDLL)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_COMPILER_ID "Clang")

	message(STATUS "Forcing GNU to Clang compiler because CLion is dumb.")
	#TODO: SET CLANG STUFFS.
	set(MSVC 0)
elseif (MSVC)
	#vc++ 2017 toolset. Available from microsoft here: http://landinghub.visualstudio.com/visual-cpp-build-tools
	if (NOT CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION OR NOT "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}" STREQUAL "10.0.17134.0")
		#message(WARNING "CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION is empty! Could not find applicable windows 10 sdk.")
		set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 10.0.17134.0)
		#TODO: CHECK FOR AVAILABLE SDK VERSIONS IF NONE SET
	endif ()

	#	set(COMPILER_REL_FLDR msvc)
	#	set(CMAKE_C_FLAGS '')
	#	set(CMAKE_CXX_FLAGS /std:c++latest\ /MP\ /MTd\ )
	#	set(CMAKE_CXX_FLAGS_DEBUG " /MP\ /MTd\ /Gy- /W3\ /Od\ /Zi\ /MP /EHsc\ /fp:fast\ /std:c++latest\ /arch:SSE2 /D\ \"WIN32\"\ /D\ \"_WINDOWS\"\ /D \"_USRDLL\"\ /D\ \"_WINDLL\"\ /D_CRT_SECURE_NO_WARNINGS")

	# 4070 : Silences `OUT directive differs from filename ignoring directive` (warning)
	#		4099 :
	set(BASE_FLAGS "/MANIFEST\ /NXCOMPAT\ /DEBUG\ /MACHINE:X86\ /SAFESEH:NO\ /INCREMENTAL:NO\ /SUBSYSTEM:WINDOWS\ /MANIFESTUAC:NO\ /OPT:NOICF\ /NOLOGO\ /ignore:4099\ /ignore:4070")

	set(LINKER_FLAGS "${BASE_FLAGS} /ASSEMBLYDEBUG\ /DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/Tempera.def\"")
	#rip alignment on win32
	set(CMAKE_SHARED_LINKER_FLAGS ${LINKER_FLAGS})
else ()
	message(STATUS "Unrecognized compiler/suite: ${CMAKE_CXX_COMPILER_ID}")
endif ()

#EXTERNAL LIBS
file(GLOB DETOURS_GLOB libs/detours.lib)
file(GLOB LUA_GLOB libs/LUA.lib)

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
	common/enums/effects_enums.h
	common/enums/generic_enums.h
	common/enums/hs_enums.h
	common/enums/objects_enums.h
	common/enums/ai_enums.h
	common/enums/scenario_enums.h
	common/enums/geometry_enums.h
	src/core_types.h
	src/tags/group/tagdata.h
	src/tags/group/tagblock.h
	src/scenario/scenario_location.h
	src/hs/hs_value_union.h src/memory/data_iterator.h)

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
#set(XINPUTLIB "D:/Program\ Files\ \(x86\)/Microsoft\ DirectX\ SDK\ \(June\ 2010\)/Lib/x86/XInput.lib")
#D:\Program Files (x86)\Windows Kits\10\Lib\10.0.17134.0\um\x86\Version.Lib
#Version.Lib is required for looking up current EXE's runtime information.
target_link_libraries(dinput8 ${MARIADB_LIBRARY} ${LUA_GLOB} ${DirectX_LIBRARIES} dxguid.lib ${XINPUTLIB} Version.Lib ${DETOURS_GLOB})

set(CMAKE_PDB_OUTPUT_DIRECTORY "${HaloCE_ROOT_DIR}")
set(LIBRARY_OUTPUT_PATH "${HaloCE_ROOT_DIR}")

set_target_properties(dinput8 PROPERTIES OUTPUT_NAME "dwood_dinput8")

