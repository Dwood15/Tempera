/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"
#include "Rasterizer/PostProcessing/Fade/s_shader_fade_definition.hpp"

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
	#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_external.hpp"
#else
	#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_pp_shaders.hpp"
#endif

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Fade
	{
		class c_shader_fade : public c_shader_postprocess
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				s_shader_fade_definition* definition;

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
				c_shader_data_external* shader_data;
#else
				c_shader_data_pp_shaders* shader_data;
#endif
			}m_members_fade;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void	SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition);

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_shader_postprocess::Ctor();

				m_members_fade.definition = NULL;
				m_members_fade.shader_data = NULL;
			}

			void Dtor()
			{
				c_shader_postprocess::Dtor();

				m_members_fade.definition = NULL;
				m_members_fade.shader_data->Dtor();
				delete m_members_fade.shader_data;
				m_members_fade.shader_data = NULL;
			}

			/////////////////////////////////////////////////
			// shader setup
		public:
			void SetupShader();
		protected:
			void GetHandles();
			void ClearHandles();
			bool ValidateImpl();
		};
	}; }; };
};
#endif