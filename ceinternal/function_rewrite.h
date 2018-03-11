#pragma once
#include "shitty_macros.h"
#include "shitty_header_ports.h"
#include <cstring>
//
namespace complete_replacements {
	static void __cdecl  player_control_initialize_for_new_map() {
		float matrix_related_flt = *(float *)0x612188;

		s_player_control_globals_data * player_controls = *(s_player_control_globals_data **)0x64C38C;
		game_options::s_game_globals * game_globals = *(game_options::s_game_globals **)0x6E2260;
		s_player_control * current_player_datum = player_controls->local_players;

		player_controls->action_flags[1] = 0;
		player_controls->action_flags[0] = 0;
		player_controls->__pad_unk = 0;
		player_controls->flags = 0;

		float * mtrx_rltd_flt = (float *)0x612188;
		float * player_look_pitch_rate = (float*)0x68CD7C;
		float * player_look_yaw_rate = (float*)0x68CD78;

		float * default_look_rates = (float*)((int)game_globals + 0x160);
		if(*player_look_pitch_rate == *mtrx_rltd_flt) {
			*player_look_pitch_rate = default_look_rates[0];
		}		

		if(*player_look_yaw_rate == *mtrx_rltd_flt) {
			*player_look_yaw_rate = default_look_rates[1];
		}

		memset(current_player_datum, 0x0, sizeof(s_player_control) * MAX_PLAYER_COUNT_LOCAL);

		for(int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++) {
			current_player_datum[i].unit_index.handle = -1;
			current_player_datum[i].weapon_index = -1;
			current_player_datum[i].grenade_index = -1;
			current_player_datum[i].zoom_level = -1;
			current_player_datum[i].target_object_index.handle = -1;
			current_player_datum[i].weapon_swap_ticks = 0;
			current_player_datum[i]._unk_fld3_32 = 0x3FBF0243;
			current_player_datum[i]._unk_fld2_32 = 0xBFBF0243;
			current_player_datum[i].__pad_unk0 = 0x0;
			current_player_datum[i].__pad_unk1 = 0x0;
		}

	} //STAT_ASSRT(complete_replacements::player_control_initialize_for_new_map, 0x9A);
}	