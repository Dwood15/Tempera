set(E_CONTRAIL_H
	${CMAKE_CURRENT_SOURCE_DIR}/contrail/contrail_definitions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/contrail/contrails.hpp
	)

set(E_DAMAGE_H 	${CMAKE_CURRENT_SOURCE_DIR}/damage/damage_effect_definitions.hpp )

set(E_MATERIAL_H ${CMAKE_CURRENT_SOURCE_DIR}/material/material_effect_definitions.hpp )

set(E_PARTICLE_H
	${CMAKE_CURRENT_SOURCE_DIR}/particle/weather/weather_particle_systems.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/particle/weather/weather_particle_system_definitions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/particle/particles.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/particle/particle_systems.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/particle/particle_system_definitions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/particle/particle_definitions.hpp
	)

set(E_PLAYER_H ${CMAKE_CURRENT_SOURCE_DIR}/player/player_effects.hpp )
set(E_SHADER_H ${CMAKE_CURRENT_SOURCE_DIR}/shader/shader_definitions.hpp )

set(E_SOUND_H
	${CMAKE_CURRENT_SOURCE_DIR}/sound/sound_definitions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/sound/sound_environment_definitions.hpp
	)

set(EFFECTS_H
#	${E_CONTRAIL_H}
#	${E_DAMAGE_H}
#	${E_MATERIAL_H}
#	${E_PARTICLE_H}
#	${E_PLAYER_H}
#	${E_SHADER_H}
#	${E_SOUND_H}

	${CMAKE_CURRENT_SOURCE_DIR}/effects_constants.h
#	${CMAKE_CURRENT_SOURCE_DIR}/effects.h
#	${CMAKE_CURRENT_SOURCE_DIR}/effect_definitions.hpp
	PARENT_SCOPE)
