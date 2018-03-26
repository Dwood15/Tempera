#pragma once

#include <cstring>
#include <cstdio>
#include "function_rewrite.h"

/**
 * @brief MPP == MAX PLAYER PATCH _B == BYTE
 * @param value == address to_patch
 */
#define MPP_B(value, ...) patchValue<byte>(value, MAX_PLAYER_COUNT_LOCAL);
#define MPP_ARB(value, arb, ...) patchValue<byte>(value, arb);

namespace spcore {
	static unsigned int          game_state_cpu_allocation  = *game_state_globals_ptr;
	static unsigned int          game_state_location_as_int = *game_state_globals_location_ptr;
	static s_players_globals_data*players_global_data       = *(s_players_globals_data**)0x815918;

	static uintptr game_state_globals_location_ptr = (uintptr)0x67DD8C;
	static uintptr game_state_globals_ptr          = (uintptr)0x67DD88;
	static void    **crc_checksum_buffer           = (void**)0x4D36D0;

	namespace hud { };

	namespace memory {
		template<typename T>
		constexpr void patchValue<T>(uintptr_t to_patch, T replace_with) {
			*(T*)to_patch = replace_with;
		}

		inline int calc_offset(uintptr_t real_address, uintptr_t reference) {
			return (int)(( real_address ) - ((int)reference ));
		}

		#define GET_OFFSET_FROM_FUNC(func, reference) calc_offset((uintptr_t)(func), reference)
	}

	static void**hud_scripted_globals = (void**)0x6B44A8;
	static void**hud_messaging_state  = (void**)0x677624;

	//static s_motion_sensor*motion_sensor = ;

	namespace initializations {

		//40000db0
		void __inline adjustNPatch32(uintptr_t*loc, uint32 size) {
			memory::patchValue<uint32>(loc[0], size);
			memory::patchValue<uint32>(loc[1], size);
		}

		void __inline interface_initialize_patches() {
			constexpr uintptr_t size_of_fp_weapons = 0x1EA0 * MAX_PLAYER_COUNT_LOCAL;

			uintptr_t fp_weap_initialize[] = { 0x497122, 0x49712F };

			adjustNPatch32(fp_weap_initialize, size_of_fp_weapons);

			//uintptr_t hud_scripted_globals_sizeofs[]          = { 0x4AC7A7, 0x4AC7AF };
			uintptr_t hud_messaging_globals_sizeofs[] = { 0x4AC7DD, 0x4AC7EA };
			uintptr_t hud_messaging_state             = 0x4AC936;
//			static_assert(sizeof(s_hud_messaging_state)  == 0x122, "stat_assrt_fail: s hud msging state");
			//memset , or rather, rep stosd assumes full integer (0x4) size in this case. thus, we
			memory::patchValue<uintptr_t>(hud_messaging_state, sizeof(s_hud_messaging_state) / 4);
			uintptr_t motion_sensor_sizeofs[]         = { 0x4AC8B3, 0x4AC8BC + 0x4 };

//			Need to confirm these...
//			uintptr_t unit_hud_globals_sizeofs[]              = { 0x4AC813, 0x4AC81B + 0x4 };
//			uintptr_t weapon_hud_globals_sizeofs[]            = { 0x4AC848, 0x4AC850 + 0x4 };
//			uintptr_t hud_interface_related_globals_sizeofs[] = { 0x4AC87D, 0x4AC885 + 0x4 };


			//adjustNPatch32(hud_scripted_globals_sizeofs, 0x4);
			//0x488 og size.
			adjustNPatch32(hud_messaging_globals_sizeofs, sizeof(s_hud_messaging_state));
//			adjustNPatch32(unit_hud_globals_sizeofs, 0x5C);
//			adjustNPatch32(weapon_hud_globals_sizeofs, 0x7C);
//			adjustNPatch32(hud_interface_related_globals_sizeofs, 0x30);
			adjustNPatch32(motion_sensor_sizeofs, sizeof(s_motion_sensor));
		}

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

		int __fastcall interface_get_tag_index(short x) {
			return -1;
		}

		void __cdecl scripted_hud_messages_clear() {
			int        v0; // edx
			signed int v1; // esi
			char       *v2; // eax
			signed int v3; // ecx

			v0 = (int)hud_messaging_state + 0x82;
			v1 = 4;
			do {
				v2 = (char*)v0;
				v3 = 4;
				do {
					*v2 = 0;
					v2 += 0x8C;
					--v3;
				} while(v3);
				v0 += 0x460;
				--v1;
			} while(v1);
		}
	};

	namespace rendering {
		int __cdecl get_render_window_count() {
			auto player_count = players_global_data->local_player_count;
			if( player_count > MAX_PLAYER_COUNT_LOCAL || player_count < 1 ) {
				player_count = 1;
			}

			return (int)player_count;
		}
	};

	void __inline nopBytes(uintptr_t location, unsigned short numNops) {
		for( unsigned short i = 0; i < numNops; i++ ) {
			memory::patchValue<byte>(location + i, 0x90);
		}
	}

	void __cdecl  player_control::player_control_initialize_for_new_map() {
		float matrix_related_flt = *(float*)0x612188;

		s_player_control_globals_data*player_controls        = *(s_player_control_globals_data**)0x64C38C;
		game_options::s_game_globals *game_globals           = *(game_options::s_game_globals**)0x6E2260;
		s_player_control             *current_player_datum   = player_controls->local_players;
		float                        *mtrx_rltd_flt          = (float*)0x612188;
		float                        *player_look_pitch_rate = (float*)0x68CD7C;
		float                        *player_look_yaw_rate   = (float*)0x68CD78;

		float*default_look_rates = (float*)((int)game_globals + 0x160 );


		//Begin of function
		player_controls->action_flags[1] = 0;
		player_controls->action_flags[0] = 0;
		player_controls->__pad_unk = 0;
		player_controls->flags     = 0;

		if( *player_look_pitch_rate == *mtrx_rltd_flt ) {
			*player_look_pitch_rate = default_look_rates[0];
		}

		if( *player_look_yaw_rate == *mtrx_rltd_flt ) {
			*player_look_yaw_rate = default_look_rates[1];
		}

		memset(current_player_datum, 0x0, sizeof(s_player_control) * MAX_PLAYER_COUNT_LOCAL);

		for( int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++ ) {
			current_player_datum[i].unit_index.handle          = (unsigned long)-1;
			current_player_datum[i].weapon_index               = -1;
			current_player_datum[i].grenade_index              = -1;
			current_player_datum[i].zoom_level                 = -1;
			current_player_datum[i].target_object_index.handle = (unsigned long)-1;
			current_player_datum[i].weapon_swap_ticks          = 0;
			current_player_datum[i]._unk_fld3_32               = 0x3FBF0243;
			current_player_datum[i]._unk_fld2_32               = 0xBFBF0243;
			current_player_datum[i].__pad_unk0                 = 0x0;
			current_player_datum[i].__pad_unk1                 = 0x0;
		}
		//End of Function
	}

	namespace memory {
//		uintptr_t player_spawn = 0x47A9E0; Valid, just not using it yet.


		//Get_window_count patch locations
#pragma region

		void __inline patchRenderPlayerFrameClamp() {
			constexpr uintptr_t render_player_frame_jg_patch = 0x50F5EB;
			patchValue<short>(render_player_frame_jg_patch, (short)0x9090);
			constexpr uintptr_t render_player_frame_cmp_patch = 0x50F5F0;
			nopBytes(render_player_frame_cmp_patch, 0xA);
		}

		void __inline patchRenderWindowCompares() {
			//TODO: CONFIRM FIXES FOR THE CMP'S IMMEDIATELY FOLLOWING THESE CALLS.
			//interface_render
			constexpr uintptr_t get_render_window_ct_patch_1 = 0x497584;
			constexpr uintptr_t get_render_window_ct_patch_2 = 0x4975CB;

			//Check_render_splitscreen
			constexpr uintptr_t get_render_window_ct_patch_3 = 0x49792B;

			//rasterizer_detail_objects_begin
			constexpr uintptr_t get_render_window_ct_patch_4 = 0x51EB05;

			//rasterizer_detail_objects_rebuild_vertices
			constexpr uintptr_t get_render_window_ct_patch_5 = 0x51EE05;

			//_rasterizer_detail_objects_draw51EF90
			//THIS ONE IS THE SAME AS XBOX BETA ALMOST.
//			constexpr uintptr_t get_render_window_ct_patch_6 = 0x51EE05;

			patchValue<byte>(get_render_window_ct_patch_1, MAX_PLAYER_COUNT_LOCAL);
			patchValue<byte>(get_render_window_ct_patch_2, MAX_PLAYER_COUNT_LOCAL);
			patchValue<byte>(get_render_window_ct_patch_3, MAX_PLAYER_COUNT_LOCAL);
			patchValue<byte>(get_render_window_ct_patch_4, MAX_PLAYER_COUNT_LOCAL);
			patchValue<byte>(get_render_window_ct_patch_5, MAX_PLAYER_COUNT_LOCAL);
			//patchValue<short>(0x4CBBC8, (short)-1);
		}

		void __inline insertRenderWindowCountHooks() {
			constexpr uintptr_t get_render_window_count_hook_1 = 0x49757D;
			constexpr uintptr_t get_render_window_count_hook_2 = 0x4975C4;
			constexpr uintptr_t get_render_window_count_hook_3 = 0x49792B;
			constexpr uintptr_t get_render_window_count_hook_4 = 0x51EB00;
			constexpr uintptr_t get_render_window_count_hook_5 = 0x51EE00;

			GET_OFFSET_FROM_FUNC(&rendering::get_render_window_count, get_render_window_count_hook_1);
			GET_OFFSET_FROM_FUNC(&rendering::get_render_window_count, get_render_window_count_hook_2);
			GET_OFFSET_FROM_FUNC(&rendering::get_render_window_count, get_render_window_count_hook_3);
			GET_OFFSET_FROM_FUNC(&rendering::get_render_window_count, get_render_window_count_hook_4);
			GET_OFFSET_FROM_FUNC(&rendering::get_render_window_count, get_render_window_count_hook_5);
		}

#pragma endregion

		void __inline mem_patch_p1() {
			//			//Function call
// 			signature: "8B 35 20 59 81 00 57 8B FA B9 .26 00 00 00 F3 AB 83 CF FF"
			constexpr uintptr_t players_init_for_new_map_overwrite = 0x476243; // overwrite the .26 with the size of the 4 player structure.

			//"E8 64 85 FE FF 66 83 3D 9C 4A 62 00 01 .75 3C A1 1C FE 6A 00"
			constexpr uintptr_t precache_new_map_max_spawn_ct_cmp = 0x45B8D4;

			//, "90 39 1C B1 74 0A 46 83 FE .01 7C F5 5E 5B 59 C3");
			constexpr uintptr_t max_players_player_new_map = 0x4764E8;

			//, "46 83 FE .04 7C F1 8B C7 5F 5E C3 5F 8B C6 5E C3");
			constexpr uintptr_t find_unused_player_index_spawn = 0x476333;

			//"66 8B 41 0C 66 3D 01 00 7C EA .7F E8 0F BF C0 C3"
			constexpr uintptr_t main_get_window_ct = 0x4CC5BA;

			//"E8 4E 9A 01 00 E8 .69 7D 01 00 8B 15 44 C8 68 00"
			constexpr uintptr_t player_control_init_new_map_hook = 0x45BC33;

			//, "74 15 66 83 F9 .01 7D 0F 8B 15 18 59 81 00");
			constexpr uintptr_t get_player_input_blob_clamping_patch                     = 0x473C86;
			constexpr uintptr_t player_ui_get_single_player_local_player_from_controller = 0x498470;

			//"33 C0 83 F9 FF 74 05 B8 .01 00 00 00 66 89 46 0C");
			constexpr uintptr_t pub_server_patch = 0x477115;

			//"FF FF FF 7D 05 89 75 F4 EB 0D .7E 05 89 75 F4 EB");
			constexpr uintptr_t main_game_render_patch = 0x4CC61A;

			//Relying on sizeof allows us to redefine MAX_PLAYER variables/defines
			patchValue<int>(players_init_for_new_map_overwrite, sizeof(s_players_globals_data));
			patchValue<int>(pub_server_patch, MAX_PLAYER_COUNT_LOCAL);  //max_player_count_local_patch
			patchValue<short>(main_get_window_ct, (short)0x9090);                //We nop the greater than count so we actually get the proper window renderings.

			//Byte Patches
			patchValue<byte>(main_game_render_patch, 0xEB);                //main_game_render's jle effectively clamps us to 1, so we just do unconditional jmp
			patchValue<byte>(precache_new_map_max_spawn_ct_cmp, 0x7C);    //75 is jne, we're gonna replace it with jl.

			//patch bytes region
			MPP_B(max_players_player_new_map);
			MPP_B(find_unused_player_index_spawn);    //This patch MAY NOT be technically necessary but I'd rather have it documented than not.
			MPP_B(get_player_input_blob_clamping_patch);
			MPP_B(player_ui_get_single_player_local_player_from_controller);

			//Still deciding if I like this macro and it's format or not.
			MPP_B(0x4476EF + 0x2, first_person_camera_update_clamp_patch);
			MPP_B(0x446760 + 0x3, director_choose_camera_game_clamp);
			MPP_B(0x446760 + 0x3, director_choose_camera_game_clamp);
			MPP_ARB(0x49F897, 0x0, player_spawn_count_hack_fuck_off);

			//I can't find the xbox equivalent of this function, so this may not be necessary?
			MPP_B(0X497930 + 0x2, check_render_splitscreen_clamp);

			//Yep, this one's the real deal...
			MPP_B(0x4CBBFC + 0x3, create_local_players_clamp);
			MPP_B(0x4A04B0 + 0x1, coop_game_initialize);
			MPP_B(0x4A0076, local_player_initialize_spawn_ct);
			MPP_B(0x4A007E, local_player_initialize_window_ct);

			//-- address is to the func(x) itself, not to to the patch
			//constexpr uintptr_t render_weapon_hud_loc = 0x4B53E0
			//MPP_B(render_weapon_hud_loc);

			int real_address        = (int)&spcore::player_control::player_control_initialize_for_new_map;
			int real_address_offset = ( real_address ) - ((int)player_control_init_new_map_hook );// + (int)4);
			printf("Calculated - 4 offset: 0x%x\n", real_address_offset - 4);
			//Hooks
			patchValue<uintptr_t>(((unsigned int)player_control_init_new_map_hook ), (unsigned int)( real_address_offset ) - 4);    //Gotta be able to loop over all the players + input devices, no?.

			//Nothing wrong's with the hook, just the function itself.
//			real_address        = (int)&initializations::motion_sensor_initialize_for_new_map;
//			real_address_offset = ( real_address ) - ((int)0x4AC98E);																					// + (int)4);
//
//			patchValue<uintptr_t>(((unsigned int)0x4AC98E ), (unsigned int)( real_address_offset ) - 4);    								//Gotta be able to loop over all the players + input devices, no?.
		}

		void get_mem_and_patch() {
			//TODO: Coneceptually separate these out..
			mem_patch_p1();

			printf("Ran the OG get_mem_and_patches");
			MPP_B(0x4B196A + 0x2, hud_messaging_update_clamp);
			MPP_B(0x4B2787 + 0x3, hud_update_nav_point_local_player_clamp);
			patchRenderPlayerFrameClamp();
			patchRenderWindowCompares();
			initializations::interface_initialize_patches();

			insertRenderWindowCountHooks();
		}
	};
};