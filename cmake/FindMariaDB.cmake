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
if (NOT MARIADB_INCLUDE_DIR)
	if (WIN32)
		find_path(MARIADB_INCLUDE_DIR mysql.h
				  $ENV{MARIADB_INCLUDE_DIR}
				  $ENV{MARIADB_DIR}/include
				  $ENV{MARIADB_DIR}/include/mariadb
				  ${PFILES}/MariaDB/*/include)
	else ()
		find_path(MARIADB_BIN_DIR mariadb_config
				  $ENV{MARIADB_DIR}/bin
				  ${MARIADB_DIR}/bin)

		find_path(MARIADB_INCLUDE_DIR mysql.h
				  $ENV{MARIADB_INCLUDE_DIR}
				  $ENV{MARIADB_DIR}/include
				  $ENV{MARIADB_DIR}/include/mariadb)
	endif ()
endif ()

if (MARIADB_INCLUDE_DIR)
	message(STATUS "Found MariaDB includes: ${MARIADB_INCLUDE_DIR}")
endif ()

if (WIN32)
	#cmake got stubborn, even between multiple forced cache reloads...
	set(MARIADB_LIBRARY_DIR "C:/Program\ Files\ (x86)/MariaDB/MariaDB\ Connector\ C/lib")
	# Try to find mariadb client libraries
	find_library(MARIADB_LIBRARY libmariadb.lib
				 ${MARIADB_LIBRARY_DIR}
				 $ENV{MARIADB_LIBRARY_DIR}
				 $ENV{MARIADB_DIR}/lib/
				 $ENV{MARIADB_LIBRARY}
				 $ENV{MARIADB_DIR}/lib
				 NO_DEFAULT_PATH)

	if (MARIADB_LIBRARY)
		get_filename_component(MARIADB_LIBRARY_DIR ${MARIADB_LIBRARY} PATH)
	endif ()
else ()
	find_path(MARIADB_BIN_DIR mariadb_config
			  $ENV{MARIADB_DIR}/bin
			  ${MARIADB_DIR}/bin)
endif ()

if (MARIADB_LIBRARY_DIR AND MARIADB_INCLUDE_DIR)
	message(STATUS "Found MariaDB libraries: ${MARIADB_LIBRARY_DIR}")
	set(MARIADB_FOUND TRUE)
else ()
	message(STATUS "MariaDB not found. Includes: ${MARIADB_INCLUDE_DIR}, Libs: ${MARIADB_LIBRARY_DIR}")
endif ()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(MariaDB DEFAULT_MSG MARIADB_LIBRARY MARIADB_INCLUDE_DIR)

mark_as_advanced(MARIADB_INCLUDE_DIR MARIADB_LIBRARY)
