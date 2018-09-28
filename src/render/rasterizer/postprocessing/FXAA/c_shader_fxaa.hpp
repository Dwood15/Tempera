/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
	#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_external.hpp"
#else
	#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_pp_shaders.hpp"
#endif

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace FXAA
	{
		class c_shader_fxaa : public c_shader_postprocess
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
#ifdef EXTERNAL_SUBSYSTEM_SHADERS
				c_shader_data_external* shader_data;
#else
				c_shader_data_pp_shaders* shader_data;
#endif
			}m_members_fxaa;

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_shader_postprocess::Ctor();

				m_members_fxaa.shader_data = NULL;
			}

			void Dtor()
			{
				c_shader_postprocess::Dtor();

				m_members_fxaa.shader_data->Dtor();
				delete m_members_fxaa.shader_data;
				m_members_fxaa.shader_data = NULL;
			}

			/////////////////////////////////////////////////
			// shader setup
		public:
			void SetupShader();
		};
	}; }; };
};
#endif