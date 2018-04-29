#pragma once

#include "macros_generic.h"

//S_animation-state won't get synched.
struct s_animation_state {
	//TODO: Double check if these are signed..
	__int16 animation_index;
	__int16 frame_index;
}; static_assert(sizeof(s_animation_state) == 0x4, STATIC_ASSERT_FAIL);
