add_subdirectory(postprocessing)
add_subdirectory(ShaderExtension)

set(R_SHADERS_H
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/effect_postprocess_collection_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/effect_postprocess_generic_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_postprocess_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_postprocess_generic_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_postprocess_globals_definitions.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader_postprocess_runtime.inl
	)

set(RASTERIZER_CXX
	${EXT_SHADERS_CXX}
	${CMAKE_CURRENT_SOURCE_DIR}/GBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Lightmaps.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rasterizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenderTargetChain.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderDraw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Sky.cpp

	PARENT_SCOPE)

set(RASTERIZER_H
	${R_SHADERS_H}
	${EXT_SHADERS_H}
	${CMAKE_CURRENT_SOURCE_DIR}/geometry.h
	${CMAKE_CURRENT_SOURCE_DIR}/GBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Lightmaps.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rasterizer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenderTargetChain.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderDraw.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Sky.hpp
	PARENT_SCOPE)
