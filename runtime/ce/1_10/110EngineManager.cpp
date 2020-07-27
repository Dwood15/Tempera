#include <utility>
#include <macros_generic.h>
#include <enums/memory_enums.h>
#include "../../common/addlog.h"
#include <engine_interface.h>
#include "110EngineManager.h"
#include "function_rewrite.h"
#include "../../../src/gamestate/player_types.h"
#include "../../../src/lua/script_manager.h"
#include "../../../src/hs/structures.h"
#include "../../../src/CurrentEngine.h"
#include "hs_function_table_references.h"
#include "../../../src/gamestate/runtime_data.h"

using namespace feature_management;
using namespace feature_management::engines;

LPCoreAddressList CE110::GetCoreAddressList() {
	PrintLn("Retrieving the Core AddressLists");
	LPCoreAddressList CurrentCore;
	CurrentCore.core_0 = 0x815900;
	CurrentCore.core_1 = 0x7FB6F8;
	CurrentCore.core_2 = 0x7FBE70;

	CurrentCore.core_3 = 0x0; //0x87A76C; //may be a pc address, not CE lol

	CurrentCore.core_4                   = 0x81B800;
	CurrentCore.core_5                   = 0x81B894;
	CurrentCore.core_6                   = 0x653BE4;
	CurrentCore.core_7                   = 0x0; //0x71D0E8; //may be a pc address, not CE lol
	CurrentCore.CAMERA                   = 0x6474E4;
	CurrentCore.MAP_HEADER_ADDRESS       = 0x643044;
	CurrentCore.TAG_INDEX_HEADER_ADDRESS = 0x40440000;

	CurrentCore.CONSOLE_HOOK_ADDRESS      = 0x004C9DC0;
	CurrentCore.DEVMODE_HOOK_ADDRESS      = 0x004836DB;
	CurrentCore.CONSOLE_TEXT_HOOK_ADDRESS = 0x00499AB0;

	CurrentCore.to_respawn_count    = 0x6B4802;
	CurrentCore.spawn_count         = 0x624A9C;
	CurrentCore.render_window_count = 0x6B4098;
	CurrentCore.at_main_menu        = 0x6B4051;

	CurrentCore.hud_scripted_globals = 0x6B44A8;
	CurrentCore.hud_messaging_state  = 0x677624;

	CurrentCore.game_state_globals_location_ptr = 0x67DD8C;
	CurrentCore.game_state_globals_ptr          = 0x67DD88;
	CurrentCore.crc_checksum_buffer             = 0x4D36D0;

	CurrentCore.game_state_cpu_allocation  = *(uint *) CurrentCore.game_state_globals_ptr;
	CurrentCore.game_state_location_as_int = *(uint *) CurrentCore.game_state_globals_location_ptr;

	//static s_players_globals_data *players_global_data       = *(s_players_globals_data **) 0x815918;
	CurrentCore.players_global_data = 0x815918;

	CurrentCore.game_time_globals      = 0x68CD70;
	CurrentCore.game_globals_conn_type = 0x6B47B0;

	PrintLn("Core AddressLists loaded");

	return CurrentCore;
}

constexpr uintptr_t regular_player_clamps[] = {
	0x4B3565, //"hud_update_unit_local_player_clamp"
	0x4B36D4, //"hud_update_unit_local_player_clamp_2"
	// 		patch bytes region
	//"90 39 1C B1 74 0A 46 83 FE .01 7C F5 5E 5B 59 C3"
	0x53D035, //player_profile_save_changes_clamp
	0x4764E8, //player_new_clamp
	// "46 83 FE .04 7C F1 8B C7 5F 5E C3 5F 8B C6 5E C3"
	0x476333, //find_unused_player_index_spawn    //This patch MAY NOT be technically necessary but I'd rather have it documented than not.
	//"74 15 66 83 F9 .01 7D 0F 8B 15 18 59 81 00"
	0x473C86,  // get_player_input_blob_clamping_patch
	0x498470, //player_ui_get_single_player_local_player_from_controller
	(0x4476EF + 0x2), // first_person_camera_update_clamp_patch
	(0x446760 + 0x3),// director_choose_camera_game_clamp
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
	0x45FC67, //"game_engine_post_rasterize_in_game clamp fix"

	0x4476F1, //"first_person_camera_update clamp fix"
	0x474B0D, //"player_control_get_facing_direction clamp fix"

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


constexpr ::std::pair<uintptr_t, char> MPPARBs[]{
	{0x476200, sizeof(s_player_control_globals_data)},//, players_initialize_sizeof_a_patch);
	{0x47620A, sizeof(s_player_control_globals_data)}, //);
	{0x49F897, 0x0}, //player_spawn_count_hack_fuck_off
	{0x45B8D4, 0x7C}, //"precache_new_map_max_spawn_ct_cmp");
	//"FF FF FF 7D 05 89 75 F4 EB 0D .7E 05 89 75 F4 EB"
	//main_game_render's jle effectively clamps us to 1, so we just do unconditional jmp
	{0x4CC61A, 0xEB}//, "main_game_render_patch");
};

constexpr ::std::pair<uintptr_t, short> short_patches[]{
	//"66 8B 41 0C 66 3D 01 00 7C EA .7F E8 0F BF C0 C3"
	//main_get_window_ct
	{0x4CC5BA, (short) 0x9090}, //We nop the greater than count so we actually get the proper window renderings.
	{0x50F5EB, (short) 0x9090}, //render_player_frame_jg_patch
};

void naked CE110::OnPlayerActionUpdate() {
	//ahhhhhhhhhhhhhhhhhhhhhh
	//clangd is dumb-- even with MSVC compat enabled they STILL
	//Do not allow variables in naked funcs.

	//Postmortem: this didn't work btw, I actually just ended up disabling clangd. /shrug
#if defined(_MSC_VER) && !defined(__CLANG__)
	s_player_action *current_action;

	__asm mov dword ptr[esp+18h], -1
	__asm mov current_action, ebp
	__asm retn
#else
	IMPLEMENTATION_REQUIRED
#endif
}

void naked CE110::OnUnitControlUpdate(int client_update_idx) {
	//PrintLn("\nOn UnitControl Update");
	static bool printOnce = false;

	//cannot update a unit's control if core's not initialized.
	if (!CurrentEngine.IsCoreInitialized()) {
		if (!printOnce) {
			PrintLn("\nOn UnitControl Update when core not initialized, printing once.");
			printOnce = true;
		}

		__asm retn;
	}

#if !defined(__GNUC__) && !defined(__CLANG__)
	unsigned short      unit_idx;
	s_unit_control_data *from_control_data;

	__asm mov unit_idx, ax
	__asm mov from_control_data, edx

	Control::UnitControl(unit_idx, from_control_data, client_update_idx);

	__asm retn
#else
	IMPLEMENTATION_REQUIRED
#endif
}

void CE110::WriteHooks() {
	//TODO: Conceptually separate these out..
	//Function call
	// 			signature: "8B 35 20 59 81 00 57 8B FA B9 .26 00 00 00 F3 AB 83 CF FF"
	//constexpr uintptr_t players_initialize_for_new_map_overwrite = 0x476243; // overwrite the .26 with the size of the 4 player structure.
	//Relying on sizeof allows us to redefine MAX_PLAYER variables/defines
	//patchValue<unsigned int>(players_initialize_for_new_map_overwrite, sizeof(s_players_globals_data) / 4);

	//uintptr_t player_spawn = 0x47A9E0; Valid, just not using it...
	constexpr uintptr_t size_of_fp_weapons = 0x1EA0 * MAX_PLAYER_COUNT_LOCAL;

	uintptr_t fp_weap_initialize[] = {0x497122, 0x49712F};

	PrintLn("\nAdjustNPatchin' the fp weapons");
	calls::adjustNPatch32(fp_weap_initialize, size_of_fp_weapons);

	//uintptr_t hud_scripted_globals_sizeofs[]          = { 0x4AC7A7, 0x4AC7AF };
	uintptr_t hud_messaging_globals_sizeofs[] = {0x4AC7DD, 0x4AC7EA};
	uintptr_t hud_messaging_state_size        = 0x4AC936;
	//static_assert(sizeof(s_hud_messaging_state)  == 0x122, "STAT_ASSERT_fail: s hud msging state");
	//memset , or rather, rep stosd assumes full integer (0x4) size in this case.
	PrintLn("\nPatching the hud messaging state size");
	calls::patchValue<uintptr_t>(hud_messaging_state_size, sizeof(::s_hud_messaging_state) / 4);
	uintptr_t motion_sensor_sizeofs[] = {0x4AC8B3, 0x4AC8BC + 0x4};

	//			Need to confirm these...
	//			uintptr_t unit_hud_globals_sizeofs[]              = { 0x4AC813, 0x4AC81B + 0x4 };
	//			uintptr_t weapon_hud_globals_sizeofs[]            = { 0x4AC848, 0x4AC850 + 0x4 };
	//			uintptr_t hud_interface_related_globals_sizeofs[] = { 0x4AC87D, 0x4AC885 + 0x4 };

	//adjustNPatch32(hud_scripted_globals_sizeofs, 0x4);
	//0x488 og size.
	PrintLn("\nPatching the hud messaging globals based on sizeofs");
	calls::adjustNPatch32(hud_messaging_globals_sizeofs, sizeof(::s_hud_messaging_state));
	//			adjustNPatch32(unit_hud_globals_sizeofs, 0x5C);
	//			adjustNPatch32(weapon_hud_globals_sizeofs, 0x7C);
	//			adjustNPatch32(hud_interface_related_globals_sizeofs, 0x30);
	PrintLn("\nPatching the motion sensor sizeofs");
	calls::adjustNPatch32(motion_sensor_sizeofs, sizeof(s_motion_sensor));


	//Gotta be able to loop over all the players + input devices, no?.
	//"E8 4E 9A 01 00 E8 .69 7D 01 00 8B 15 44 C8 68 00"
	constexpr uintptr_t player_control_init_new_map_hook = 0x45BC33;
	// //Hooks

	PrintLn("\nWriting the player controls hook");
	calls::WriteSimpleHook(player_control_init_new_map_hook, &spcore::player_control::player_control_initialize_for_new_map);

	PrintLn("\nWriting the update before game server iterator_next wrapper hook");
	constexpr uintptr_t players_update_before_game_server_iterator_hook = 0x476CB0;
	calls::WriteSimpleHook(players_update_before_game_server_iterator_hook, &Yelo::blam::data_iterator_next_wrapper);

	//Lua Hooks
	constexpr uintptr_t post_initialize_hook = 0x4CAABA;

	PrintLn("\nWriting the lua main setup connection init hook");
	calls::WriteSimpleHook(post_initialize_hook, &main_setup_connection_init);

	constexpr uintptr_t game_tick_hook = 0x473815;

	PrintLn("\nWriting the game tick hook");
	calls::WriteSimpleHook(game_tick_hook, &game_tick);

	PrintLn("\nAdditional Player action hooks");
	//Originally: C7 44 24 18 FF FF FF FF
	//Basically just sets some random value to -1. Couldn't tell if it was being used or not.
	calls::patchValue<byte>(0x476CF2, 0xE8); //call
	calls::WriteSimpleHook(0x476CF3, &CE110::OnPlayerActionUpdate); //6 bytes off.
	calls::patchValue<byte>(0x476CF7, 0x90); //
	calls::patchValue<unsigned short>(0x476CF8, 0x9090);

	PrintLn("\nCE110 OnUnitControl Update hook for lua");
	calls::WriteSimpleHook(0x4770CF, &CE110::OnUnitControlUpdate);

	// patchValue<uintptr_t>(player_control_init_new_map_hook, addr); //Gotta be able to loop over all the players + input devices, no?.

	// constexpr uintptr_t scenario_load_hookA = 0x4CC9AD; //inside main_new_map
	// addr = calc_addr_offset(scenario_load_hookA, (int)&scenario_load);
	// patchValue<uintptr_t>(scenario_load_hookA, addr); //Gotta be able to loop over all the players + input devices, no?.
	//
	// constexpr uintptr_t scenario_load_hookB = 0x4E1C28; //inside network_game_create_game_objects
	// addr = calc_addr_offset(scenario_load_hookB, (int)&scenario_load);
	// patchValue<uintptr_t>(scenario_load_hookB, addr); //Gotta be able to loop over all the players + input devices, no?.

	//This hook is called right before the game enters the actual main loop

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

	PrintLn("\nUpgrading the max player count locals to %d", MAX_PLAYER_COUNT_LOCAL);

	for (auto i : regular_player_clamps) {
		calls::patchValue<byte>(i, MAX_PLAYER_COUNT_LOCAL);
	}

	for (auto elem : MPPARBs) {
		calls::patchValue<byte>(elem.first, elem.second);
	}

	PrintLn("\nRunning the generic short patches, num: %d", sizeof(short_patches) / sizeof(::std::pair<uintptr_t, short>));

	for (auto elem : short_patches) {
		calls::patchValue<short>(elem.first, elem.second);
	}

	PrintLn("\nRender player frame clamp cmp patch - nopBytes");
	//render player frame
	calls::nopBytes(0x50F5F0, 0xA); //render_player_frame_cmp_patch
	//end renderplayerframeclamp

	PrintLn("\nWritten shit");
}

#include "function_map.txt"
//Basic correctness validation/checks
static_assert(hce110_function_map[0].contains(hce110_function_map[0].begin), "Should be able to find itself");
static_assert(!hce110_function_map[0].contains(0x0), "Should not find 0x0");
const defined_functionrange *CE110::GetFunctionMap() {
	return hce110_function_map;
}
