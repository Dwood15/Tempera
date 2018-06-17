#pragma once

#include "macros_generic.h"

struct s_scenario_location {
	long             leaf_index;
	__int16          cluster_index;
	unsigned __int16 __unk_type0;
}; static_assert(sizeof(s_scenario_location) == 0x8);
