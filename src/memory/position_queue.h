#pragma once

#include <precompile.h>
#include "simple_circular_queue.h"
#include "../cseries/base.h"

namespace Yelo::Game {
	struct s_position_queue_entry : TStructImpl(20) {
	};
	struct s_position_queue {
		Memory::s_simple_circular_queue queue;
		s_position_queue_entry          *entries;      // 0x14, #30, user_data
	};
	static_assert(sizeof(s_position_queue) == 0x18);
};
