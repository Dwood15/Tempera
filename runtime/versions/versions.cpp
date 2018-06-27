#pragma once

#include <precompile.h>
#include "versions.h"

using namespace feature_management;
using namespace feature_management::engines;

LPCoreAddressList CE110::GetCoreAddressList() {
	LPCoreAddressList CurrentCore;
	CurrentCore.core_0 = 0x815900;
	CurrentCore.core_1 = 0x7FB6F8;
	CurrentCore.core_2 = 0x7FBE70;

	CurrentCore.core_3 = 0x87A76C;

	CurrentCore.core_4 = 0x81B800;
	CurrentCore.core_5 = 0x81B894;
	CurrentCore.core_6 = 0x653BE4;
	CurrentCore.core_7 = 0x71D0E8;
	CurrentCore.CAMERA = 0x6474E4;
	CurrentCore.MAP_HEADER_ADDRESS = 0x643044;
	CurrentCore.TAG_INDEX_HEADER_ADDRESS = 0x40440000;

	return CurrentCore;
}

constexpr uintptr_t regular_player_clamps[] = {
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

constexpr std::pair<uintptr_t, char> MPPARBs[] {
	{ 0x476200, sizeof(s_player_control_globals_data) },//, players_initialize_sizeof_a_patch);
	{ 0x47620A, sizeof(s_player_control_globals_data) }, //);
	{ 0x49F897, 0x0 }, //player_spawn_count_hack_fuck_off
	{ 0x45B8D4, 0x7C }, //"precache_new_map_max_spawn_ct_cmp");
	//"FF FF FF 7D 05 89 75 F4 EB 0D .7E 05 89 75 F4 EB"
	//main_game_render's jle effectively clamps us to 1, so we just do unconditional jmp
	{ 0x4CC61A, 0xEB }//, "main_game_render_patch");
};

constexpr std::pair<uintptr_t, short> short_patches[] {
	//"66 8B 41 0C 66 3D 01 00 7C EA .7F E8 0F BF C0 C3"
	//main_get_window_ct
	{ 0x4CC5BA, (short)0x9090}, //We nop the greater than count so we actually get the proper window renderings.
	{ 0x50F5EB, (short)0x9090}, //render_player_frame_jg_patch
};

//Update the player clamps
static inline void run_byte_size_patches() {
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

inline void CE110::WriteHooks() {
	//TODO: Coneceptually separate these out..
	//Function call
	// 			signature: "8B 35 20 59 81 00 57 8B FA B9 .26 00 00 00 F3 AB 83 CF FF"
	//constexpr uintptr_t players_initialize_for_new_map_overwrite = 0x476243; // overwrite the .26 with the size of the 4 player structure.
	//Relying on sizeof allows us to redefine MAX_PLAYER variables/defines
	//patchValue<unsigned int>(players_initialize_for_new_map_overwrite, sizeof(s_players_globals_data) / 4);

	//uintptr_t player_spawn = 0x47A9E0; Valid, just not using it...
	constexpr uintptr_t size_of_fp_weapons = 0x1EA0 * MAX_PLAYER_COUNT_LOCAL;

	uintptr_t fp_weap_initialize[] = {0x497122, 0x49712F};

	spcore::memory::adjustNPatch32(fp_weap_initialize, size_of_fp_weapons);

	//uintptr_t hud_scripted_globals_sizeofs[]          = { 0x4AC7A7, 0x4AC7AF };
	uintptr_t hud_messaging_globals_sizeofs[] = {0x4AC7DD, 0x4AC7EA};
	uintptr_t hud_messaging_state_size        = 0x4AC936;
	//static_assert(sizeof(s_hud_messaging_state)  == 0x122, "stat_assrt_fail: s hud msging state");
	//memset , or rather, rep stosd assumes full integer (0x4) size in this case.
	spcore::memory::patchValue<uintptr_t>(hud_messaging_state_size, sizeof(spcore::s_hud_messaging_state) / 4);
	uintptr_t motion_sensor_sizeofs[] = {0x4AC8B3, 0x4AC8BC + 0x4};

	//			Need to confirm these...
	//			uintptr_t unit_hud_globals_sizeofs[]              = { 0x4AC813, 0x4AC81B + 0x4 };
	//			uintptr_t weapon_hud_globals_sizeofs[]            = { 0x4AC848, 0x4AC850 + 0x4 };
	//			uintptr_t hud_interface_related_globals_sizeofs[] = { 0x4AC87D, 0x4AC885 + 0x4 };

	//adjustNPatch32(hud_scripted_globals_sizeofs, 0x4);
	//0x488 og size.
	spcore::memory::adjustNPatch32(hud_messaging_globals_sizeofs, sizeof(spcore::s_hud_messaging_state));
	//			adjustNPatch32(unit_hud_globals_sizeofs, 0x5C);
	//			adjustNPatch32(weapon_hud_globals_sizeofs, 0x7C);
	//			adjustNPatch32(hud_interface_related_globals_sizeofs, 0x30);
	spcore::memory::adjustNPatch32(motion_sensor_sizeofs, sizeof(s_motion_sensor));

	//Gotta be able to loop over all the players + input devices, no?.
	//"E8 4E 9A 01 00 E8 .69 7D 01 00 8B 15 44 C8 68 00"
	constexpr uintptr_t player_control_init_new_map_hook = 0x45BC33;
	// //Hooks

	calls::WriteSimpleHook(player_control_init_new_map_hook, &spcore::player_control::player_control_initialize_for_new_map);

	//Lua Hooks
	constexpr uintptr_t parse_for_connect_invert_hook = 0x4CAAC4;
	calls::WriteSimpleHook(parse_for_connect_invert_hook, &::parse_for_connect_invert);

	constexpr uintptr_t game_tick_hook = 0x473815;
	calls::WriteSimpleHook(game_tick_hook, &::game_tick);
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
	::run_byte_size_patches();
}

std::string GlobalEngine::GetCurrentFileName() {
	static char name[64];
	auto        lpCmdline = GetCommandLineA();

	auto procName = GetModuleFileName(NULL, name, 64);


	//GetModuleFileNameA(hmodule, name, std::size(name));
	printf("CommandLine Args: %s\n", lpCmdline);
	printf("File Name: %s\n", name);

	int num = 63;
	while (name[num] != 'e') {
		num--;
	}
	int till = num;
	while (name[till] != '\\') {
		till--;
	}

	return std::string(name).substr(till + 1, num - (till));
}

bool GlobalEngine::VerSupported()  {
	if (this->CurrentMajor == major::CE) {
		switch (this->CurrentMinor) {
			case minor::halo_1_10:
				return (CE110::SupportedFeatures() > features::NOPE);
			default:
				return false;
		}
	}

	return false;
}

features GlobalEngine::GetSupported() {
	switch (this->CurrentMajor) {
		case major::CE:
			switch (this->CurrentMinor) {
				case minor::halo_1_10:
					return CE110::SupportedFeatures();
				default:
					return features::NOPE;
			}
		case major::HEK:
			switch (this->CurrentMinor) {
				case minor::sapien:
					return Sapien::SupportedFeatures();
				default:
					return features::NOPE;
			}
		default:
			return features::NOPE;
	}
}


GlobalEngine::GlobalEngine() {
	//Get the current path.
	//auto currentPath = std::experimental::filesystem::current_path();
	//TODO: Something like: "GetGameRegistryPath"

	static wchar_t currentPath[MAX_PATH];

	// GetModuleFileNameA(hmodule, currentPath, 512);
	//Check that the path is valid.

	if (std::experimental::filesystem::_Current_get(currentPath)) {
		printf("Current Path: %ls\n", currentPath);
	}

	auto filename = GetCurrentFileName();

	printf("Filename: %s\n", filename.c_str());

	auto fName = std::string(filename.c_str());

	auto found = fName.find("sapien.exe");

	if (fName == "sapien.exe") {
		printf("Detected sapien!\n");
		this->CurrentMajor = major::HEK;
		this->CurrentMinor = minor::sapien;
		this->DEBUG_FILENAME = Sapien::DEBUG_FILENAME;
		this->current_map = const_cast<defined_functionrange *>(Sapien::GetFunctionMap());
	}

	if (fName == "haloce.exe") {
		printf("Detected haloce!\n");
		this->CurrentMajor = major::CE;
		this->CurrentMinor = minor::halo_1_10;
		this->DEBUG_FILENAME = CE110::DEBUG_FILENAME;
		this->current_map = const_cast<defined_functionrange *>(CE110::GetFunctionMap());
		this->eCore = new Core(CE110::GetCoreAddressList());
		CE110::WriteHooks();
	}

	if (this->HasSupport()) {
		CurrentSupported = this->GetSupported();
	}

}

const char *GlobalEngine::GetCurrentMajorVerString() {
	switch (this->CurrentMajor) {
		case engines::major::DEDI:
			return "DEDI";
		case engines::major::HEK:
			return "HEK";
		case engines::major::PC:
			return "PC";
		case engines::major::MAC:
			return "MAC";
		case engines::major::STUBBZ:
			return "STBZ";
		case engines::major::TRIAL:
			return "TRIL";
		case engines::major::CE:
			return "CE";
		default:
			return "";
	}
}

bool GlobalEngine::HasSupport() {
	if (this->CurrentMajor == major::NO) {
		return false;
	}

	if (IsHek()) {
		return this->IsSapien();
	}

	if (IsCustomEd()) {
		return this->VerSupported();
	}

	return false;
}

uintptr_t GlobalEngine::getFunctionBegin(const char * needle) {
	using dfr = defined_functionrange;

	const dfr* funcList = current_map;

	for (dfr * item = const_cast<dfr *>(funcList); item++;  item) {
		if (equal(needle, item->funcName)) {
			return item->begin;
		}
	}

	return (uintptr_t) -1;
}

const char *GlobalEngine::getMemoryRegionDescriptor(const uintptr_t addr) {

	printf("searching for address: 0x%X\n", addr);

	if (addr < 0x200000) {
		return "probably_in_stack";
	}

	if (addr < 0x400000) {
		return "unmapped_region";
	}
	using dfr = defined_functionrange;

	const dfr* funcList = current_map;
	for (dfr * item = const_cast<dfr*>(funcList); item++;  item) {
		if (item->contains(addr)) {
			return item->funcName;
		}
	}

	if (addr < 0x5E9020) {
		return "function_space_unmapped";
	}

	return "unmapped_region";
}


int l_InMainMenu(lua_State * L) {
	lua_pushboolean(L, *at_main_menu);
	return 1;
}

int l_GetEngineContext(lua_State * L) {

	const char * MAJSTR = CurrentEngine.GetCurrentMajorVerString();

	//TODO More refactor.
	if (strcmp(MAJSTR, "") == 0)
		goto unrecognized;

	lua_pushstring(L, MAJSTR);


	lua_pushinteger(L, CurrentEngine.GetMinorVersion());
	lua_pushboolean(L, true);
	return 3;

unrecognized:
	lua_pushstring(L, "");
	lua_pushnumber(L, -1);
	lua_pushboolean(L, false);
	return 3;
}
