#pragma once

#include <precompile.h>
#include "../function_rewrite.h"

static constexpr uintptr_t regular_player_clamps[] = {
	0x4B3565, //"hud_update_unit_local_player_clamp"
	0x4B36D4, //"hud_update_unit_local_player_clamp_2"
	// 		patch bytes region
	// 			//"90 39 1C B1 74 0A 46 83 FE .01 7C F5 5E 5B 59 C3"
	0x53D035, //player_profile_save_changes_clamp);
	0x4764E8, //player_new_clamp);
	 			// "46 83 FE .04 7C F1 8B C7 5F 5E C3 5F 8B C6 5E C3"
	0x476333, //find_unused_player_index_spawn);    //This patch MAY NOT be technically necessary but I'd rather have it documented than not.
	// 			//, "74 15 66 83 F9 .01 7D 0F 8B 15 18 59 81 00");
	0x473C86,  // get_player_input_blob_clamping_patch);
	0x498470, //player_ui_get_single_player_local_player_from_controller);
	(0x4476EF + 0x2), // first_person_camera_update_clamp_patch);
	(0x446760 + 0x3),// director_choose_camera_game_clamp);
	//
	// 			//I can't find the xbox equivalent of this function, so this may not be necessary?
	(0X497930 + 0x2), //check_render_splitscreen_clamp);
	//
	// 			//Yep, this one's the real deal...
	0x4CBBFC + 0x3, //create_local_players_clamp);
	0x4A04B0 + 0x1, //coop_game_initialize);
	0x4A0076, //local_player_initialize_spawn_ct);
	0x4A007E, //local_player_initialize_window_ct);

	// 			//-- address is to the func(x) itself, not to to the patch
	// 			//constexpr uintptr_t render_weapon_hud_loc = 0x4B53E0
	// 			//MPP_B(render_weapon_hud_loc);

	0x4B196A + 0x2, //hud_messaging_update_clamp);
	0x4B2787 + 0x3, //hud_update_nav_point_local_player_clamp);

	//"33 C0 83 F9 FF 74 05 B8 .01 00 00 00 66 89 46 0C");
	0x477115, //players_update_before_game_server patch

	0x4476F1, //"first_person_camera_update clamp fix")
	0x45FC65, //"game_engine_post_rasterize_in_game clamp fix")
	0x474B0D, //"player_control_get_facing_direction clamp fix")

	0x477BEF, //"First cmp of requested_player_index with 1");
	0x477C10, //"2nd cmp of requested_player_index with 1");
	0x4B4D75, //hud_update_weapon_local_player_clamp


	//TODO: CONFIRM FIXES FOR THE CMP'S IMMEDIATELY FOLLOWING THESE CALLS.

	// MPP_B(0x497584, "interface_render get_render_window_ct_patch_1");
	// MPP_B(0x4975CB, " get_render_window_ct_patch_2");
	// MPP_B(0x49792B, "Check_render_splitscreen get_render_window_ct_patch_3");
	// MPP_B(0x51EB05, "rasterizer_detail_objects_begin get_render_window_ct_patch_4");
	// MPP_B(0x51EE05, "rasterizer_detail_objects_rebuild_vertices get_render_window_ct_patch_5");
};

static_assert(sizeof(s_player_control_globals_data) < (unsigned int) 0xFF);
//MPP_ARB(
static constexpr std::pair<uintptr_t, char> MPPARBs[] {
	{ 0x476200, sizeof(s_player_control_globals_data) },//, players_initialize_sizeof_a_patch);
	{ 0x47620A, sizeof(s_player_control_globals_data) }, //);
	{ 0x49F897, 0x0 }, //player_spawn_count_hack_fuck_off
	{ 0x45B8D4, 0x7C }, //"precache_new_map_max_spawn_ct_cmp");
	//"FF FF FF 7D 05 89 75 F4 EB 0D .7E 05 89 75 F4 EB"
	//main_game_render's jle effectively clamps us to 1, so we just do unconditional jmp
	{ 0x4CC61A, 0xEB }//, "main_game_render_patch");
};

static constexpr std::pair<uintptr_t, short> short_patches[] {
	//"66 8B 41 0C 66 3D 01 00 7C EA .7F E8 0F BF C0 C3"
	//main_get_window_ct
	{ 0x4CC5BA, (short)0x9090}, //We nop the greater than count so we actually get the proper window renderings.
	{ 0x50F5EB, (short)0x9090}, //render_player_frame_jg_patch
};


//Update the player clamps
static void const run_byte_size_patches() {
	for (auto i : regular_player_clamps) {
		spcore::memory::patchValue<byte>(i, MAX_PLAYER_COUNT_LOCAL);
	}

	for (auto elem : MPPARBs) {
		spcore::memory::patchValue<byte>(elem.first, elem.second);
	}

	for (auto elem : short_patches) {
		spcore::memory::patchValue<short>(elem.first, elem.second);
	}

	//render player frame
	spcore::memory::nopBytes(0x50F5F0, 0xA); //render_player_frame_cmp_patch
	//end renderplayerframeclamp

}
