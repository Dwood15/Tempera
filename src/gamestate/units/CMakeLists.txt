set(UNITS_H
	${CMAKE_CURRENT_SOURCE_DIR}/unit_controls.h
	${CMAKE_CURRENT_SOURCE_DIR}/unit_speech.h
	PARENT_SCOPE)

set(UNITS_SRC

	PARENT_SCOPE)

add_custom_target("units" SOURCES ${UNITS_H})
