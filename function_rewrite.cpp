#pragma once

#include <cstring>
#include <cstdio>
#include <engine_interface.h>
#include "function_rewrite.h"
#include "gamestate/game_globals.h"
#include "gamestate/player_types.h"
#include "lua/lua.h"
#include "headers/hce_addresses.h"

namespace spcore {
	static unsigned int           game_state_cpu_allocation  = *game_state_globals_ptr;
	static unsigned int           game_state_location_as_int = *game_state_globals_location_ptr;
	static s_players_globals_data *players_global_data       = *(s_players_globals_data **) 0x815918;

	static uintptr game_state_globals_location_ptr = (uintptr) 0x67DD8C;
	static uintptr game_state_globals_ptr          = (uintptr) 0x67DD88;
	static void    **crc_checksum_buffer           = (void **) 0x4D36D0;

	static void **hud_scripted_globals = (void **) 0x6B44A8;
	static void **hud_messaging_state  = (void **) 0x677624;

	//static s_motion_sensor*motion_sensor = ;

	namespace initializations {
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
			int        v0; // edx
			signed int v1; // esi
			char       *v2; // eax
			signed int v3; // ecx

			v0 = (int) hud_messaging_state + 0x82;
			v1 = 4;
			do {
				v2 = (char *) v0;
				v3 = 4;
				do {
					*v2 = 0;
					v2 += 0x8C;
					--v3;
				} while (v3);
				v0 += 0x460;
				--v1;
			} while (v1);
		}
	};

	//Initialzie at new map!
	static const void __cdecl player_control::player_control_initialize() {
		//TODO: overwrite the built-in control initialization to account for s_player_control_globals I think
	}

	void __cdecl  player_control::player_control_initialize_for_new_map() {
		float matrix_related_flt = *(float *) 0x612188;

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

		for (int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++) {
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

	void __cdecl hud_update_nav_points() {
		signed int player_idx = 0;
		auto player_datum = players_globals->local_player_players[player_idx].index;

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
			if (player_idx >= 4 ) {
				player_datum = -1;
			}
		} while ( player_datum > 0 );
	}


	void __cdecl hud_update_unit() {
		signed int player_idx = 0;
		auto player_datum = players_globals->local_player_players[player_idx].index;

		if (player_datum < 0) {
			return;
		}

		do {
			//TODO: Add function pointer for hud_update_unit_local_player
			//Unfortunately, this is one of those bullshit cases where the game engine does its own calling convention.
			//According to ida:
			//	void __usercall hud_update_unit_local_player(__int16 player_idx@<di>)
			//hud_update_unit_local_player(player_datum);
			if (player_idx >= 4 ) {
				player_datum = -1;
			}
		} while ( player_datum > 0 );
	}


	namespace memory {
		const void __cdecl scenario_tags_load() {
			LuaState->call_lua_event_by_type(LuaCallbackId::before_scenario_tags_load);
			calls::DoCall<0x442290>();
			LuaState->call_lua_event_by_type(LuaCallbackId::after_scenario_tags_load);
		}

		const void __cdecl game_tick(int current_frame_tick) {
			LuaState->call_lua_event_by_type(LuaCallbackId::before_game_tick);
			calls::DoCall<0x45C0F0, Convention::m_cdecl, void, int>(current_frame_tick);
			LuaState->call_lua_event_by_type(LuaCallbackId::after_game_tick);
		}

		void __inline interface_initialize_patches() {
			constexpr uintptr_t size_of_fp_weapons = 0x1EA0 * MAX_PLAYER_COUNT_LOCAL;

			uintptr_t fp_weap_initialize[] = {0x497122, 0x49712F};

			memory::adjustNPatch32(fp_weap_initialize, size_of_fp_weapons);

			//uintptr_t hud_scripted_globals_sizeofs[]          = { 0x4AC7A7, 0x4AC7AF };
			uintptr_t hud_messaging_globals_sizeofs[] = {0x4AC7DD, 0x4AC7EA};
			uintptr_t hud_messaging_state_size             = 0x4AC936;
			//static_assert(sizeof(s_hud_messaging_state)  == 0x122, "stat_assrt_fail: s hud msging state");
			//memset , or rather, rep stosd assumes full integer (0x4) size in this case.
			memory::patchValue<uintptr_t>(hud_messaging_state_size, sizeof(s_hud_messaging_state) / 4);
			uintptr_t motion_sensor_sizeofs[] = {0x4AC8B3, 0x4AC8BC + 0x4};

			//			Need to confirm these...
			//			uintptr_t unit_hud_globals_sizeofs[]              = { 0x4AC813, 0x4AC81B + 0x4 };
			//			uintptr_t weapon_hud_globals_sizeofs[]            = { 0x4AC848, 0x4AC850 + 0x4 };
			//			uintptr_t hud_interface_related_globals_sizeofs[] = { 0x4AC87D, 0x4AC885 + 0x4 };


			//adjustNPatch32(hud_scripted_globals_sizeofs, 0x4);
			//0x488 og size.
			memory::adjustNPatch32(hud_messaging_globals_sizeofs, sizeof(s_hud_messaging_state));
			//			adjustNPatch32(unit_hud_globals_sizeofs, 0x5C);
			//			adjustNPatch32(weapon_hud_globals_sizeofs, 0x7C);
			//			adjustNPatch32(hud_interface_related_globals_sizeofs, 0x30);
			memory::adjustNPatch32(motion_sensor_sizeofs, sizeof(s_motion_sensor));
		}

		void get_mem_and_patch() {
			//TODO: Coneceptually separate these out..
			//Function call
			// 			signature: "8B 35 20 59 81 00 57 8B FA B9 .26 00 00 00 F3 AB 83 CF FF"
			constexpr uintptr_t players_initialize_for_new_map_overwrite = 0x476243; // overwrite the .26 with the size of the 4 player structure.
			//Relying on sizeof allows us to redefine MAX_PLAYER variables/defines
			patchValue<int>(players_initialize_for_new_map_overwrite, sizeof(s_players_globals_data));

			//"E8 64 85 FE FF 66 83 3D 9C 4A 62 00 01 .75 3C A1 1C FE 6A 00"
			//75 is jne, we're gonna replace it with jl.
			//0x476200

			printf("Ran the OG get_mem_and_patches");

			//uintptr_t player_spawn = 0x47A9E0; Valid, just not using it...

			interface_initialize_patches();

			//Gotta be able to loop over all the players + input devices, no?.
			//"E8 4E 9A 01 00 E8 .69 7D 01 00 8B 15 44 C8 68 00"
			constexpr uintptr_t player_control_init_new_map_hook = 0x45BC33;
			//printf("Calculated - 4 offset: 0x%x\n", real_address_offset - 4);
			//Hooks
			auto addr = calc_addr_offset(player_control_init_new_map_hook, (int)&spcore::player_control::player_control_initialize_for_new_map);
			patchValue<uintptr_t>(player_control_init_new_map_hook, addr); //Gotta be able to loop over all the players + input devices, no?.

			constexpr uintptr_t scenario_load_hook = 0x541A04;
			addr = calc_addr_offset(scenario_load_hook, (int)&scenario_tags_load);
			patchValue<uintptr_t>(scenario_load_hook, addr); //Gotta be able to loop over all the players + input devices, no?.

			constexpr uintptr_t game_tick_hook = 0x473815;
			addr = calc_addr_offset(game_tick_hook, (int)&game_tick);
			patchValue<uintptr_t>(game_tick_hook, addr);

			//Nothing wrong's with the hook, just the function.
			//			real_address        = (int)&initializations::motion_sensor_initialize_for_new_map;
			//			real_address_offset = ( real_address ) - ((int)0x4AC98E);																					// + (int)4);
			//
			//			patchValue<uintptr_t>(((unsigned int)0x4AC98E ), (unsigned int)( real_address_offset ) - 4);


			// insertRenderWindowCountHooks();

			//_rasterizer_detail_objects_draw51EF90
			//THIS ONE IS THE SAME AS XBOX BETA ALMOST.
			//			constexpr uintptr_t get_render_window_ct_patch_6 = 0x51EE05;
			//patchValue<short>(, (short)-1);
			::run_byte_size_patches();
		}
	};
};
