# Find YamlCpp config loader installation
# Find the YamlCpp includes and library
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
if (NOT YAMLCPP_INCLUDE_DIR)
	if (WIN32)
		find_path(YAMLCPP_INCLUDED_DIR mysql.h
				  $ENV{YAMLCPP_INCLUDED_DIR}
				  $ENV{YAMLCPP_DIR}/include
				  $ENV{YAMLCPP_DIR}/include/mariadb
				  ${PFILES}/MariaDB/*/include)
	else ()
		find_path(YAMLCPP_BIN_DIR mariadb_config
				  $ENV{YAMLCPP_DIR}/bin
				  ${YAMLCPP_DIR}/bin)

		find_path(YAMLCPP_INCLUDED_DIR mysql.h
				  $ENV{YAMLCPP_INCLUDED_DIR}
				  $ENV{YAMLCPP_DIR}/include
				  $ENV{YAMLCPP_DIR}/include/mariadb)
	endif ()
endif ()

if (YAMLCPP_INCLUDED_DIR)
	message(STATUS "Found MariaDB includes: ${YAMLCPP_INCLUDED_DIR}")
endif ()

if (WIN32)
	#cmake got stubborn, even between multiple forced cache reloads...
	set(YAMLCPP_LIBRARY_DIR "C:\\Users\\Dwood\\Documents\\GitHub\\TemperaRedux\\libs")
	# Try to find yamlcpp static library
	find_library(YAMLCPP_LIBRARY yamlcpp.lib
				 ${YAMLCPP_LIBRARY_DIR}
				 $ENV{YAMLCPP_LIBRARY_DIR}
				 $ENV{YAMLCPP_DIR}/lib/
				 $ENV{YAMLCPP_LIBRARY}
				 $ENV{YAMLCPP_DIR}/lib
				 NO_DEFAULT_PATH)

	if (YAMLCPP_LIBRARY)
		get_filename_component(YAMLCPP_LIBRARY_DIR ${YAMLCPP_LIBRARY} PATH)
	endif ()
else ()
	message(FATAL_ERROR "ONLY WIN32 SUPPORT FOR NOW")
endif ()

if (YAMLCPP_LIBRARY_DIR AND YAMLCPP_INCLUDED_DIR)
	message(STATUS "Found YamlCpp libraries: ${YAMLCPP_LIBRARY_DIR}")
	set(YAMLCPP_FOUND TRUE)
else ()
	message(STATUS "YamlCpp not found. Includes: ${YAMLCPP_INCLUDED_DIR}, Libs: ${YAMLCPP_LIBRARY_DIR}")
endif ()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(YamlCpp DEFAULT_MSG YAMLCPP_LIBRARY YAMLCPP_INCLUDED_DIR)

mark_as_advanced(YAMLCPP_INCLUDED_DIR YAMLCPP_LIBRARY)
