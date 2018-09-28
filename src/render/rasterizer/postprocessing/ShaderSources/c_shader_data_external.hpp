/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_base.hpp"
#include "Common/FileIO.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		class c_shader_data_external : public c_shader_data_base
		{
			char* m_file_data;
			const char* m_file_path;
			const char* m_include_path;

			D3DXMACRO m_macros[2];

		public:
			void SetFilePath(const char* file_path, const char* include_path);

			void Ctor()
			{
				m_file_data = NULL;
				m_file_path = NULL;
				m_include_path = NULL;

				m_macros[0].Name = "SHADER_EXTERNAL";
				m_macros[0].Definition = NULL;
				m_macros[1].Name = NULL;
				m_macros[1].Definition = NULL;
			}
			void Dtor()
			{
				m_file_path = NULL;
				m_include_path = NULL;

				delete [] m_file_data;
				m_file_data = NULL;
			}

			const char* DataSourceID();
			const void* GetData(uint32& data_size);
			void Dispose();
			const bool IsBinary() { return false; }
			const char* GetIncludePath();
			const D3DXMACRO* GetMacros();
		};
	};};
};
#endif