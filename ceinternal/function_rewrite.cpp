#pragma once

#include <cstring>
#include <cstdio>
#include "function_rewrite.h"

namespace spcore {
	static unsigned int           game_state_cpu_allocation  = *game_state_globals_ptr;
	static unsigned int           game_state_location_as_int = *game_state_globals_location_ptr;
	static s_players_globals_data *players_global_data       = *(s_players_globals_data **) 0x815918;

	static uintptr game_state_globals_location_ptr = (uintptr) 0x67DD8C;
	static uintptr game_state_globals_ptr          = (uintptr) 0x67DD88;
	static void    **crc_checksum_buffer           = (void **) 0x4D36D0;

	namespace hud {

	};

	static void **hud_scripted_globals = (void **) 0x6B44A8;
	static void **hud_messaging_state  = (void **) 0x677624;

	static s_motion_sensor *motion_sensor = *(s_motion_sensor **) 0x6B44C8;

	namespace initializations {
		void __inline adjustNPatch32 (uintptr_t *loc, uint32 size) {
			memory::patchValue<uint32> (loc[0] + 0x1, size);
			memory::patchValue<uint32> (loc[1] + 0x1, size);
		}

		void patch_game_state_allocat_func () {
			uintptr_t hud_scripted_globals_sizeofs[]          = { 0x4AC7A7, 0x4AC7AF };
			uintptr_t hud_messaging_globals_sizeofs[]         = { 0x4AC7DC, 0x4AC7E6 };
			uintptr_t unit_hud_globals_sizeofs[]              = { 0x4AC813, 0x4AC81B };
			uintptr_t weapon_hud_globals_sizeofs[]            = { 0x4AC848, 0x4AC850 };
			uintptr_t hud_interface_related_globals_sizeofs[] = { 0x4AC87D, 0x4AC885 };
			uintptr_t motion_sensor_sizeofs[]                 = { 0x4AC8B2, 0x4AC8BC };

			adjustNPatch32 (hud_scripted_globals_sizeofs, 0x4);
			adjustNPatch32 (hud_messaging_globals_sizeofs, 0x488);
			adjustNPatch32 (unit_hud_globals_sizeofs, 0x5C);
			adjustNPatch32 (weapon_hud_globals_sizeofs, 0x7C);
			adjustNPatch32 (hud_interface_related_globals_sizeofs, 0x30);
			adjustNPatch32 (motion_sensor_sizeofs, 0x570);
		}

		void __cdecl motion_sensor_initialize_for_new_map () {
			s_motion_sensor *motion_snsor_location = motion_sensor;
			memset (motion_sensor, 0, sizeof (s_motion_sensor));
			//auto * local_players = motion_snsor_location->local_players; // edx
			for ( int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++ ) {
				for ( int j = 0; j < k_number_of_game_teams; j++ ) {
					for ( int k = 0; k < MAX_CUSTOM_BLIPS; k++ ) {
						motion_snsor_location->local_players[i].nearby_team_objects[j].object_blips[k].type = blip_type_none;
					}
				}
			}
		}

		int __fastcall interface_get_tag_index (short x) {
			return -1;
		}

		void __cdecl interface_initialize_for_new_map () {
			//I have no clue the actual contents of these structures yet, so we're guessing!
			*hud_scripted_globals          = 0x0;
			*(char *) hud_scripted_globals = 0x1;
		}

		void __cdecl scripted_hud_messages_clear () {
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

	namespace rendering {
		int __cdecl get_render_window_count () {
			auto player_count = players_global_data->local_player_count;
			if ( player_count > MAX_PLAYER_COUNT_LOCAL || player_count < 1 ) {
				player_count = 1;
			}

			return (int) player_count;
		}
	};

	void __cdecl  player_control::player_control_initialize_for_new_map () {
		float matrix_related_flt = *(float *) 0x612188;

		s_player_control_globals_data *player_controls        = *(s_player_control_globals_data **) 0x64C38C;
		game_options::s_game_globals  *game_globals           = *(game_options::s_game_globals **) 0x6E2260;
		s_player_control              *current_player_datum   = player_controls->local_players;
		float                         *mtrx_rltd_flt          = (float *) 0x612188;
		float                         *player_look_pitch_rate = (float *) 0x68CD7C;
		float                         *player_look_yaw_rate   = (float *) 0x68CD78;

		float *default_look_rates = (float *) ((int) game_globals + 0x160 );


		//Begin of function
		player_controls->action_flags[1] = 0;
		player_controls->action_flags[0] = 0;
		player_controls->__pad_unk = 0;
		player_controls->flags     = 0;

		if ( *player_look_pitch_rate == *mtrx_rltd_flt ) {
			*player_look_pitch_rate = default_look_rates[0];
		}

		if ( *player_look_yaw_rate == *mtrx_rltd_flt ) {
			*player_look_yaw_rate = default_look_rates[1];
		}

		memset (current_player_datum, 0x0, sizeof (s_player_control) * MAX_PLAYER_COUNT_LOCAL);

		for ( int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++ ) {
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
	} //STAT_ASSRT(complete_replacements::player_control_initialize_for_new_map, 0x9A);

	namespace memory {
		//signature: "81 EC B4 00 00 00 8B 0D ?? ?? ?? ?? 53 8B D8"
		uintptr_t player_spawn                       = 0x47A9E0;
//			//Function call
// 			signature: "8B 35 20 59 81 00 57 8B FA B9 .26 00 00 00 F3 AB 83 CF FF"
		uintptr_t players_init_for_new_map_overwrite = 0x476243; // overwrite the .26 with the size of the 4 player structure.

		//"33 C0 83 F9 FF 74 05 B8 .01 00 00 00 66 89 46 0C");
		uintptr_t pub_server_patch = 0x477115;

		//"FF FF FF 7D 05 89 75 F4 EB 0D .7E 05 89 75 F4 EB");
		uintptr_t main_game_render_patch = 0x4CC61A;

		//"E8 64 85 FE FF 66 83 3D 9C 4A 62 00 01 .75 3C A1 1C FE 6A 00"
		uintptr_t precache_new_map_max_spawn_ct_cmp = 0x45B8D4;

		//, "90 39 1C B1 74 0A 46 83 FE .01 7C F5 5E 5B 59 C3");
		uintptr_t max_players_player_new_map = 0x4764E6;

		//, "46 83 FE .04 7C F1 8B C7 5F 5E C3 5F 8B C6 5E C3");
		uintptr_t find_unused_player_index_spawn = 0x476333;

		uintptr_t main_get_window_ct_cmp_num = 0x4CC5B6;
		//"66 8B 41 0C 66 3D 01 00 7C EA .7F E8 0F BF C0 C3"
		uintptr_t main_get_window_ct         = 0x4CC5BA;

		//"E8 4E 9A 01 00 .E8 69 7D 01 00 8B 15 44 C8 68 00"
		uintptr_t player_control_init_new_map_hook = 0x45BC33;

		//, "74 15 66 83 F9 .01 7D 0F 8B 15 18 59 81 00");
		uintptr_t get_player_input_blob_clamping_patch                     = 0x473C86;
		uintptr_t player_ui_get_single_player_local_player_from_controller = 0x498470;

		//Get_window_count patch locations
#pragma region
		//TODO: CONFIRM FIXES FOR THE CMP'S IMMEDIATELY FOLLOWING THESE CALLS.
		//interface_render
		uintptr_t get_render_window_count_hook_1 = 0x49757D;
		uintptr_t get_render_window_count_hook_2 = 0x4975C4;

		//Check_render_splitscreen
		uintptr_t get_render_window_count_hook_3 = 0x49792B;

		//rasterizer_detail_objects_begin
		uintptr_t get_render_window_count_hook_4 = 0x51EB00;

		//rasterizer_detail_objects_rebuild_vertices
		uintptr_t get_render_window_count_hook_5 = 0x51EE00;

		//_rasterizer_detail_objects_draw51EF90
		//THIS ONE IS THE SAME AS XBOX BETA ALMOST.
		uintptr_t get_render_window_count_hook_6 = 0x51EFA3;
#pragma endregion

		template<typename T>
		constexpr void patchValue<T> (uintptr_t to_patch, T replace_with) {
			*(T *) to_patch = replace_with;
		}

		/**
		 * @brief MPP == MAX PLAYER PATCH _B == BYTE
		 * @param value == address to_patch
		 */
		#define MPP_B(value, ...) patchValue<byte>(value, MAX_PLAYER_COUNT_LOCAL);

		void get_mem_and_patch () {

			//Relying on sizeof allows us to redefine MAX_PLAYER variables/defines
			patchValue<int> (players_init_for_new_map_overwrite, sizeof (s_players_globals_data));
			patchValue<int> (pub_server_patch, MAX_PLAYER_COUNT_LOCAL);  //max_player_count_local_patch
			patchValue<int> (main_get_window_ct, 0x9090);                //We nop the greater than count so we actually get the proper window renderings.

			//Byte Patches
			patchValue<byte> (main_game_render_patch, 0xEB);                //main_game_render's jle effectively clamps us to 1, so we just do unconditional jmp
			patchValue<byte> (precache_new_map_max_spawn_ct_cmp, 0x7C);    //75 is jne, we're gonna replace it with jl.

			MPP_B(max_players_player_new_map);
			MPP_B(find_unused_player_index_spawn);    //This patch MAY NOT be technically necessary but I'd rather have it documented than not.
			MPP_B(get_player_input_blob_clamping_patch);
			MPP_B(player_ui_get_single_player_local_player_from_controller);

			//Still deciding if I like this macro and it's format or not.
			MPP_B(0x4476EF + 0x2, first_person_camera_update_clamp_patch);
			MPP_B(0x446760 + 0x3, director_choose_camera_game_clamp);
			MPP_B(0x446760 + 0x3, director_choose_camera_game_clamp);

			//I can't find the xbox equivalent of this function, so this may not be necessary?
			MPP_B(0X497930 + 0x2, check_render_splitscreen_clamp);

			//Yep, this one's the real deal...
			MPP_B(0x4CBBFC + 0x3, create_local_players_clamp);

			//-- address is to the func(x) itself, not to to the patch
//			constexpr uintptr_t render_weapon_hud_loc = 0x4B53E0
//			MPP_B(render_weapon_hud_loc);

			int real_address        = (int) &spcore::player_control::player_control_initialize_for_new_map;
			int real_address_offset = ( real_address ) - ((int) player_control_init_new_map_hook );// + (int)4);

			//Hooks
			patchValue<uintptr_t> (((unsigned int) player_control_init_new_map_hook ), (unsigned int) ( real_address_offset ) - 4);    //Gotta be able to loop over all the players + input devices, no?.

			printf ("Calculated offset: 0x%x, real address: 0x%x\n", real_address_offset, real_address);
			printf ("Calculated - 5 offset: 0x%x\n", real_address_offset - 5);
			printf ("Calculated + 5 offset: 0x%x", real_address_offset + 5);
		}
	};

};