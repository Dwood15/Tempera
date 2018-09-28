/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_instance_generic.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic
	{
		/////////////////////////////////////////////////
		// member accessors
		void c_shader_instance_generic::SetShader(c_shader_postprocess* shader)
		{
			m_members_generic.shader = CAST_PTR(c_shader_generic*, shader);
			c_shader_instance::SetShader(shader);
		}

		/////////////////////////////////////////////////
		// shader instance setup
		/*!
		 * \brief
		 * Sets up the generic instances members.
		 * 
		 * Sets up the generic instances members. The parameter set up function is called here, and any other
		 * per instance additions should be set up here too.
		 */
		void c_shader_instance_generic::SetupShaderInstance()
		{
			c_shader_instance::SetupShaderInstance();

			CreateParameterInstances();
		}

		c_parameter_instance* c_shader_instance_generic::GetParameterInstance(const char* name)
		{
			c_parameter_instance* instance = nullptr;

			uint32 index = 0;
			while((index < m_members_generic.parameter_count) && !instance)
			{
				TagGroups::s_shader_postprocess_parameter* parameter = m_members_generic.parameters[index].GetParameter();

				YELO_ASSERT_DISPLAY(parameter != nullptr, "parameter instance has no assigned parameter");

				// compare the parameter instances name with the requested name
				if(strcmp(parameter->value_name, name) == 0)
					instance = &m_members_generic.parameters[index];

				index++;
			}
			return instance;
		}

		/*!
		 * \brief
		 * Creates and sets up the parameter instances of the shader.
		 *
		 * Creates and sets up the parameter instances of the shader.
		 */
		void c_shader_instance_generic::CreateParameterInstances()
		{
			TagGroups::s_shader_postprocess_generic* definition = m_members_generic.shader->GetShader();

			// if the shader has no parameters no further set up is needed
			if(definition->parameters.Count == 0)
				return;

			// create the parameter instances
			m_members_generic.parameter_count = definition->parameters.Count;
			m_members_generic.parameters = new c_parameter_instance[m_members_generic.parameter_count];

			for(uint32 i = 0; i < m_members_generic.parameter_count; i++)
			{
				// initialise each parameter instance and run its set up logic
				m_members_generic.parameters[i].Ctor();
				m_members_generic.parameters[i].SetParameter(&definition->parameters[i]);
				m_members_generic.parameters[i].SetupParameterInstance();
			}
		}

		/*!
		 * \brief
		 * Destructs and deletes all parameter instances.
		 * 
		 * Destructs and deletes all parameter instances.
		 */
		void c_shader_instance_generic::DestroyParameterInstances()
		{
			// if there are no parameters no further action is needed
			if(m_members_generic.parameter_count == 0)
				return;

			// run the destruction logic on each instance to delete memory and reset to defaults
			for(uint32 i = 0; i < m_members_generic.parameter_count; i++)
				m_members_generic.parameters[i].Dtor();

			// delete the parameter instance array
			delete [] m_members_generic.parameters;
			m_members_generic.parameters = nullptr;

			m_members_generic.parameter_count = 0;
		}

		/////////////////////////////////////////////////
		// shader instance application
		/*!
		 * \brief
		 * Updates the time based parameter instances.
		 * 
		 * \param delta_time
		 * The time that has passed since the last update.
		 * 
		 * Updates the time based parameter instances.
		 */
		void c_shader_instance_generic::UpdateShaderInstance(real delta_time)
		{
			for(uint32 i = 0; i < m_members_generic.parameter_count; i++)
				m_members_generic.parameters[i].Update(delta_time);
		}

		/*!
		 * \brief
		 * Sets the variable values in the effect to those of the shader instance's parameters.
		 * 
		 * Sets the variable values in the effect to those of the shader instance's parameters.
		 */
		void c_shader_instance_generic::SetShaderInstanceVariables()
		{
			for(uint32 i = 0; i < m_members_generic.parameter_count; i++)
				m_members_generic.parameters[i].SetParameterVariable(m_members_generic.shader->GetEffect());
		}
	};};};
};
#endif