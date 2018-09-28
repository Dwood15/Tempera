/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_effect_internal.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/Generic/c_shader_instance_generic.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic { namespace Internal
	{
		/////////////////////////////////////////////////
		// member accessors
		void c_effect_internal::SetEffectReferenceDefinition(TagGroups::s_effect_postprocess_collection_effect* definition)
		{
			m_members_internal.definition = definition;
		}

		const char* c_effect_internal::GetName()
		{
			return m_members_internal.definition->name;
		}

		int16 c_effect_internal::GetScriptedVariableIndex(const char* name)
		{
			// compare the names of all script variables with the test string, return the index if found
			for(int i = 0; i < m_members_internal.definition->script_variables.Count; i++)
				if(strcmp(name, m_members_internal.definition->script_variables[i].script_variable_name) == 0)
					return i;
			// if no matching variable was found, return -1
			return NONE;
		}

		/////////////////////////////////////////////////
		// effect setup
		void c_effect_internal::SetupEffect()
		{
			c_effect_generic::SetupEffect();

			if(m_members_internal.definition->script_variables.Count == 0)
				return;

			SetupScriptedVariables();
		}

		void c_effect_internal::SetupScriptedVariables()
		{
			for(int i = 0; i < m_members_internal.definition->script_variables.Count; i++)
			{
				TagGroups::s_effect_postprocess_collection_script_variable& variable = m_members_internal.definition->script_variables[i];

				// gets a pointer to the parameter being exposed to halo script
				variable.runtime.exposed_parameter = GetExposedParameter(variable.exposed_parameter_name);
			}
		}

		/////////////////////////////////////////////////
		// scripting
		void c_effect_internal::SetScriptedVariable(int16 index, real change_time, bool value)
		{
			if(index >= m_members_internal.definition->script_variables.Count)
				return;

			c_parameter_instance* parameter = CAST_PTR(c_parameter_instance*, m_members_internal.definition->script_variables[index].runtime.exposed_parameter);

			if(parameter)
				parameter->SetOverrideInterp(value, change_time);
		}

		void c_effect_internal::SetScriptedVariable(int16 index, real change_time, int32 value)
		{
			if(index >= m_members_internal.definition->script_variables.Count)
				return;

			c_parameter_instance* parameter = CAST_PTR(c_parameter_instance*, m_members_internal.definition->script_variables[index].runtime.exposed_parameter);

			if(parameter)
				parameter->SetOverrideInterp(value, change_time);
		}

		void c_effect_internal::SetScriptedVariable(int16 index, real change_time, real value0, real value1, real value2, real value3)
		{
			if(index >= m_members_internal.definition->script_variables.Count)
				return;

			union{
				real real32;
				real_vector2d vector2d;
				real_vector3d vector3d;
				real_quaternion quaternion;
				real_argb_color color;
			};
			quaternion.i = value0;
			quaternion.j = value1;
			quaternion.k = value2;
			quaternion.w = value3;

			c_parameter_instance* parameter = CAST_PTR(c_parameter_instance*, m_members_internal.definition->script_variables[index].runtime.exposed_parameter);
			TagGroups::s_shader_postprocess_parameter* parameter_block = parameter->GetParameter();

			switch(parameter_block->value_type.count)
			{
			case 1:
				if(parameter_block->value_type.type == Enums::_shader_variable_base_type_argb_color)
					parameter->SetOverrideInterp(color, change_time);
				else
					parameter->SetOverrideInterp(real32, change_time);
				break;
			case 2: parameter->SetOverrideInterp(vector2d, change_time); break;
			case 3: parameter->SetOverrideInterp(vector3d, change_time); break;
			case 4: parameter->SetOverrideInterp(quaternion, change_time); break;
			}
		}
	};};};};
};
#endif