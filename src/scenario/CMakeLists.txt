set(S_YELO_H
	${CMAKE_CURRENT_SOURCE_DIR}/yelo/yelo_global_definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/yelo/yelo_scenario.h
	)

set(SCENARIO_SRC
	PARENT_SCOPE)

set(SCENARIO_H
	${S_YELO_H}
	${CMAKE_CURRENT_SOURCE_DIR}/scenario.h
	${CMAKE_CURRENT_SOURCE_DIR}/definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/structure_bsp.h
	PARENT_SCOPE)
