# Lifted off Aobe Webkit repo

# - Find DirectX SDK installation
# Find the DirectX includes and library
# This module defines
#  DirectX_INCLUDE_DIRS, where to find d3d9.h, etc.
#  DirectX_LIBRARIES, libraries to link against to use DirectX.
#  DirectX_FOUND, If false, do not try to use DirectX.
#  DirectX_ROOT_DIR, directory where DirectX was installed.
#message(STATUS "FINDING DIRECTX")

message(STATUS "FindDirectX, CMAKE current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

find_path(DirectX_LIBRARY_PATHS d3dx9.lib xinput.lib PATHS
		${CMAKE_CURRENT_SOURCE_DIR}/libs/
		"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86"
		"$ENV{DXSDK_DIR}/Lib/x86/"
		"$ENV{DXSDK_DIR}/Lib/"
		"$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Lib/x86/"
		"$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Lib/"
		  NO_DEFAULT_PATH)

message(STATUS "DX Libs ${DirectX_LIBRARY_PATHS}")

find_library(DirectX_XINPUT_LIBRARY xinput ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)
find_library(DirectX_D3D9_LIBRARY d3d9 ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)
find_library(DirectX_D3DX9_LIBRARY d3dx9 ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)

find_path(DirectX_HEADER_DIRS d3dx9.h PATHS
		${CMAKE_CURRENT_SOURCE_DIR}/include/dxredist
		"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include"
		$ENV{DXSDK_DIR}/Include
		${DXSDK_DIR}/Include
		NO_DEFAULT_PATH)

message(STATUS "DX include path: ${DirectX_HEADER_DIRS}")

get_filename_component(DirectX_ROOT_DIR "${DirectX_HEADER_DIRS}/.." ABSOLUTE)

set(DirectX_LIBRARIES ${DirectX_D3D9_LIBRARY} ${DirectX_D3DX9_LIBRARY} ${DirectX_XINPUT_LIBRARY})

# handle the QUIETLY and REQUIRED arguments and set DirectX_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DirectX DEFAULT_MSG DirectX_ROOT_DIR DirectX_LIBRARIES DirectX_HEADER_DIRS)
mark_as_advanced(DirectX_HEADER_DIRS DirectX_D3D9_LIBRARY DirectX_D3DX9_LIBRARY DirectX_XINPUT_LIBRARY)

set(DirectX_FOUND TRUE)
