#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/interpolation)

set(TIME_H
#	${INTERP_H}
	${CMAKE_CURRENT_SOURCE_DIR}/time.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/time_structures.hpp
	PARENT_SCOPE)
