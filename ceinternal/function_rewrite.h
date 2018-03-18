#pragma once

#ifndef function_rewrite
#define function_rewrite
#include "ceinternal.h"
#include "shitty_header_ports.h"
#include "gamestate_headers/player_types.h"
#include <cstring>

//yolo coding at its finest.
namespace spcore {
	static uintptr game_state_globals_location_ptr = (uintptr) 0x67DD8C;
	static uintptr game_state_globals_ptr = (uintptr) 0x67DD88;

	static unsigned int game_state_cpu_allocation = *game_state_globals_ptr;
	static unsigned int game_state_location_as_int = *game_state_globals_location_ptr;

	struct S_scripted_hud_messages {
		char unk[0x8C];
	};

	struct s_hud_messaging_state {
		S_scripted_hud_messages hmi[4];
	};

//	struct s_hud_message_state_player {
//		char unk[0x82];
//		s_hud_messaging_state scripted_hud_messages[4];
//	}; STAT_ASSRT(s_hud_message_state_player, 0x)

	static void **crc_checksum_buffer = (void **) 0x4D36D0;
	static void **hud_scripted_globals = (void **) 0x6B44A8;
	static void **hud_messaging_state = (void **) 0x677624;

	static s_motion_sensor *motion_sensor = *(s_motion_sensor **) 0x6B44C8;

	namespace initializations {
		void __inline adjustNPatch32(uintptr_t *loc, uint32 size);

		void patch_game_state_allocat_func();

		void __cdecl motion_sensor_initialize_for_new_map();

		int __fastcall interface_get_tag_index(short x);

		void __cdecl interface_initialize_for_new_map();

		void __cdecl scripted_hud_messages_clear();
	};

	namespace player_control {
		void __cdecl  player_control_initialize_for_new_map();
	};
};
#endif