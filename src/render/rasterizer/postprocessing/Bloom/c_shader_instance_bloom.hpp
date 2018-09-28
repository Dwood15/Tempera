/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/time/interpolation/c_interp_linear.hpp>
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"
#include "Rasterizer/PostProcessing/Bloom/s_shader_bloom_definition.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_shader_bloom.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Bloom
	{
		class c_shader_instance_bloom : public c_shader_instance
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				c_shader_bloom* definition;
				TagGroups::s_shader_postprocess_globals_bloom* globals;

				struct
				{
					TagGroups::s_shader_postprocess_bloom_definition current_values;
					TagGroups::s_shader_postprocess_bloom_definition target_values;

					Time::Interpolation::c_interp_linear<1> size_interp;
					Time::Interpolation::c_interp_linear<1> exposure_interp;
					Time::Interpolation::c_interp_linear<1> mix_amount_interp;
					Time::Interpolation::c_interp_linear<3> minimum_color_interp;
					Time::Interpolation::c_interp_linear<3> maximum_color_interp;
				}m_variable_animation;
			}m_members_bloom;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetShader(c_shader_postprocess* definition);
			void SetInitialValues(TagGroups::s_shader_postprocess_globals_bloom& definition);

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_shader_instance::Ctor();

				m_members_bloom.definition = NULL;
				m_members_bloom.globals = NULL;

				m_members_bloom.m_variable_animation.size_interp.Reset();
				m_members_bloom.m_variable_animation.exposure_interp.Reset();
				m_members_bloom.m_variable_animation.mix_amount_interp.Reset();
				m_members_bloom.m_variable_animation.minimum_color_interp.Reset();
				m_members_bloom.m_variable_animation.maximum_color_interp.Reset();
			}

			void Dtor()
			{
				c_shader_instance::Dtor();

				m_members_bloom.definition = NULL;
				m_members_bloom.globals = NULL;

				m_members_bloom.m_variable_animation.size_interp.Reset();
				m_members_bloom.m_variable_animation.exposure_interp.Reset();
				m_members_bloom.m_variable_animation.mix_amount_interp.Reset();
				m_members_bloom.m_variable_animation.minimum_color_interp.Reset();
				m_members_bloom.m_variable_animation.maximum_color_interp.Reset();
			}

			/////////////////////////////////////////////////
			// shader instance application
		public:
			void UpdateShaderInstance(real delta_time);
			void SetShaderInstanceVariables();

			/////////////////////////////////////////////////
			// bloom changing
			void SetBloomSize(real size, real change_time);
			void SetBloomExposure(real exposure, real change_time);
			void SetBloomMixAmount(real mix_amount, real change_time);
			void SetBloomMinimumColor(const real_rgb_color& minimum_color, real change_time);
			void SetBloomMaximumColor(const real_rgb_color& maximum_color, real change_time);
		};
	}; }; };
};
#endif