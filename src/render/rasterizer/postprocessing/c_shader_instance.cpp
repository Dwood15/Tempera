/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_shader_instance.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		/////////////////////////////////////////////////
		// member accessors
		void c_shader_instance::SetShader(c_shader_postprocess* definition)
		{
			m_members.definition = definition;
		}

		c_shader_postprocess* c_shader_instance::GetShader() 
		{
			return m_members.definition;
		}

		bool c_shader_instance::IsValid()
		{
			return m_members.m_flags.is_valid;
		}

		bool& c_shader_instance::IsActive()
		{
			return m_members.m_flags.is_active;
		}
		
		bool c_shader_instance::UsesGBuffer()
		{
			if(!GetShader() || !GetShader()->GetShaderDefinition<TagGroups::s_shader_postprocess_definition>())
				return false;
			return GetShader()->GetShaderDefinition<TagGroups::s_shader_postprocess_definition>()->runtime.flags.uses_gbuffer_bit;
		}

		/////////////////////////////////////////////////
		// shader instance setup
		/*!
		 * \brief
		 * Sets the is_valid flag.
		 * 
		 * Sets the is_valid flag.
		 */
		void c_shader_instance::Validate()
		{
			m_members.m_flags.is_valid = ValidateImpl();
		}

		/*!
		 * \brief
		 * Returns whether this shader instance is valid.
		 * 
		 * \returns
		 * True if the shader instance is valid.
		 * 
		 * Returns whether this shader instance is valid. A shader instance is valid if it points to a valid shader.
		 */
		bool c_shader_instance::ValidateImpl()
		{
			bool valid = false;
			do
			{
				if(!m_members.definition) break;

				valid = true;
			}while(false);

			if(valid)
				valid &= m_members.definition->IsValid();
			return valid;
		}
	};};
};
#endif