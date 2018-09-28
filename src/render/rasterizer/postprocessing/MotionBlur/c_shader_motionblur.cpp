/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_shader_motionblur.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace MotionBlur
	{
#ifdef EXTERNAL_SUBSYSTEM_SHADERS

#define SUBSYSTEM_MOTIONBLUR
#include "Rasterizer/PostProcessing/PostProcessing_ExternalSubsystemPaths.inl"
#undef SUBSYSTEM_MOTIONBLUR

#endif
		/////////////////////////////////////////////////
		// member accessors
		/*!
		 * \brief
		 * Sets the shader definition for the shader class.
		 * 
		 * \param definition
		 * The shader definition.
		 * 
		 * Sets the shader definition for the shader class.
		 * 
		 * \see
		 * TagGroups::s_shader_postprocess_definition
		 */
		void c_shader_motionblur::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
		{
			m_members_motionblur.definition = CAST_PTR(s_shader_motionblur_definition*, definition);
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
		void c_shader_motionblur::SetupShader()
		{
			YELO_ASSERT_DISPLAY(m_members_motionblur.shader_data == nullptr, "Shader data object has not been deleted and NULLed before being set again.");

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
			m_members_motionblur.shader_data = new c_shader_data_external();
			m_members_motionblur.shader_data->Ctor();
			m_members_motionblur.shader_data->SetFilePath(g_external_subsystem_shader_path, g_external_subsystem_include_path);
#else
			m_members_motionblur.shader_data = new c_shader_data_pp_shaders();
			m_members_motionblur.shader_data->Ctor();
			m_members_motionblur.shader_data->SetShaderID("PP_MotionBlur");
#endif
			SetSourceData(m_members_motionblur.shader_data);
		}

		/*!
		 * \brief
		 * Retrieves the D3DXHANDLE's for the motion blur parameters.
		 * 
		 * Retrieves the D3DXHANDLE's for the motion blur parameters.
		 */
		void c_shader_motionblur::GetHandles()
		{
			c_shader_postprocess::GetHandles();

			m_members_motionblur.definition->blur_amount_var.Initialize(m_members.definition->runtime.dx_effect, "c_blur_amount");
			m_members_motionblur.definition->vignette_amount_var.Initialize(m_members.definition->runtime.dx_effect, "c_vignette_amount");
		}

		void c_shader_motionblur::ClearHandles()
		{
			c_shader_postprocess::ClearHandles();

			m_members_motionblur.definition->blur_amount_var.ClearHandles();
			m_members_motionblur.definition->vignette_amount_var.ClearHandles();
		}

		/*!
		 * \brief
		 * Null's the motion blur parameter handles
		 * 
		 * Null's the motion blur parameter handles
		 */
		bool c_shader_motionblur::ValidateImpl()
		{
			if(!c_shader_postprocess::ValidateImpl())
				return false;

			bool valid = false;
			do
			{
				if(!c_post_processing_main::Instance().Globals().gbuffer) break;
				if(!c_post_processing_main::Instance().Globals().gbuffer->m_rt_velocity.IsEnabled()) break;
				if(!m_members_motionblur.definition->blur_amount_var.IsUsed()) break;
				if(!m_members_motionblur.definition->vignette_amount_var.IsUsed()) break;

				valid = true;
			}while(false);

			return valid;
		}
	}; }; };
};
#endif