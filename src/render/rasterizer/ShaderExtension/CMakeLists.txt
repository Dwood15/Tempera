set(EXT_SHADERS_H
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderExtension.h
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderExtension_Effect.inl
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderExtension_Environment.inl
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderExtension_Model.inl
	PARENT_SCOPE)

set(EXT_SHADERS_CXX
#	${CMAKE_CURRENT_SOURCE_DIR}/c_packed_texture_loader.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/device_hooks.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/dx9.h
#	${CMAKE_CURRENT_SOURCE_DIR}/dx9.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderExtension.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/rasterizer.h
#	${CMAKE_CURRENT_SOURCE_DIR}/render_target_chain.h

	PARENT_SCOPE)
