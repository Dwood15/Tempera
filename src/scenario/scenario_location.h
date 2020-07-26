#pragma once

#include "macros_generic.h"
#include "../math/real_math.h"
#include "../math/colors.h"

namespace Yelo::Scenario {
	// The engine uses this for things (objects, ai, etc) which get dis/connected to the scenario's bsp
	struct s_scenario_location {
		long  leaf_index;
		short cluster_index;
		short pad0;
	};
	STAT_ASSERT(s_scenario_location, 0x8);

#pragma pack(push, 1)
	struct s_sky_atmospheric_fog {
		real_rgb_color color;
		long long : 64;
		real_fraction maximum_density;
		real          start_distance;
		real          opaque_distance;
	};
#pragma pack(pop)

	STAT_ASSERT(s_sky_atmospheric_fog, 0x20);

};
