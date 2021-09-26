# - Find DirectX SDK installation
# Find the DirectX includes and library
# This module defines
#  DirectX_INCLUDE_DIRS, where to find d3d9.h, etc.
#  DirectX_LIBRARIES, libraries to link against to use DirectX.
#  DirectX_FOUND, If false, do not try to use DirectX.
#  DirectX_ROOT_DIR, directory where DirectX was installed.

find_path(DirectX_LIBRARY_PATHS d3dx9.lib d3d9.lib xinput.lib PATHS
		$ENV{DXSDK_DIR}/Lib/
		$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Lib/
		"C:\\Program\ Files\ (x86)\\Microsoft\ DirectX\ SDK\ (June 2010)\\Lib\\x86"
		${CMAKE_CURRENT_SOURCE_DIR}/libs/
		$ENV{DXSDK_DIR}/Lib/x86)


find_library(DirectX_XINPUT_LIBRARY xinput ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)
find_library(DirectX_D3D9_LIBRARY d3d9 ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)
find_library(DirectX_D3DX9_LIBRARY d3dx9 ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)

find_path(DirectX_INCLUDE_DIRS d3dx9.h d3dx9math.h PATHS
		${CMAKE_CURRENT_SOURCE_DIR}/include/dxredist
		"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include"
		$ENV{DXSDK_DIR}/Include)

get_filename_component(DirectX_ROOT_DIR "${DirectX_INCLUDE_DIRS}/.." ABSOLUTE)

set(DirectX_LIBRARIES ${DirectX_D3D9_LIBRARY} ${DirectX_D3DX9_LIBRARY} ${DirectX_XINPUT_LIBRARY})

message(STATUS "FindDirectX, CMAKE current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "DX Header DIRS: ${DirectX_INCLUDE_DIRS}")
message(STATUS "DX Libs ${DirectX_LIBRARY_PATHS}")
message(STATUS "DX Libraries: ${DirectX_LIBRARIES}")

# handle the QUIETLY and REQUIRED arguments and set DirectX_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DirectX DEFAULT_MSG DirectX_ROOT_DIR DirectX_LIBRARIES DirectX_INCLUDE_DIRS)
mark_as_advanced(DirectX_INCLUDE_DIRS DirectX_D3D9_LIBRARY DirectX_D3DX9_LIBRARY DirectX_XINPUT_LIBRARY)

set(DirectX_FOUND TRUE)
