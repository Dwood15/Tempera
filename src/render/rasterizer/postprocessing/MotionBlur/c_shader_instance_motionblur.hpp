/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/time/interpolation/c_interp_linear.hpp>
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_shader_motionblur.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace MotionBlur
	{
		class c_shader_instance_motionblur : public c_shader_instance
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				struct
				{
					bool vignette_enabled_prev;
					bool vignette_enabled;
					PAD16;
				}m_flags;

				c_shader_motionblur* definition;

				real blur_amount;

				real start_vignette;
				real target_vignette;
				Time::Interpolation::c_interp_linear<1> vignette_interp;
			}m_members_motionblur;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetShader(c_shader_postprocess* shader);
			real& BlurAmount();

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_shader_instance::Ctor();

				m_members_motionblur.m_flags.vignette_enabled_prev = false;
				m_members_motionblur.m_flags.vignette_enabled = false;

				m_members_motionblur.definition = NULL;

				m_members_motionblur.start_vignette = 0.0f;
				m_members_motionblur.target_vignette = 0.0f;
				m_members_motionblur.vignette_interp.Ctor();
			}

			void Dtor()
			{
				c_shader_instance::Dtor();

				m_members_motionblur.m_flags.vignette_enabled_prev = false;
				m_members_motionblur.m_flags.vignette_enabled = false;

				m_members_motionblur.definition = NULL;

				m_members_motionblur.start_vignette = 0.0f;
				m_members_motionblur.target_vignette = 0.0f;
				m_members_motionblur.vignette_interp.Ctor();
			}

			/////////////////////////////////////////////////
			// shader instance application
		public:
			void UpdateShaderInstance(real delta_time);
			void SetShaderInstanceVariables();
		};
	}; }; };
};
#endif