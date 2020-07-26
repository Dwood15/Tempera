#pragma once

#include <enums/cache_enums.h>
#include "engine_functions.h"
#include "../cache/cache_files.h"

// comment the following to just use the parameters as-is
// instead of copying to the stack. I believe there was some
// concurrency issues originally which caused me to use local
// buffers instead...
enum {
	k_engine_function_string_buffer_size = 512,
};

// End the code to a naked function which takes arguments
// and is also __stdcall
#define API_FUNC_NAKED_END(arg_count)    \
        __asm pop    ebp                \
        __asm retn    (arg_count * 4)    }

namespace Yelo {
	namespace Physics {
		struct s_collision_result;
	};

	namespace Players {
		struct s_unit_camera_info;
	};

	namespace TagGroups {
		struct s_bitmap_data;

		struct s_sound_permutation;

		struct predicted_resource;
	};
}
