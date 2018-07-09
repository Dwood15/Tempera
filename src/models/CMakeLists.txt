#Yeah, I know, I got lazy. But holey moley is it a pain to do so many of these different CMakeLists files.

set(M_ANIMATIONS_H
#	${CMAKE_CURRENT_SOURCE_DIR}/animations/definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/animations/model.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/animations/model_animation_definitions_yelo.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/animations/recorded_animation_definitions.h
#	${CMAKE_CURRENT_SOURCE_DIR}/animations/recorded_animation_playback.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/animations/recorded_animations_structures.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/animations/recorded_animations.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/animations/recorded_animation_playback_v1.hpp
	)

set(M_COLLISIONS_H
#	${CMAKE_CURRENT_SOURCE_DIR}/collision/bsp_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/collision/bsp.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/collision/features.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/collision/model_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/collision/models.hpp
	)

set(M_STRUCTURE_H
#	${CMAKE_CURRENT_SOURCE_DIR}/structure/bsp_definitions.hpp
	)

set(MODELS_H
	${M_ANIMATIONS_H}
	${M_COLLISIONS_H}
	${M_STRUCTURE_H}
#	${CMAKE_CURRENT_SOURCE_DIR}/collisions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/model_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/models.hpp
	PARENT_SCOPE)

