/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"
#include "Rasterizer/PostProcessing/Fade/s_shader_fade_definition.hpp"
#include "Rasterizer/PostProcessing/Fade/c_shader_fade.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Fade
	{
		class c_shader_instance_fade : public c_shader_instance
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				c_shader_fade* definition;

				real fade_amount;
			}m_members_fade;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetShader(c_shader_postprocess* definition);

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_shader_instance::Ctor();

				m_members_fade.definition = NULL;
			}

			void Dtor()
			{
				c_shader_instance::Dtor();

				m_members_fade.definition = NULL;
			}

			/////////////////////////////////////////////////
			// shader instance application
		public:
			void SetShaderInstanceVariables();
			HRESULT Render(IDirect3DDevice9* render_device, c_quad_instance* quad_instance, real fade_amount);
		};
	}; }; };
};
#endif