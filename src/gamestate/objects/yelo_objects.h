#pragma once
#include <macros_generic.h>
#include "enums/generic_enums.h"
#include "../../memory/datum_index.h"
#include "../../math/real_math.h"

namespace Yelo::Objects {
	size_t PredictMemoryPoolUsage(const object_type type, const int32 node_count, const bool include_yelo_upgrades);
	real GetObjectDistanceFromPoint(const datum_index object_index, const real_vector3d &destination_point);
};

