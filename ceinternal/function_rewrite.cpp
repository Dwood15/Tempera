#pragma once

#include <cstring>
#include <cstdio>
#include "function_rewrite.h"

namespace spcore {
	namespace initializations {
		void __inline adjustNPatch32(uintptr_t *loc, uint32 size) {
			memory::patchValue<uint32>(loc[0] + 0x1, size);
			memory::patchValue<uint32>(loc[1] + 0x1, size);
		}

		void patch_game_state_allocat_func() {
			uintptr_t hud_scripted_globals_sizeofs[] = {0x4AC7A7, 0x4AC7AF};
			uintptr_t hud_messaging_globals_sizeofs[] = {0x4AC7DC, 0x4AC7E6};
			uintptr_t unit_hud_globals_sizeofs[] = {0x4AC813, 0x4AC81B};
			uintptr_t weapon_hud_globals_sizeofs[] = {0x4AC848, 0x4AC850};
			uintptr_t hud_interface_related_globals_sizeofs[] = {0x4AC87D, 0x4AC885};
			uintptr_t motion_sensor_sizeofs[] = {0x4AC8B2, 0x4AC8BC};

			adjustNPatch32(hud_scripted_globals_sizeofs, 0x4);
			adjustNPatch32(hud_messaging_globals_sizeofs, 0x488);
			adjustNPatch32(unit_hud_globals_sizeofs, 0x5C);
			adjustNPatch32(weapon_hud_globals_sizeofs, 0x7C);
			adjustNPatch32(hud_interface_related_globals_sizeofs, 0x30);
			adjustNPatch32(motion_sensor_sizeofs, 0x570);
		}

		void __cdecl motion_sensor_initialize_for_new_map() {
			s_motion_sensor *motion_snsor_location = motion_sensor;
			memset(motion_sensor, 0, sizeof(s_motion_sensor));
			//auto * local_players = motion_snsor_location->local_players; // edx
			for (int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++) {
				for (int j = 0; j < k_number_of_game_teams; j++) {
					for (int k = 0; k < MAX_CUSTOM_BLIPS; k++) {
						motion_snsor_location->local_players[i].nearby_team_objects[j].object_blips[k].type = blip_type_none;
					}
				}
			}
		}

		int __fastcall interface_get_tag_index(short x) {
			return -1;
		}

		void __cdecl interface_initialize_for_new_map() {
			//I have no clue the actual contents of these structures yet, so we're guessing!
			*hud_scripted_globals = 0x0;
			*(char *) hud_scripted_globals = 0x1;
		}

		void __cdecl scripted_hud_messages_clear() {
			int v0; // edx
			signed int v1; // esi
			char *v2; // eax
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

	void __cdecl  player_control::player_control_initialize_for_new_map() {
		float matrix_related_flt = *(float *) 0x612188;

		s_player_control_globals_data *player_controls = *(s_player_control_globals_data **) 0x64C38C;
		game_options::s_game_globals *game_globals = *(game_options::s_game_globals **) 0x6E2260;
		s_player_control *current_player_datum = player_controls->local_players;
		float *mtrx_rltd_flt = (float *) 0x612188;
		float *player_look_pitch_rate = (float *) 0x68CD7C;
		float *player_look_yaw_rate = (float *) 0x68CD78;

		float *default_look_rates = (float *) ((int) game_globals + 0x160);


		//Begin of function
		player_controls->action_flags[1] = 0;
		player_controls->action_flags[0] = 0;
		player_controls->__pad_unk = 0;
		player_controls->flags = 0;

		if (*player_look_pitch_rate == *mtrx_rltd_flt) {
			*player_look_pitch_rate = default_look_rates[0];
		}

		if (*player_look_yaw_rate == *mtrx_rltd_flt) {
			*player_look_yaw_rate = default_look_rates[1];
		}

		memset(current_player_datum, 0x0, sizeof(s_player_control) * MAX_PLAYER_COUNT_LOCAL);

		for (int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++) {
			current_player_datum[i].unit_index.handle = -1;
			current_player_datum[i].weapon_index = -1;
			current_player_datum[i].grenade_index = -1;
			current_player_datum[i].zoom_level = -1;
			current_player_datum[i].target_object_index.handle = (unsigned long) -1;
			current_player_datum[i].weapon_swap_ticks = 0;
			current_player_datum[i]._unk_fld3_32 = 0x3FBF0243;
			current_player_datum[i]._unk_fld2_32 = 0xBFBF0243;
			current_player_datum[i].__pad_unk0 = 0x0;
			current_player_datum[i].__pad_unk1 = 0x0;
		}
		//End of Function
	} //STAT_ASSRT(complete_replacements::player_control_initialize_for_new_map, 0x9A);

	namespace memory {
		template<typename T>
		void patchValue<T>(uintptr_t to_patch, T replace_with) {
			*(T *) to_patch = replace_with;
		}

		void get_mem_and_patch() {
			//			//Relying on sizeof allows us to redefine MAX_PLAYER variables/defines
			patchValue<int>(players_init_for_new_map_overwrite, sizeof(s_players_globals_data));
			patchValue<int>(pub_server_patch, MAX_PLAYER_COUNT_LOCAL);  //max_player_count_local_patch
			patchValue<int>(main_get_window_ct, 0x9090);                //We nop the greater than count so we actually get the proper window renderings.

			//Byte Patches
			patchValue<byte>(main_game_render_patch, 0xEB);                //main_game_render's jle effectively clamps us to 1, so we just do unconditional jmp
			patchValue<byte>(precache_new_map_max_spawn_ct_cmp, 0x7C);    //75 is jne, we're gonna replace it with jl.
			patchValue<byte>(max_players_player_new_map, MAX_PLAYER_COUNT_LOCAL);        //
			patchValue<byte>(find_unused_player_index_spawn, MAX_PLAYER_COUNT_LOCAL);    //This patch isn't technically necessary but I'd rather have this sig documented than not.

			patchValue<byte>(get_player_input_blob_clamping_patch, MAX_PLAYER_COUNT_LOCAL);
			patchValue<byte>(player_ui_get_single_player_local_player_from_controller, MAX_PLAYER_COUNT_LOCAL);

			int real_address = (int) &spcore::player_control::player_control_initialize_for_new_map;
			int real_address_offset = (real_address) - ((int) player_control_init_new_map_hook);// + (int)4);

			//Hooks
			patchValue<uintptr_t>(((unsigned int) player_control_init_new_map_hook), (unsigned int) (real_address_offset) - 4);    //Gotta be able to loop over all the players + input devices, no?.

			printf("Calculated offset: 0x%x, real address: 0x%x\n", real_address_offset, real_address);
			printf("Calculated - 5 offset: 0x%x\n", real_address_offset - 5);
			printf("Calculated + 5 offset: 0x%x", real_address_offset + 5);
		}
	};


};