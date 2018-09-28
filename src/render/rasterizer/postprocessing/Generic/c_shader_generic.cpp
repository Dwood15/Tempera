/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_generic.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic
	{
		/////////////////////////////////////////////////
		// member accessors
		void c_shader_generic::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
		{
			m_members_generic.definition = CAST_PTR(TagGroups::s_shader_postprocess_generic*, definition);
			c_shader_postprocess::SetShaderDefinition(definition);
		}

		TagGroups::s_shader_postprocess_generic* 
				c_shader_generic::GetShader()
		{
			return m_members_generic.definition;
		}

		/////////////////////////////////////////////////
		// shader setup
		/*!
		 * \brief
		 * Loads the shaders bitmaps.
		 * 
		 * \param render_device
		 * The current render device
		 * 
		 * \returns
		 * S_OK if successful.
		 * 
		 * Loads the shaders bitmaps.
		 */
		HRESULT	c_shader_generic::LoadBitmaps(IDirect3DDevice9* render_device)
		{
			HRESULT success = S_OK;
			// checks for whether the parameter is in fact a texture happen in LoadBitmap so just call LoadBitmap on every parameter
			for(int i = 0; i < m_members_generic.definition->parameters.Count; i++)
				success &= m_members_generic.definition->parameters[i].LoadBitmap(render_device);
			return success;
		}

		void c_shader_generic::UnloadBitmaps()
		{
			for(int i = 0; i < m_members_generic.definition->parameters.Count; i++)
				m_members_generic.definition->parameters[i].ReleaseBitmap();
		}

		void c_shader_generic::SetupShader()
		{
			// if the source data is null this function is being called directly
			YELO_ASSERT_DISPLAY(m_members.source_data != nullptr, "c_shader_generic setup function does not create the source data, it must be created in a derived class");

			c_shader_postprocess::SetupShader();
		}

		/*!
		 * \brief
		 * Gets the shaders D3DXHANDLE's from the compiled effect.
		 * 
		 * Gets the shaders D3DXHANDLE's from the compiled effect.
		 * 
		 * \remarks
		 * c_shader_postprocess::GetHandles() must be called to get the handles for the base shader.
		 */
		void	c_shader_generic::GetHandles()
		{
			c_shader_postprocess::GetHandles();

			for(int i = 0; i < m_members_generic.definition->parameters.Count; i++)
				GetParameterHandle(m_members.definition->runtime.dx_effect, m_members_generic.definition->parameters[i]);
		}

		/*!
		 * \brief
		 * Clears all of the shaders D3DXHANDLE's.
		 * 
		 * Clears all of the shaders D3DXHANDLE's.
		 * 
		 * \remarks
		 * c_shader_postprocess::ClearHandles() must be called to clear the handles in the base shader.
		 */
		void	c_shader_generic::ClearHandles()
		{
			c_shader_postprocess::ClearHandles();

			for(int i = 0; i < m_members_generic.definition->parameters.Count; i++)
				ClearParameterHandle(m_members_generic.definition->parameters[i]);
		}

		/*!
		 * \brief
		 * Checks that the shader is valid.
		 * 
		 * \returns
		 * True if the shader is valid.
		 * 
		 * Checks that the shader is valid. For the shader to be valid the base validation logic must return
		 * true and all the parameters must have handles from the DX effect.
		 */
		bool	c_shader_generic::ValidateImpl()
		{
			if(!c_shader_postprocess::ValidateImpl())
				return false;

			bool success = true;
			for(int i = 0; i < m_members_generic.definition->parameters.Count; i++)
				success &= m_members_generic.definition->parameters[i].value.base.handle.IsUsed();

			return success;
		}

		void	c_shader_generic::GetParameterHandle(LPD3DXEFFECT effect, TagGroups::s_shader_postprocess_parameter& parameter)
		{
			parameter.value.base.handle.Initialize(effect, parameter.value_name, false);
		}

		void	c_shader_generic::ClearParameterHandle(TagGroups::s_shader_postprocess_parameter& parameter)
		{
			parameter.value.base.handle.ClearHandles();
		}

		void	c_shader_generic::SetParameterVariable(LPD3DXEFFECT effect, TagGroups::s_shader_postprocess_parameter& parameter)
		{
			// TODO: grab a (const?) reference to parameter.value and use it instead of parameter.value. directly
			switch(parameter.value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				parameter.value.boolean.handle.SetVariable(effect, &parameter.value.boolean.enabled);
				break;
			case Enums::_shader_variable_base_type_integer:
				parameter.value.integer32.handle.SetVariable(effect, &parameter.value.integer32.upper_bound);
				break;
			case Enums::_shader_variable_base_type_float:
				{
					switch(parameter.value_type.count)
					{
					case 1:
						parameter.value.real32.handle.SetVariable(effect, &parameter.value.real32.upper_bound);
						break;
					case 2:
						parameter.value.vector2d.handle.SetVariable(effect, &parameter.value.vector2d.upper_bound);
						break;
					case 3:
						parameter.value.vector3d.handle.SetVariable(effect, &parameter.value.vector3d.upper_bound);
						break;
					case 4:
						parameter.value.vector4d.handle.SetVariable(effect, &parameter.value.vector4d.upper_bound);
						break;
					};
				}
				break;
			case Enums::_shader_variable_base_type_argb_color:
					parameter.value.color4d.handle.SetVariable(effect, &parameter.value.color4d.upper_bound, false);
				break;
			case Enums::_shader_variable_base_type_texture:
					parameter.value.bitmap.handle.SetVariable(effect, parameter.GetTexture());
				break;
			}
		}

		/////////////////////////////////////////////////
		// shader application
		/*!
		 * \brief
		 * Sets the effects variables to their initial values.
		 * 
		 * Sets the effects variables to their initial values.
		 */
		void	c_shader_generic::SetVariables()
		{
			c_shader_postprocess::SetVariables();

			// TODO: should be able to use a ranged for loop here on ->parameters
			for(int i = 0; i < m_members_generic.definition->parameters.Count; i++)
				SetParameterVariable(m_members.definition->runtime.dx_effect, m_members_generic.definition->parameters[i]);
		}
	};};};
};
#endif