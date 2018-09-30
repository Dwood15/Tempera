set(POSTPROCESSING_CXX
	${CMAKE_CURRENT_SOURCE_DIR}/c_effect_instance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/c_effect_postprocess.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/c_effect_render_set.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/c_post_processing_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/c_quad_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/c_shader_include_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/c_shader_instance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/c_shader_posprocess.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PostProcessing.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Scripting.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PostProcessingErrorReporting.cpp

	PARENT_SCOPE)

set(POSTPROCESSING_H
	${CMAKE_CURRENT_SOURCE_DIR}/c_effect_instance.h
	${CMAKE_CURRENT_SOURCE_DIR}/c_effect_postprocess.h
	${CMAKE_CURRENT_SOURCE_DIR}/c_effect_render_set.h
	${CMAKE_CURRENT_SOURCE_DIR}/c_post_processing_main.h
	${CMAKE_CURRENT_SOURCE_DIR}/c_quad_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/c_shader_include_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/c_shader_instance.h
	${CMAKE_CURRENT_SOURCE_DIR}/c_shader_posprocess.h
	${CMAKE_CURRENT_SOURCE_DIR}/PostProcessing.h
	${CMAKE_CURRENT_SOURCE_DIR}/Scripting.h
	${CMAKE_CURRENT_SOURCE_DIR}/PostProcessingErrorReporting.h
	PARENT_SCOPE)
