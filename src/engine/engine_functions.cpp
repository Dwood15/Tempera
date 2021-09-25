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

	/*
 * 0x4CCF76
 * - E8 75 EF FF FF
 *
 * 0x53BF45
 * - E8 A6 FF F8 FF
 *
 * 0x53D018
 * - E8 D3 EE F8 FF
 *
 * 0x53D0B8
 * - E8 33 EE F8 FF
 */
	int blam::main_get_current_solo_level_from_name(char level_string[128]) {
		char test[128];

		memset(&test, 0, 124u);
		strncpy(test, level_string, 127u);
		test[127] = 0;
		_strlwr(test);
		test[127] = 0;
		if ( strstr(test, "a10") )
			return 0;
		if ( strstr(test, "a30") )
			return 1;
		if ( strstr(test, "a50") )
			return 2;
		if ( strstr(test, "b30") )
			return 3;
		if ( strstr(test, "b40") )
			return 4;
		if ( strstr(test, "c10") )
			return 5;
		if ( strstr(test, "c20") )
			return 6;
		if ( strstr(test, "c40") )
			return 7;
		if ( strstr(test, "d20") )
			return 8;
		return strstr(test, "d40") != 0 ? 9 : -1;
	}

}
