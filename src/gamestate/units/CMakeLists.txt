set(UNITS_H
	${CMAKE_CURRENT_SOURCE_DIR}/unit_controls.h
	${CMAKE_CURRENT_SOURCE_DIR}/unit_speech.h
	${CMAKE_CURRENT_SOURCE_DIR}/units.h
	${CMAKE_CURRENT_SOURCE_DIR}/definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/extended_definitions.h
	PARENT_SCOPE)

set(UNITS_CXX
	${CMAKE_CURRENT_SOURCE_DIR}/units.cpp
	PARENT_SCOPE)

add_custom_target("units" SOURCES ${UNITS_H})
