/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Fade/c_shader_fade.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Fade
	{
#ifdef EXTERNAL_SUBSYSTEM_SHADERS

#define SUBSYSTEM_FADE
#include "Rasterizer/PostProcessing/PostProcessing_ExternalSubsystemPaths.inl"
#undef SUBSYSTEM_FADE

#endif
		/////////////////////////////////////////////////
		// member accessors
		/*!
		 * \brief
		 * Sets the shader definition for the fade shader class.
		 * 
		 * \param definition
		 * The fade shader definition.
		 * 
		 * Sets the shader definition for the fade shader class.
		 * 
		 * \see
		 * TagGroups::s_shader_postprocess_definition
		 */
		void c_shader_fade::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
		{
			m_members_fade.definition = CAST_PTR(s_shader_fade_definition*, definition);
			c_shader_postprocess::SetShaderDefinition(definition);
		}

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
		void c_shader_fade::SetupShader()
		{
			YELO_ASSERT_DISPLAY(m_members_fade.shader_data == nullptr, "Shader data object has not been deleted and NULLed before being set again.");

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
			m_members_fade.shader_data = new c_shader_data_external();
			m_members_fade.shader_data->Ctor();
			m_members_fade.shader_data->SetFilePath(g_external_subsystem_shader_path, g_external_subsystem_include_path);
#else
			m_members_fade.shader_data = new c_shader_data_pp_shaders();
			m_members_fade.shader_data->Ctor();
			m_members_fade.shader_data->SetShaderID("PP_EffectFade");
#endif
			SetSourceData(m_members_fade.shader_data);
		}

		/*!
		 * \brief
		 * Retrieves the D3DXHANDLE's for the fade parameters.
		 * 
		 * Retrieves the D3DXHANDLE's for the fade parameters.
		 */
		void c_shader_fade::GetHandles()
		{
			c_shader_postprocess::GetHandles();

			m_members_fade.definition->fade_amount_handle.Initialize(m_members.definition->runtime.dx_effect, "c_fade_amount");
		}

		/*!
		 * \brief
		 * Null's the fade parameter handles
		 * 
		 * Null's the fade parameter handles
		 */
		void c_shader_fade::ClearHandles()
		{
			c_shader_postprocess::ClearHandles();

			m_members_fade.definition->fade_amount_handle.ClearHandles();
		}

		/*!
		 * \brief
		 * Validates the fade shader class.
		 * 
		 * \returns
		 * true if the shader is valid.
		 * 
		 * Validates the fade shader class. The shader is only valid if all of the parameter handls are present in the shader.
		 */
		bool c_shader_fade::ValidateImpl()
		{
			if(!c_shader_postprocess::ValidateImpl())
				return false;

			bool valid = false;
			do
			{
				if(!m_members_fade.definition->fade_amount_handle.IsUsed()) break;

				valid = true;
			}while(false);

			return valid;
		}
	}; }; };
};
#endif