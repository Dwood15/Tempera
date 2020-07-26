# Find MariaDB connector SDK installation
# Find the MariaDB includes and library
# This module defines
#  MariaDB_INCLUDE_DIR, where to find headers.
#  MariaDB_LIBRARY, library to link against to use MDB odbc.
#  MariaDB_FOUND, If false, do not try to use MDB odbc.
#  MariaDB_ROOT_DIR, directory where MDB odbc was installed.
#
# find_path(MariaDB_INCLUDE_DIRS mysql.h PATHS
#		  C:/Program\ Files\ \(X86\)/MariaDB/MariaDB\ Connector\ C/include
#		  D:/Program\ Files\ \(X86\)/MariaDB/MariaDB\ Connector\ C/include
#		  NO_DEFAULT_PATH)
#
# get_filename_component(MariaDB_ROOT_DIR "${MariaDB_INCLUDE_DIRS}/.." ABSOLUTE)
#
# find_path(MariaDB_LIBRARY_PATHS libmariadb.lib PATHS
#		  C:/Program\ Files\ \(X86\)/MariaDB/MariaDB\ Connector\ C/lib
#		  D:/Program\ Files\ \(X86\)/MariaDB/MariaDB\ Connector\ C/lib
#		  NO_DEFAULT_PATH)
#
# find_library(MariaDB_LIBRARY libmariadb ${MariaDB_LIBRARY_PATHS} NO_DEFAULT_PATH)

# FindMariaDB.cmake

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(PFILES $ENV{ProgramW6432})
else ()
	set(PFILES $ENV{ProgramFiles})
endif ()

# Try to find the include directory
if (NOT TINYDNN_INCLUDE_DIR)
	if (WIN32)
		find_path(TINYDNN_INCLUDE_DIR tiny_dnn.h
				  $ENV{TINYDNN_INCLUDE_DIR}
				  $ENV{TINYDNN_INCLUDE_DIR}/tiny_dnn
				  $ENV{TINYDNN_DIR}/tiny_dnn
				  ${PFILES}/tiny-dnn/tiny_dnn
				  #TODO: Make this not suck
				  ${PROJECT_SOURCE_DIR}/../tinY-dnn/tiny_dnn
				  ${PROJECT_SOURCE_DIR}/include/tinY-dnn/tiny_dnn/
				  )
	else ()
		message(FATAL "Don't use 64 bit TDNN with halo ce. that's a bad idea.")
	endif ()
endif ()

if (TINYDNN_INCLUDE_DIR)
	message(STATUS "Found TinyDNN libraries: ${TINYDNN_INCLUDE_DIR}")
	set(TINYDNN_FOUND TRUE)
else ()
	message(STATUS "TINYDNN_INCLUDE_DIR not found. Includes: ${TINYDNN_INCLUDE_DIR}")
endif ()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(TinyDnn DEFAULT_MSG TINYDNN_INCLUDE_DIR)

mark_as_advanced(TINYDNN_INCLUDE_DIR)
