#Project: Tempera
#Copyright � 2020 dwood
# - GNU GPLv3

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
message(STATUS "cmake version ${CMAKE_VERSION}")

#Apparently having this set to 20 causes CMAKE to set std:c++17
#because GOD KNOWS WHY
#set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_EXTENSIONS TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#this forces cmake to set the name of the dll to dinput8 correctly.
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

add_subdirectory(runtime)

include_directories(AFTER ${GAME_LIB_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/common/)

find_path(DX_PATH FindDirectX.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)

find_path(LUA_PATH FindLua.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)

#find_path(TDN_PATH FindTinyDNN.cmake PATHS ${PROJECT_SOURCE_DIR}/cmake REQUIRED)
#C:\Users\Dwood\Documents\GitHub\tiny-dnn\tiny_dnn
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LUA_PATH} ${DX_PATH} ${GAME_PATH} ${TDN_PATH})

find_package(HaloCE REQUIRED)
find_package(Lua REQUIRED)
#find_package(YamlCpp REQUIRED)

message(STATUS "Post-Find LUA debug: LUA_LIBRARY: ${LUA_LIBRARY}\ LUA_LIBRARIES: ${LUA_LIBRARIES}\ LUA_INCLUDE_DIR: ${LUA_INCLUDE_DIR}")

find_package(DirectX REQUIRED)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include_directories(BEFORE ${DirectX_INCLUDE_DIRS} ${INCLUDE}/detours ${LUA_INCLUDE_DIR})

add_definitions(-D_WINDOWS -D_USRDLL)
#TODO: Include GCC_CLANG/CMakeLists.txt

if (MSVC)
    set(CMAKE_VERBOSE_MAKEFILE ON)
    if (MSVC_VERSION GREATER_EQUAL "1900")
        include(CheckCXXCompilerFlag)
        CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
        if (_cpp_latest_flag_supported)
            message(STATUS "Add compiler options std cpp latest and experimental ${_cpp_latest_flag_supported}")
            add_compile_options("/std:c++latest")
        endif ()
    endif ()

    #vc++ 2017 toolset. Available from microsoft here: http://landinghub.visualstudio.com/visual-cpp-build-tools
    if (NOT CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION OR NOT "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}" STREQUAL "10.0.17134.0")
        #message(WARNING "CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION is empty! Could not find applicable windows 10 sdk.")
        set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 10.0.17134.0)
        #TODO: CHECK FOR AVAILABLE SDK VERSIONS IF NONE SET
    endif ()

    #CLANGD HELPER
    set(CMAKE_CXX_COMPILER_TARGET i686-pc-win32-msvc)

    set(COMPILER_MACROS "/D\ \"WIN32\"\ /D\ \"_WINDOWS\"\ /D \"_USRDLL\"\ /D\ \"_WINDLL\"\ /DDIRECTINPUT_VERSION=0x800\ /D_CRT_SECURE_NO_WARNINGS")

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

add_subdirectory(src)

add_library(dinput8 SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/common/engine_interface.cpp
        ${RUNTIME_CXX}
        ${SRC_CXX}
		${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    	${CMAKE_CURRENT_SOURCE_DIR}/common/addlog.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/memory/data.cpp)

#The compiler is defaulting to the Windows 10 SDK, which requires external DLL's (xinput1_4.dll is not found error)
#I manually force it to use my directx sdk xinput.
#TODO: Use DX_LIBS or whatever the variable is, to specify the 2010 dx XInput.lib explicitly.
set(XINPUTLIB "C:/Program\ Files\ \(x86\)/Microsoft\ DirectX\ SDK\ \(June\ 2010\)/Lib/x86/XInput.lib")

#D:\Program Files (x86)\Windows Kits\10\Lib\10.0.17134.0\um\x86\Version.Lib
#Version.Lib is required for looking up current EXE's runtime information.

#message(WARNING "Target name: ${TARGET_NAME}")

target_link_libraries(dinput8 ${LUA_GLOB} ${DirectX_LIBRARIES} dxguid.lib ${XINPUTLIB} Version.Lib ${DETOURS_GLOB})
set_property(TARGET dinput8 PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#Take tempera.init.lua script and export it with the build files
add_custom_target(copy-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lua_scripts
        ${HaloCE_ROOT_DIR}
        )
#without this, cmake doesn't know to export it on file changes.
add_dependencies(dinput8 copy-files)

#Files we don't actually give a shit about are getting exported to the build directory.
#This will (at least) clean them out.
set(ADDITIONAL_CLEAN_FILES  "${HaloCE_ROOT_DIR}/dinput8.dll ${HaloCE_ROOT_DIR}/dinput8.ilk " ${ADDITIONAL_CLEAN_FILES})

set(CMAKE_PDB_OUTPUT_DIRECTORY "${HaloCE_ROOT_DIR}")
set(LIBRARY_OUTPUT_PATH "${HaloCE_ROOT_DIR}")

set_target_properties(dinput8 PROPERTIES OUTPUT_NAME "dinput8")
