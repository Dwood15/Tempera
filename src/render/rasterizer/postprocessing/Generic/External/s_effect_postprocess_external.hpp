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
	namespace Rasterizer { namespace PostProcessing { namespace Generic { namespace External
	{
		struct s_effect_postprocess_external : TagGroups::s_effect_postprocess_generic
		{
			TAG_TBLOCK_(shader_ids, tag_string);
		};
	};};};};
};
#endif