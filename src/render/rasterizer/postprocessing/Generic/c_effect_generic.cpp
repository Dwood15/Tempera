/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/c_effect_generic.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_instance_generic.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic
	{
		/////////////////////////////////////////////////
		// member accessors
		void c_effect_generic::SetEffectDefinition(TagGroups::s_effect_postprocess_definition* definition)
		{
			m_members_generic.definition = CAST_PTR(TagGroups::s_effect_postprocess_generic*, definition);
			c_effect_postprocess::SetEffectDefinition(m_members_generic.definition);
		}

		/////////////////////////////////////////////////
		// effect setup
		void c_effect_generic::SetupEffect()
		{
			c_effect_postprocess::SetupEffect();

			if(m_members_generic.definition->exposed_parameters.Count == 0)
				return;

			SetupExposedParameters();
		}

		void c_effect_generic::SetupExposedParameters()
		{
			for(int i = 0; i < m_members_generic.definition->exposed_parameters.Count; i++)
			{
				TagGroups::s_effect_postprocess_generic_exposed_parameter& parameter = m_members_generic.definition->exposed_parameters[i];
				c_shader_instance_generic* instance = GetShaderInstance<c_shader_instance_generic>(parameter.shader_index);

				// get the parameter instance and set it to be overidden
				c_parameter_instance* parameter_instance = instance->GetParameterInstance(parameter.parameter_name);
				parameter_instance->SetupParameterOverride();

				// set the parameter pointer in the exposed parameter to the parameter in the shader instance
				parameter.runtime.parameter_instance = parameter_instance;
			}
		}

		c_parameter_instance* c_effect_generic::GetExposedParameter(const char* exposed_name)
		{
			for(int i = 0; i < m_members_generic.definition->exposed_parameters.Count; i++)
			{
				TagGroups::s_effect_postprocess_generic_exposed_parameter& variable = m_members_generic.definition->exposed_parameters[i];

				// compare the exposed parameter name with the requested name, returing a pointer if matching
				if(strcmp(variable.exposed_name, exposed_name) == 0)
					return CAST_PTR(c_parameter_instance*, variable.runtime.parameter_instance);
			}
			// a matching parameter was not found, return NULL
			return NULL;
		}
	};};};
};
#endif