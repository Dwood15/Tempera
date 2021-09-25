#include <cstring>
#include <cstdio>
#include <engine_interface.h>
#include "function_rewrite.h"
#include "../../../src/gamestate/game_globals.h"
#include "../../../src/memory/data_array.h"

namespace spcore {
	//static s_motion_sensor*motion_sensor = ;

	namespace initializations {
		void players_initialize_for_new_map() {
			static auto *const players_globals = *reinterpret_cast<s_players_globals_data **>(0x815918);
			static auto *const player_data = *reinterpret_cast<Yelo::Memory::s_data_array**>(0x815920);
			static auto *const team_data = *reinterpret_cast<s_team_data **>(0x7A4210);


			memset(players_globals, 0, sizeof(s_players_globals_data));

			for (auto i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++) {
				players_globals->local_player_players[i] = -1;
				players_globals->local_player_dead_units[i] = -1;
			}

			players_globals->unused_after_initialize_unk = -1;
			players_globals->input_disabled = 0;
			players_globals->double_speed_ticks_remaining = 0;
			players_globals->are_all_dead = 0;
			players_globals->_bsp_switch_trigger_idx = -1;
			players_globals->respawn_failure = 0;

			Yelo::blam::data_make_valid(player_data);
			Yelo::blam::data_make_valid((Yelo::Memory::s_data_array *)team_data);
			memset((void *)(0x676FA8), 0xFF, 0x40);

		}

		//40000db0
		//		void __cdecl motion_sensor_initialize_for_new_map() {
		//			s_motion_sensor * motion_snsor_location = (s_motion_sensor*)0x6B44C8;
		//			printf("Address @ motion sensor location: 0x08%x", *motion_snsor_location);
		//			s_motion_sensor *motion_sensor = (s_motion_sensor*)0x6B44C8;
		//			memset(motion_sensor, 0, sizeof(s_motion_sensor));
		//			//auto * local_players = motion_snsor_location->local_players; // edx
		//			for( int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++ ) {
		//				for( int j = 0; j < k_number_of_game_teams; j++ ) {
		//					for( int k = 0; k < MAX_CUSTOM_BLIPS; k++ ) {
		//						motion_snsor_location->local_players[i].nearby_team_objects[j].object_blips[k].type = blip_type_none;
		//					}
		//				}
		//			}
		//		}

		void scripted_hud_messages_clear() {
			// int        v0; // edx
			// signed int v1; // esi
			// char       *v2; // eax
			// signed int v3; // ecx
			//
			// v0 = (int) ::core->hud_messaging_state + 0x82;
			// v1 = 4;
			// do {
			// 	v2 = (char *) v0;
			// 	v3 = 4;
			// 	do {
			// 		*v2 = 0;
			// 		v2 += 0x8C;
			// 		--v3;
			// 	} while (v3);
			// 	v0 += 0x460;
			// 	--v1;
			// } while (v1);
		}
	};

	namespace player_updates {
		int calculatePlayerCount() {
			static auto *const players_globals = *reinterpret_cast<s_players_globals_data **>(0x815918);

			//Classically, Halo allowed for nearly any ordering of allocated players
			int numPlyrs = 0;
			for (int i = MAX_PLAYER_COUNT_LOCAL; i > 0; i--) {
				auto plyrRef = players_globals->local_player_players[i-1];
				if (!plyrRef.IsNull()) {
					++numPlyrs;
				}
			}

			return numPlyrs;
		}

		void naked compute_combined_pvs_local() {
			__asm call calculatePlayerCount
			//Oh god oh fuck this is so fucked and probably won't work
			__asm mov     [esi+0Ch], ax
			__asm retn
		}
	}
	//Initialzie at new map!
	const void player_control::player_control_initialize() {
		//TODO: overwrite the built-in control initialization to account for s_player_control_globals I think
	}

	void player_control::player_control_initialize_for_new_map() {
		// float matrix_related_flt = *(float *) 0x612188;
		s_player_control_globals_data *player_controls        = *(s_player_control_globals_data **) 0x64C38C;
		game_options::s_game_globals  *game_globals           = *(game_options::s_game_globals **) 0x6E2260;
		s_player_control              *current_player_datum   = player_controls->local_players;
		float                         *mtrx_rltd_flt          = (float *) 0x612188;
		float                         *player_look_pitch_rate = (float *) 0x68CD7C;
		float                         *player_look_yaw_rate   = (float *) 0x68CD78;

		float *default_look_rates = (float *) ((int) game_globals + 0x160);

		//Begin of function
		player_controls->action_flags[1] = 0;
		player_controls->action_flags[0] = 0;
		player_controls->__pad_unk = 0;
		player_controls->flags     = 0;

		if (*player_look_pitch_rate == *mtrx_rltd_flt) {
			*player_look_pitch_rate = default_look_rates[0];
		}

		if (*player_look_yaw_rate == *mtrx_rltd_flt) {
			*player_look_yaw_rate = default_look_rates[1];
		}

		memset(current_player_datum, 0x0, sizeof(s_player_control) * MAX_PLAYER_COUNT_LOCAL);

		for (int i = 0; i < (int)MAX_PLAYER_COUNT_LOCAL; i++) {
			current_player_datum[i].unit_index.handle          = (unsigned long) -1;
			current_player_datum[i].weapon_index               = -1;
			current_player_datum[i].grenade_index              = -1;
			current_player_datum[i].zoom_level                 = -1;
			current_player_datum[i].target_object_index.handle = (unsigned long) -1;
			current_player_datum[i].weapon_swap_ticks          = 0;
			current_player_datum[i]._unk_fld3_32               = 0x3FBF0243;
			current_player_datum[i]._unk_fld2_32               = 0xBFBF0243;
			current_player_datum[i].__pad_unk0                 = 0x0;
			current_player_datum[i].__pad_unk1                 = 0x0;
		}
		//End of Function
	}

	void hud_update_nav_points() {
		signed int player_idx   = 0;
		static auto *const players_globals = *reinterpret_cast<s_players_globals_data **>(0x815918);

		auto       player_datum = players_globals->local_player_players[player_idx].index;

		if (player_datum < 0) {
			return;
		}

		do {
			//TODO: Add function pointer for hud_update_nav_points
			//Unfortunately, this is one of those bullshit cases where the game engine does its own calling convention.
			//According to ida:
			//	void __usercall hud_update_nav_point_local_player(__int16 player_idx@<di>)
			// 0x4B3550
			//hud_update_unit_local_player(player_datum);
			if (player_idx >= 4) {
				player_datum = -1;
			}
		} while (player_datum > 0);
	}

	void hud_update_unit() {
		signed int player_idx   = 0;
		static auto *const players_globals = *reinterpret_cast<s_players_globals_data **>(0x815918);
		auto       player_datum = players_globals->local_player_players[player_idx].index;

		if (player_datum < 0) {
			return;
		}

		do {
			//TODO: Add function pointer for hud_update_unit_local_player
			//Unfortunately, this is one of those bullshit cases where the game engine does its own calling convention.
			//According to ida:
			//	void __usercall hud_update_unit_local_player(__int16 player_idx@<di>)
			//hud_update_unit_local_player(player_datum);
			if (player_idx >= 4) {
				player_datum = -1;
			}
		} while (player_datum > 0);
	}
};
