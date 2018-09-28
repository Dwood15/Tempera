/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_shader_instance_motionblur.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/time/interpolation/interpolation.hpp>
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace MotionBlur
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
		void c_shader_instance_motionblur::SetShader(c_shader_postprocess* definition)
		{
			m_members_motionblur.definition = CAST_PTR(c_shader_motionblur*, definition);
			c_shader_instance::SetShader(definition);
		}

		real& c_shader_instance_motionblur::BlurAmount()
		{
			return m_members_motionblur.blur_amount;
		}

		/////////////////////////////////////////////////
		// shader instance application
		void c_shader_instance_motionblur::UpdateShaderInstance(real delta_time)
		{
			m_members_motionblur.m_flags.vignette_enabled_prev = m_members_motionblur.m_flags.vignette_enabled;
			m_members_motionblur.m_flags.vignette_enabled = c_post_processing_main::Instance().Globals().m_activation_variables.player_has_control &&
				!c_post_processing_main::Instance().Globals().m_activation_variables.local_player_in_vehicle;

			if(m_members_motionblur.m_flags.vignette_enabled_prev != m_members_motionblur.m_flags.vignette_enabled)
			{
				if(m_members_motionblur.m_flags.vignette_enabled)
				{
					Time::Interpolation::InterpolateValues<1>(&m_members_motionblur.start_vignette, &m_members_motionblur.target_vignette,
						m_members_motionblur.vignette_interp.GetValues(), &m_members_motionblur.start_vignette);

					m_members_motionblur.target_vignette = 1.0f;

					m_members_motionblur.vignette_interp.Begin(0.25);
				}
				else
				{
					Time::Interpolation::InterpolateValues<1>(&m_members_motionblur.start_vignette, &m_members_motionblur.target_vignette,
						m_members_motionblur.vignette_interp.GetValues(), &m_members_motionblur.start_vignette);

					m_members_motionblur.target_vignette = 0.0f;

					m_members_motionblur.vignette_interp.Begin(0.25);
				}
			}
			else
				m_members_motionblur.vignette_interp.Update(delta_time);
		}

		/*!
		 * \brief
		 * Sets the shaders parameters to the current values.
		 * 
		 * Sets the shaders parameters to the current values.
		 */
		void c_shader_instance_motionblur::SetShaderInstanceVariables()
		{
			s_shader_motionblur_definition* definition = m_members_motionblur.definition->GetShaderDefinition<s_shader_motionblur_definition>();
			YELO_ASSERT_DISPLAY(definition != nullptr, "Motion blur shader class has no valid definition");

			LPD3DXEFFECT effect = m_members_motionblur.definition->GetEffect();
			YELO_ASSERT_DISPLAY(effect != nullptr, "Motion blur shader class has no effect");

			real vignette_amount;
			Time::Interpolation::InterpolateValues<1>(&m_members_motionblur.start_vignette, &m_members_motionblur.target_vignette, m_members_motionblur.vignette_interp.GetValues(), &vignette_amount);

			definition->blur_amount_var.SetVariable(effect, &m_members_motionblur.blur_amount);
			definition->vignette_amount_var.SetVariable(effect, &vignette_amount);
		}
	}; }; };
};
#endif