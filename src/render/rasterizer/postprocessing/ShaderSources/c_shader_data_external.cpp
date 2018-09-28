/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_external.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_base.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		const char* c_shader_data_external::DataSourceID()
		{
			uint32 id_length = strlen(m_file_path) + 1;
			if(id_length > k_data_source_id_max_length)
				return &m_file_path[id_length - k_data_source_id_max_length];
			else
				return m_file_path;
		}

		void c_shader_data_external::SetFilePath(const char* file_path, const char* include_path)
		{
			m_file_path = file_path;
			m_include_path = include_path;
		}

		const void* c_shader_data_external::GetData(uint32& data_size)
		{
			FileIO::s_file_info file_info;
			bool success = false;
			do
			{
				if(FileIO::OpenFile(file_info, m_file_path))
					break;
				if(FileIO::ReadFileToMemory(file_info, CAST_PTR(void**,&m_file_data)))
					break;
				data_size = file_info.file_size;

				success = true;
			}while(false);

			FileIO::CloseFile(file_info);

			if(!success)
				return NULL;

			return m_file_data;
		}

		void c_shader_data_external::Dispose()
		{
			delete [] m_file_data;
			m_file_data = NULL;
		}

		const char* c_shader_data_external::GetIncludePath()
		{
			return m_include_path;
		}

		const D3DXMACRO* c_shader_data_external::GetMacros()
		{
			return &m_macros[0];
		}
	};};
};
#endif