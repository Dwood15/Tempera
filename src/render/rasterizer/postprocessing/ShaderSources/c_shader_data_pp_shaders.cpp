/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_pp_shaders.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		const char* c_shader_data_pp_shaders::DataSourceID()
		{
			return m_shader_id;
		}

		const void* c_shader_data_pp_shaders::GetData(uint32& data_size)
		{
			data_size = 0;
			return c_post_processing_main::Instance().Globals().shader_file.GetDataPointer(&m_shader_id[0], &data_size);
		}
	};};
};
#endif