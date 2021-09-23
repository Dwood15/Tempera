#pragma once
#include "../../math/real_math.h"
#include "../../math/colors.h"

namespace Yelo::Objects {
		struct s_distant_light_info {
			real_rgb_color color;
			real_vector3d color_direction;
		};
		STAT_ASSERT(s_distant_light_info, 0x18);

		struct s_object_lighting {
			real_rgb_color ambient_color;
			short distant_light_count;
			unsigned short unkPad0;
			s_distant_light_info distant_light[2];

			//////////////////////////////////////////////////////////////////////////
			// not tag based, used by the game state
			short point_light_count;
			unsigned short unkPad1;
			datum_index point_light_indices[2]; // light_data
			//////////////////////////////////////////////////////////////////////////

			real_argb_color reflection_tint;
			real_vector3d shadow_vector;
			real_rgb_color shadow_color;
		};
		STAT_ASSERT(s_object_lighting, 0x74 );
};
