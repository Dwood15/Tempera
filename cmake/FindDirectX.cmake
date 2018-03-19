#lifted off adobe's webkit repo (lol)

# - Find DirectX SDK installation
# Find the DirectX includes and library
# This module defines
#  DirectX_INCLUDE_DIRS, where to find d3d9.h, etc.
#  DirectX_LIBRARIES, libraries to link against to use DirectX.
#  DirectX_FOUND, If false, do not try to use DirectX.
#  DirectX_ROOT_DIR, directory where DirectX was installed.
#message(STATUS "FINDING DIRECTX")

find_path(DirectX_INCLUDE_DIRS d3dx9.h PATHS
		  "D:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include"
		  ${DXSDK_DIR}/Include
		  NO_DEFAULT_PATH)

message(STATUS "Found DX include path.") #: ${DirectX_INCLUDE_DIRS}")

find_path(DirectX_LIBRARY_PATHS d3dx9.lib PATHS
		  "$ENV{DXSDK_DIR}/Lib/x86"
		  "$ENV{DXSDK_DIR}/Lib/"
		  "$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Lib/x86"
		  "$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Lib"
		  "$ENV{DXSDK_DIR}/Lib/x64"
		  "$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Lib/x64"
		  "D:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86"
		  NO_DEFAULT_PATH)

get_filename_component(DirectX_ROOT_DIR "${DirectX_INCLUDE_DIRS}/.." ABSOLUTE)

message(STATUS "Found DX Libs") #: ${DirectX_LIBRARY_PATHS}")

find_library(DirectX_D3D9_LIBRARY d3d9 ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)
find_library(DirectX_D3DX9_LIBRARY d3dx9 ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)

set(DirectX_LIBRARIES ${DirectX_D3D9_LIBRARY} ${DirectX_D3DX9_LIBRARY})

# handle the QUIETLY and REQUIRED arguments and set DirectX_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DirectX DEFAULT_MSG DirectX_ROOT_DIR DirectX_LIBRARIES DirectX_INCLUDE_DIRS)
mark_as_advanced(DirectX_INCLUDE_DIRS DirectX_D3D9_LIBRARY DirectX_D3DX9_LIBRARY)

set(DirectX_FOUND TRUE)
