/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/FXAA/c_shader_fxaa.hpp"
#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace FXAA
	{
#ifdef EXTERNAL_SUBSYSTEM_SHADERS

#define SUBSYSTEM_FXAA
#include "Rasterizer/PostProcessing/PostProcessing_ExternalSubsystemPaths.inl"
#undef SUBSYSTEM_FXAA

#endif
		/////////////////////////////////////////////////
		// shader setup
		/*!
		 * \brief
		 * Sets up the shader classes shader data object.
		 * 
		 * Sets up the shader classes shader data object. When EXTERNAL_SUBSYSTEM_SHADERS is define the shader data object will be
		 * a c_shader_data_external, otherwise it will be a c_shader_data_pp_shaders;
		 * 
		 * \see
		 * c_shader_data_external | c_shader_data_pp_shaders
		 */
		void c_shader_fxaa::SetupShader()
		{
			YELO_ASSERT_DISPLAY(m_members_fxaa.shader_data == nullptr, "Shader data object has not been deleted and NULLed before being set again.");

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
			m_members_fxaa.shader_data = new c_shader_data_external();
			m_members_fxaa.shader_data->Ctor();
			m_members_fxaa.shader_data->SetFilePath(g_external_subsystem_shader_path, g_external_subsystem_include_path);
#else
			m_members_fxaa.shader_data = new c_shader_data_pp_shaders();
			m_members_fxaa.shader_data->Ctor();
			m_members_fxaa.shader_data->SetShaderID("PP_FXAA");
#endif
			SetSourceData(m_members_fxaa.shader_data);
		}
	}; }; };
};
#endif