/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_postprocess_definition.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_base.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		const char* c_shader_data_postprocess_definition::DataSourceID()
		{
			uint32 name_length = strlen(m_shader_name) + 1;
			if(name_length > k_data_source_id_max_length)
				return &m_shader_name[name_length - k_data_source_id_max_length];
			else
				return m_shader_name;
		}

		const void* c_shader_data_postprocess_definition::GetData(uint32& data_size)
		{
			data_size = m_shader_definition->shader_code_binary.size;
			return m_shader_definition->shader_code_binary.address;
		}
	};};
};
#endif