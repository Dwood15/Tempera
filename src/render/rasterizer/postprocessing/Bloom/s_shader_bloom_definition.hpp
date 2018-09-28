/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Bloom
	{
		/////////////////////////////////////////////////////////////////////
		// Tag struct for holding runtime values
		/////////////////////////////////////////////////////////////////////
		struct s_shader_bloom_definition : TagGroups::s_shader_postprocess_definition
		{
			TAG_FIELD(TagGroups::t_shader_variable_real,		size_handle);
			TAG_FIELD(TagGroups::t_shader_variable_real,		exposure_handle);
			TAG_FIELD(TagGroups::t_shader_variable_real,		mix_amount_handle);
			TAG_FIELD(TagGroups::t_shader_variable_real3d,		minimum_color_handle);
			TAG_FIELD(TagGroups::t_shader_variable_real3d,		maximum_color_handle);
		};
	};};};
};
#endif