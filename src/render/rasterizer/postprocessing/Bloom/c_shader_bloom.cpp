/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_shader_bloom.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Bloom
	{
#ifdef EXTERNAL_SUBSYSTEM_SHADERS

#define SUBSYSTEM_BLOOM
#include "Rasterizer/PostProcessing/PostProcessing_ExternalSubsystemPaths.inl"
#undef SUBSYSTEM_BLOOM

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
		void c_shader_bloom::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
		{
			m_members_bloom.definition = CAST_PTR(s_shader_bloom_definition*, definition);
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
		void c_shader_bloom::SetupShader()
		{
			YELO_ASSERT_DISPLAY(m_members_bloom.shader_data == nullptr, "Shader data object has not been deleted and NULLed before being set again.");

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
			m_members_bloom.shader_data = new c_shader_data_external();
			m_members_bloom.shader_data->Ctor();
			m_members_bloom.shader_data->SetFilePath(g_external_subsystem_shader_path, g_external_subsystem_include_path);
#else
			m_members_bloom.shader_data = new c_shader_data_pp_shaders();
			m_members_bloom.shader_data->Ctor();
			m_members_bloom.shader_data->SetShaderID("PP_Bloom");
#endif
			SetSourceData(m_members_bloom.shader_data);
		}

		/*!
		 * \brief
		 * Retrieves the D3DXHANDLE's for the bloom parameters.
		 * 
		 * Retrieves the D3DXHANDLE's for the bloom parameters.
		 */
		void c_shader_bloom::GetHandles()
		{
			c_shader_postprocess::GetHandles();

			m_members_bloom.definition->size_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_size");
			m_members_bloom.definition->exposure_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_exposure");
			m_members_bloom.definition->mix_amount_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_mix_amount");
			m_members_bloom.definition->minimum_color_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_minimum_color");
			m_members_bloom.definition->maximum_color_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_maximum_color");
		}

		/*!
		 * \brief
		 * Null's the bloom parameter handles
		 * 
		 * Null's the bloom parameter handles
		 */
		void c_shader_bloom::ClearHandles()
		{
			c_shader_postprocess::ClearHandles();

			m_members_bloom.definition->size_handle.ClearHandles();
			m_members_bloom.definition->exposure_handle.ClearHandles();
			m_members_bloom.definition->mix_amount_handle.ClearHandles();
			m_members_bloom.definition->minimum_color_handle.ClearHandles();
			m_members_bloom.definition->maximum_color_handle.ClearHandles();
		}

		/*!
		 * \brief
		 * Validates the bloom shader class.
		 * 
		 * \returns
		 * true if the shader is valid.
		 * 
		 * Validates the bloom shader class. The shader is only valid if all of the parameter handls are present in the shader.
		 */
		bool c_shader_bloom::ValidateImpl()
		{
			if(!c_shader_postprocess::ValidateImpl())
				return false;

			bool valid = false;
			do
			{
				if(!m_members_bloom.definition->size_handle.IsUsed()) break;
				if(!m_members_bloom.definition->exposure_handle.IsUsed()) break;
				if(!m_members_bloom.definition->mix_amount_handle.IsUsed()) break;
				if(!m_members_bloom.definition->minimum_color_handle.IsUsed()) break;
				if(!m_members_bloom.definition->maximum_color_handle.IsUsed()) break;

				valid = true;
			}while(false);

			return valid;
		}
	}; }; };
};
#endif