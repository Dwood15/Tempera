#I can't help but feel like this whole bit right here is needlessly verbose....
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bipeds)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/field)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/units)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vehicles)

set(O_WEAPONS_H
#	${CMAKE_CURRENT_SOURCE_DIR}/weapon/weapons.h
	)

set(OBJECTS_CXX
	PARENT_SCOPE)

set(OBJECTS_H
#	${CMAKE_CURRENT_SOURCE_DIR}/damage.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/damage_upgrades.h
#	${CMAKE_CURRENT_SOURCE_DIR}/grenade_hooks.inl
#	${CMAKE_CURRENT_SOURCE_DIR}/index_resolution.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/object_definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/object_lights.h
#	${CMAKE_CURRENT_SOURCE_DIR}/object_structures.h
#	${CMAKE_CURRENT_SOURCE_DIR}/object_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/objects.h
#	${CMAKE_CURRENT_SOURCE_DIR}/objects_yelo.h
#	${CMAKE_CURRENT_SOURCE_DIR}/placeholder_definitions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/scenario_definitions_structures.h
#	${CMAKE_CURRENT_SOURCE_DIR}/scenery_definitions.hpp
#	${O_BIPEDS_H}
#	${O_FIELD_H}
#	${O_UNITS_H}
#	${O_WEAPONS_H}
#	${O_VEHICLES_H}
	PARENT_SCOPE)
