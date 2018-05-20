#pragma once

#include <precompile.h>
#include "../player_types.h"
#include "../../function_rewrite.h"

namespace render {
	using ::players_global_data;
	uintptr_t game_engine_definition   = 0x68CD24;
	uintptr_t game_engine_globals_mode = 0x815EB0; // some enum like >= 2 && <= 3
	uintptr_t cinematic_globals        = 0x68C83C; // and cinematic globals in progress

	int main_get_window_count_override() {
		//Todo: check if cinematic is playing.
		auto player_count = players_global_data->local_player_count;
		if (player_count > MAX_PLAYER_COUNT_LOCAL || player_count < 1) {
			player_count = 1;
		}

		return (int) player_count;
	}

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
	int main_get_current_solo_level(char * level_string) {
		char v5[124];
		char test[127];


		memset(&v5, 0, 124u);
		strncpy(test, level_string, 127u);

		if (test) {
			char *v1 = test;
			char v2;
			do
			{
				*v1 = tolower((unsigned __int8)*v1);
				v2 = (v1++)[1];
			}
			while ( v2 );
		}
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

	/**
	 * 0x49757D - interface_draw_screen
	 * - E8 FE 4F 03 00
	 *
	 * 0x49792B - see above
    * - E8 50 4C 03 00
	 *
	 * 0x4975C4 - check_render_split_screen
	 * - E8 B7 4F 03 00
	 *
	 * 0x51EB00 rasterizer_detail_objects_begin
	 * - E8 7B DA FA FF
	 *
	 * 0x51EE00 - rasterizer_detail_objects_rebuild_vertices
	 * - E8 7B D7 FA FF
	 *
	 * 0x51EFA3 - rasterizer_detail_objects_draw
	 * - E8 D8 D5 FA FF
	 */
	namespace overrides {
		constexpr uintptr_t override_function_call_list[] = {0x51EFA3, 0x51EE00, 0x51EB0, 0x49792B, 0x4975C4, 0x49757D};

		void override_all() {
			for (int i = 0; i < std::size(override_function_call_list); i++) {
				auto current = override_function_call_list[i];
				auto addr    = calc_addr_offset(current, (int) &render::main_get_window_count_override);
				spcore::memory::patchValue<uintptr_t>(current, addr);
			}
		}
	};

};
