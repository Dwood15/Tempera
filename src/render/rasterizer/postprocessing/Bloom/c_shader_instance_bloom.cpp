/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_shader_instance_bloom.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/time/interpolation/interpolation.hpp>

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Bloom
	{
		/////////////////////////////////////////////////
		// member accessors
		/*!
		 * \brief
		 * Sets the shader this class will instance.
		 * 
		 * \param definition
		 * The shader this class will instance.
		 * 
		 * Sets the shader this class will instance.
		 * 
		 * \see
		 * c_shader_postprocess
		 */
		void c_shader_instance_bloom::SetShader(c_shader_postprocess* definition)
		{
			m_members_bloom.definition = CAST_PTR(c_shader_bloom*, definition);
			c_shader_instance::SetShader(definition);
		}

		/*!
		 * \brief
		 * Sets the bloom shaders parameters to their initial values.
		 * 
		 * \param definition
		 * The bloom globals object to initialise the parameters to.
		 * 
		 * Sets the bloom shaders parameters to their initial values.
		 */
		void c_shader_instance_bloom::SetInitialValues(TagGroups::s_shader_postprocess_globals_bloom& definition)
		{
			m_members_bloom.m_variable_animation.current_values = definition.bloom;
			m_members_bloom.m_variable_animation.target_values = m_members_bloom.m_variable_animation.current_values;

			m_members_bloom.m_variable_animation.size_interp.Reset();
			m_members_bloom.m_variable_animation.exposure_interp.Reset();
			m_members_bloom.m_variable_animation.mix_amount_interp.Reset();
			m_members_bloom.m_variable_animation.minimum_color_interp.Reset();
			m_members_bloom.m_variable_animation.maximum_color_interp.Reset();
		}

		/////////////////////////////////////////////////
		// shader instance application
		/*!
		 * \brief
		 * Updates the interpolation classes of each bloom parameter.
		 * 
		 * \param delta_time
		 * The time that has passed in seconds since the last update.
		 * 
		 * Updates the interpolation classes of each bloom parameter.
		 */
		void c_shader_instance_bloom::UpdateShaderInstance(real delta_time)
		{
			m_members_bloom.m_variable_animation.size_interp.Update(delta_time);
			m_members_bloom.m_variable_animation.exposure_interp.Update(delta_time);
			m_members_bloom.m_variable_animation.mix_amount_interp.Update(delta_time);
			m_members_bloom.m_variable_animation.minimum_color_interp.Update(delta_time);
			m_members_bloom.m_variable_animation.maximum_color_interp.Update(delta_time);
		}

		/*!
		 * \brief
		 * Sets the shaders parameters to the current bloom values.
		 * 
		 * Sets the shaders parameters to the current bloom values. The values are interpolated as they can be modified by Halo script.
		 */
		void c_shader_instance_bloom::SetShaderInstanceVariables()
		{
			s_shader_bloom_definition* definition = m_members.definition->GetShaderDefinition<s_shader_bloom_definition>();
			YELO_ASSERT_DISPLAY(definition != nullptr, "Bloom shader has no tag definition");

			LPD3DXEFFECT effect = m_members.definition->GetEffect();
			// TODO: why are we reasserting definition?
			YELO_ASSERT_DISPLAY(definition != nullptr, "Bloom shader has no valid effect");

			definition->size_handle.SetVariableInterp(effect,
				&m_members_bloom.m_variable_animation.current_values.size,
				&m_members_bloom.m_variable_animation.target_values.size,
				m_members_bloom.m_variable_animation.size_interp.GetValues());
			definition->exposure_handle.SetVariableInterp(effect,
				&m_members_bloom.m_variable_animation.current_values.exposure,
				&m_members_bloom.m_variable_animation.target_values.exposure,
				m_members_bloom.m_variable_animation.exposure_interp.GetValues());
			definition->mix_amount_handle.SetVariableInterp(effect,
				&m_members_bloom.m_variable_animation.current_values.mix_amount,
				&m_members_bloom.m_variable_animation.target_values.mix_amount,
				m_members_bloom.m_variable_animation.mix_amount_interp.GetValues());
			definition->minimum_color_handle.SetVariableInterp(effect,
				&m_members_bloom.m_variable_animation.current_values.minimum_color,
				&m_members_bloom.m_variable_animation.target_values.minimum_color,
				m_members_bloom.m_variable_animation.minimum_color_interp.GetValues());
			definition->maximum_color_handle.SetVariableInterp(effect,
				&m_members_bloom.m_variable_animation.current_values.maximum_color,
				&m_members_bloom.m_variable_animation.target_values.maximum_color,
				m_members_bloom.m_variable_animation.maximum_color_interp.GetValues());
		};

		/////////////////////////////////////////////////
		// bloom changing
		/*!
		 * \brief
		 * Sets the size of the bloom sample spread.
		 * 
		 * \param size
		 * The target spread size.
		 * 
		 * \param change_time
		 * The time in seconds to reach to target spread size from the current value.
		 * 
		 * Sets the size of the bloom sample spread.
		 */
		void c_shader_instance_bloom::SetBloomSize(real size, real change_time)
		{
			real current;
			Time::Interpolation::InterpolateValues<1>(
				&m_members_bloom.m_variable_animation.current_values.size, 
				&m_members_bloom.m_variable_animation.target_values.size,
				m_members_bloom.m_variable_animation.size_interp.GetValues(),
				&current);

			m_members_bloom.m_variable_animation.current_values.size = current;
			m_members_bloom.m_variable_animation.target_values.size = size;
			m_members_bloom.m_variable_animation.size_interp.Begin(change_time);
		}

		/*!
		 * \brief
		 * Sets the bloom shaders exposure amount value.
		 * 
		 * \param exposure
		 * The target exposure amount.
		 * 
		 * \param change_time
		 * The time in seconds to reach to target exposure amount from the current value.
		 * 
		 * Sets the bloom shaders exposure amount value.
		 */
		void c_shader_instance_bloom::SetBloomExposure(real exposure, real change_time)
		{
			real current;
			Time::Interpolation::InterpolateValues<1>(
				&m_members_bloom.m_variable_animation.current_values.exposure, 
				&m_members_bloom.m_variable_animation.target_values.exposure,
				m_members_bloom.m_variable_animation.exposure_interp.GetValues(),
				&current);

			m_members_bloom.m_variable_animation.current_values.exposure = current;
			m_members_bloom.m_variable_animation.target_values.exposure = exposure;
			m_members_bloom.m_variable_animation.exposure_interp.Begin(change_time);
		}

		/*!
		 * \brief
		 * Sets the bloom mix amount value.
		 * 
		 * \param mix_amount
		 * The target mix amount.
		 * 
		 * \param change_time
		 * The time in seconds to reach the target mix amount from the current value.
		 * 
		 * Sets the bloom mix amount value.
		 */
		void c_shader_instance_bloom::SetBloomMixAmount(real mix_amount, real change_time)
		{
			real current;
			Time::Interpolation::InterpolateValues<1>(
				&m_members_bloom.m_variable_animation.current_values.mix_amount,
				&m_members_bloom.m_variable_animation.target_values.mix_amount,
				m_members_bloom.m_variable_animation.mix_amount_interp.GetValues(),
				&current);

			m_members_bloom.m_variable_animation.current_values.mix_amount = current;
			m_members_bloom.m_variable_animation.target_values.mix_amount = mix_amount;
			m_members_bloom.m_variable_animation.mix_amount_interp.Begin(change_time);
		}

		/*!
		 * \brief
		 * Sets the bloom minimum color value.
		 * 
		 * \param minimum_color
		 * The target minimum color.
		 * 
		 * \param change_time
		 * The time in seconds to reach the target minimum color from the current value.
		 * 
		 * Sets the bloom minimum color value.
		 */
		void c_shader_instance_bloom::SetBloomMinimumColor(const real_rgb_color& minimum_color, real change_time)
		{
			real_rgb_color current;
			Time::Interpolation::InterpolateValues<3>(
				&m_members_bloom.m_variable_animation.current_values.minimum_color.red,
				&m_members_bloom.m_variable_animation.target_values.minimum_color.red,
				m_members_bloom.m_variable_animation.minimum_color_interp.GetValues(),
				&current.red);

			m_members_bloom.m_variable_animation.current_values.minimum_color = current;
			m_members_bloom.m_variable_animation.target_values.minimum_color = minimum_color;
			m_members_bloom.m_variable_animation.minimum_color_interp.Begin(change_time);
		}

		/*!
		 * \brief
		 * Sets the bloom maximum color value.
		 * 
		 * \param maximum_color
		 * The target maximum color.
		 * 
		 * \param change_time
		 * The time in seconds to reach the target maximum color from the current value.
		 * 
		 * Sets the bloom maximum color value.
		 */
		void c_shader_instance_bloom::SetBloomMaximumColor(const real_rgb_color& maximum_color, real change_time)
		{
			real_rgb_color current;
			Time::Interpolation::InterpolateValues<3>(
				&m_members_bloom.m_variable_animation.current_values.maximum_color.red,
				&m_members_bloom.m_variable_animation.target_values.maximum_color.red,
				m_members_bloom.m_variable_animation.maximum_color_interp.GetValues(),
				&current.red);

			m_members_bloom.m_variable_animation.current_values.maximum_color = current;
			m_members_bloom.m_variable_animation.target_values.maximum_color = maximum_color;
			m_members_bloom.m_variable_animation.maximum_color_interp.Begin(change_time);
		}
	}; }; };
};
#endif