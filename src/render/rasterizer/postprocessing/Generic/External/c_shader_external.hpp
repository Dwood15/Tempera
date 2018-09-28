/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/Generic/c_shader_generic.hpp"
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_external.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic { namespace External
	{
		class c_shader_external : public c_shader_generic
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				tag_string shader_id;
				char shader_path[MAX_PATH];
				char include_path[MAX_PATH];
				c_shader_data_external* shader_source_data;
			}m_members_external;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetShaderPath(const char* path);
			void SetShaderID(const char* id);
			const char* GetShaderID();

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_shader_generic::Ctor();

				m_members_external.shader_path[0] = 0;
				m_members_external.shader_id[0] = '\0';
				m_members_external.shader_source_data = NULL;
			}

			void Dtor()
			{
				m_members_external.shader_path[0] = 0;
				m_members_external.shader_id[0] = '\0';
				delete m_members_external.shader_source_data;
				m_members_external.shader_source_data = NULL;

				c_shader_generic::Dtor();
			}

			/////////////////////////////////////////////////
			// shader setup
		public:
			void SetupShader();
		};
	};};};};
};
#endif