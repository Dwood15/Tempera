set(R_SHADERS_H
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/effect_postprocess_collection_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/effect_postprocess_generic_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_postprocess_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_postprocess_generic_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_postprocess_globals_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_postprocess_runtime.inl
	)

set(RASTERIZER_H
	${R_SHADERS_H}
#	${CMAKE_CURRENT_SOURCE_DIR}/c_packed_texture_loader.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/device_hooks.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/dx9.h
#	${CMAKE_CURRENT_SOURCE_DIR}/dx9.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/geometry.h
#	${CMAKE_CURRENT_SOURCE_DIR}/rasterizer.h
#	${CMAKE_CURRENT_SOURCE_DIR}/rasterizer.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/render_target_chain.h

	PARENT_SCOPE)
