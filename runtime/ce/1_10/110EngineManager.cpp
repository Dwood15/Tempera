#include <utility>
#include <macros_generic.h>
#include <enums/memory_enums.h>
#include "../../common/addlog.h"
#include <engine_interface.h>
#include "110EngineManager.h"
#include "function_rewrite.h"
#include "../../../src/gamestate/player_types.h"
#include "../../../src/hs/structures.h"
#include "../../../src/RuntimeManager.h"
#include "hs_function_table_references.h"
#include "../../../src/gamestate/runtime_data.h"

using namespace feature_management::engines;

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

	//main game render
	0x4CC604 + 0x1,

	// 			//-- address is to the func(x) itself, not to to the patch
	// 			//constexpr uintptr_t render_weapon_hud_loc = 0x4B53E0
	// 			//MPP_B(render_weapon_hud_loc);

	///hud_messaging_update_clamps
	///That whole function has a fuckton of clamps that need to be inspected and overridden
//	0x4B196A + 0x2,
//	0x4B1993 + 0x2,

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

	0x497584, // "interface_render get_render_window_ct_patch_1");
	0x4975CB, //" get_render_window_ct_patch_2");
	0x49792B, //"Check_render_splitscreen get_render_window_ct_patch_3");
	0x51EB05, //"rasterizer_detail_objects_begin get_render_window_ct_patch_4");
	0x51EE05, //"rasterizer_detail_objects_rebuild_vertices get_render_window_ct_patch_5");
};


constexpr ::std::pair<uintptr_t, unsigned char> MPPARBs[]{
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
	{0x50F5EB, (short) 0x9090}, //render_player_frame_jg_patch
};

namespace feature_management::engines {
	LPCoreAddressList CE110::GetCoreAddressList() {
		PrintLn("Retrieving the Core AddressLists");
		static LPCoreAddressList CurrentCore;
		CurrentCore.core_0 = 0x815900;
		CurrentCore.core_1 = 0x7FB6F8;
		CurrentCore.core_2 = 0x7FBE70;

		CurrentCore.core_3 = 0x0; //0x87A76C; //may be a pc address, not CE lol

		CurrentCore.core_4 = 0x81B800;
		CurrentCore.core_5 = 0x81B894;
		CurrentCore.core_6 = 0x653BE4;
		CurrentCore.core_7 = 0x0; //0x71D0E8; //may be a pc address, not CE lol
		CurrentCore.CAMERA = 0x6474E4;
		CurrentCore.MAP_HEADER_ADDRESS = 0x643044;
		CurrentCore.TAG_INDEX_HEADER_ADDRESS = 0x40440000;

		CurrentCore.CONSOLE_HOOK_ADDRESS = 0x004C9DC0;
		CurrentCore.DEVMODE_HOOK_ADDRESS = 0x004836DB;
		CurrentCore.CONSOLE_TEXT_HOOK_ADDRESS = 0x00499AB0;

		CurrentCore.to_respawn_count = 0x6B4802;
		CurrentCore.spawn_count = 0x624A9C;

		///num windows to render. Fills with black when there is no corresponding player/camera to render.
		CurrentCore.render_window_count = 0x6B4098;
		CurrentCore.at_main_menu = 0x6B4051;

		CurrentCore.hud_scripted_globals = 0x6B44A8;
		CurrentCore.hud_messaging_state = 0x677624;

		CurrentCore.players_global_data = 0x815918;
		CurrentCore.player_control_globals_data = 0x64C38C;

		CurrentCore.game_time_globals = 0x68CD70;
		CurrentCore.game_globals_conn_type = 0x6B47B0;

		CurrentCore.cinematic_globals = 0x68C83C;

		PrintLn("Core AddressLists loaded");

		return CurrentCore;
	}

	namespace static110 {
		void naked OnPlayerActionUpdate() {
#if defined(_MSC_VER) && !defined(__CLANG__)
//			s_player_action *current_action;

			__asm mov dword ptr[esp+1Ch], -1
//			__asm mov current_action, ebp
			__asm retn
#else
			IMPLEMENTATION_REQUIRED
#endif
		}
	};
	static bool printOnce = false;

	//With stdcall the callee saves the stack
	//Since I'm not sure what our actual sitch here is, this should work.
	void __stdcall CE110::OnUnitControlUpdate() {
		unsigned short unit_idx;
		s_unit_control_data *from_control_data;

		//praying to god that eax and edx aren't getting clobbered here :V
		__asm mov unit_idx, ax
		__asm mov from_control_data, edx

		//cannot update a unit's control if core's not initialized.
		if (!::feature_management::engines::IsCoreInitialized()) {
			if (!printOnce) {
				PrintLn("\nOn UnitControl Update when core not initialized, printing once.");
				printOnce = true;
			}
			CurrentRuntime->RefreshCore();

			return;
		}

		Control::UnitControl(unit_idx, from_control_data, 0);
		return;
	}

	inline void patchOnPlayerUpdate() {
		PrintLn("\nAdditional Player action hooks");
		//Originally: C7 44 24 18 FF FF FF FF
		//Basically just sets some random value to -1. Couldn't tell if it was being used or not.
		calls::patchValue<byte>(0x476CF2, 0xE8); //call

		//clear it with some NOPS
		calls::patchValue<byte>(0x476CF7, 0x90); //
		calls::patchValue<unsigned short>(0x476CF8, 0x9090);

		//This OnPlayerActionUpdate Hook is required. For some reason the game
		//Crashes when this is removed.
		calls::WriteSimpleHook(0x476CF3, static110::OnPlayerActionUpdate); //6 bytes off.
	}

	/**
 * 0x49757D - interface_draw_screen
 * - E8 FE 4F 03 00
 *network
 * 0x49792B - see above
* - E8 50 4C 03 00
 *
 * 0x4975C4 - check_render_split_screen
 * - E8 B7 4F 03 00
 *
 * 0x51EB00 rasterizer_detail_objects_begin
 * - E8 7B DA FA FF
 *
 * 0x51EE00 - rasterizer_detail_objects_rebuild_vertices
 * - E8 7B D7 FA FF
 *
 * 0x51EFA3 - rasterizer_detail_objects_draw
 * - E8 D8 D5 FA FF
 */

	namespace overrides {
		int main_get_window_count_override() {
			if (CurrentRuntime->AreWeInMainMenu()) {
				return 1;
			}

			if (CurrentRuntime->AreWeInCutScene()) {
				return 1;
			}

			//Todo: remember to  check if cinematic is playing.
			auto player_count = CurrentRuntime->GetLocalPlayerCount();
			if (player_count > MAX_PLAYER_COUNT_LOCAL || player_count < 1) {
				player_count = 1;
			}

			return (int) player_count;
		}

		void naked main_get_window_ct_wrapper() {
#ifndef _MSC_VER
			static_assert(false, "only msvc is supported, update asm for data_iterator_next_wrapper to match compiler differences");
#endif
			__asm {
				push edx
				call main_get_window_count_override
				pop edx
				retn
			}
		}

		void override_main_get_window_count_calls() {
			//These functions do not get called during a multiplayer game
			constexpr uintptr_t override_function_call_list[] = {
				0x51EFA2, 0x51EE00, 0x51EB00, 0x49792B, 0x4975C4, 0x49757D
			};

			for (auto current : override_function_call_list) {
				//The first instruction of any asm call is E8. We didn't pre-add so we have to do it here.
				calls::WriteSimpleHook(current+1, main_get_window_ct_wrapper);
			}
		}
	};

	namespace initializes {
		namespace forNewMapInitializes {
			void hudInitializeForNewMap() {
				//			uintptr_t unit_hud_globals_sizeofs[]              = { 0x4AC813, 0x4AC81B + 0x4 };
				//			uintptr_t weapon_hud_globals_sizeofs[]            = { 0x4AC848, 0x4AC850 + 0x4 };
				//			uintptr_t hud_interface_related_globals_sizeofs[] = { 0x4AC87D, 0x4AC885 + 0x4 };

				//calls::adjustNPatch32(unit_hud_globals_sizeofs, 0x5C);

				PrintLn("\nPatching the hud messaging globals based on sizeofs");
				//			adjustNPatch32(weapon_hud_globals_sizeofs, 0x7C);
				//			adjustNPatch32(hud_interface_related_globals_sizeofs, 0x30);

				//uintptr_t hud_messaging_state_size = 0x4AC936;
				//calls::patchValue<uintptr_t>(hud_messaging_state_size, sizeof(s_hud_messaging_state) / 4);
			}
		}

		//TODO: Override the initialize function with our own.
		//It would be less complicated and easier to read and understand, at least.
		void hudInitializePatches() {
			//hud_scripted globals (0x6B44A8) is the same size, so we don't worry about that one

			//These hud Initialize patches all need corresponding
			//Initialize for new map overrides as well.
			uintptr_t hud_messaging_globals_sizeofs[] = {0x4AC7DD, 0x4AC7EA};
			calls::adjustNPatch32(hud_messaging_globals_sizeofs, sizeof(s_hud_messaging_state));

			constexpr uintptr_t weapHudInterfaceSzOfs[] = {0x4AC84A, 0x4AC854};

			static_assert((uint) sizeof(s_hud_weapon_interface) < (uint) 0xFF,
						  "Weapon Hud interface size is larger than a byte. We must rewrite our value patches");

			for (auto elem: weapHudInterfaceSzOfs) {
				calls::patchValue<byte>(elem, sizeof(s_hud_weapon_interface));
			}

			//These navPoint SizeOf changes cause the game to crash
//			static_assert(sizeof(s_hud_nav_points) < 0xFF,
//						  "Players nav point state larger than a byte");
//
//			constexpr uintptr_t navPointStateSzOfs[] = {0x4AC87F, 0x4AC889};
//
//			for (auto elem: navPointStateSzOfs) {
//				calls::patchValue<byte>(elem, sizeof(s_hud_nav_points));
//			}
		}

		Memory::s_data_array *game_state_data_new(short object_size, const char * name, int allocated_mem) {
			__int16 num_items; // bp
			int new_cpu_allocation_size; // ecx

			uint gsgPtr = 0;
			uint gsgcpuAllocSz = 0;
			uint gsgCRC = 0;

			CurrentEngine->GetMallocAddresses(gsgPtr, gsgcpuAllocSz, gsgCRC);

			num_items = allocated_mem;
			uint location_allocate_to = *(uintptr) gsgPtr + *(uintptr) gsgcpuAllocSz;

			constexpr auto sDataArray = sizeof(Memory::s_data_array);

			new_cpu_allocation_size = object_size * allocated_mem + sDataArray + gsgcpuAllocSz;
			allocated_mem = object_size * allocated_mem + sDataArray;
			*(uintptr) gsgcpuAllocSz = new_cpu_allocation_size;

			static ::std::optional<uintptr_t> funcFound = CurrentRuntime->getFunctionBegin("malloc_crc_checksum_buffer");
			if ((uint) gsgCRC != (uint) 0x67dd94) {
				PrintLn("Game State Globals CRC does not look like the expected address");
			}

			if (funcFound)
				calls::DoCall<Convention::m_cdecl, void, int, int, uint>(*funcFound, (int) gsgCRC, (int) &allocated_mem,
																		 (uint) 0x4u);

			//Could probably replace this with an actual function call.
			Memory::s_data_array *newItem = (Memory::s_data_array *)location_allocate_to;
			std::memset(newItem, 0, sizeof(Memory::s_data_array));
			std::strncpy(newItem->name, name, 0x1Fu);
			newItem->max_datum = num_items;
			newItem->datum_size = object_size;
			newItem->signature = 'd@t@';
			newItem->base_address = (void*)(&newItem[1]);
			newItem->is_valid = 0;

			return newItem;
		}

		//These multi-layers of indirection are obnoxious lmao
		void scenarioInitializeWrapper() {
			auto addr = CurrentEngine->GetScenarioGlobals();
			CurrentRuntime->GameStateMalloc<Scenario::s_scenario_globals>((void **)addr);

			auto director_camera_scripted = (void**)0x81713C;
			CurrentRuntime->GameStateMalloc<int>(director_camera_scripted);
			(**(int **)director_camera_scripted) = 0;

			Memory::s_data_array ** cached_object_render_states = (Memory::s_data_array **)0x75E44C;
			*(cached_object_render_states) = game_state_data_new(0x100, "cached object render states", 0x100);
		}

		void naked nakedScenarioInitializeWrapper() {
#ifndef _MSC_VER
			static_assert(false, "only msvc is supported, update asm for data_iterator_next_wrapper to match compiler differences");
#endif
			__asm {
				push ebx
				push edx
				push ebp
				mov ebp, esp
				call scenarioInitializeWrapper
				pop ebp
				pop edx
				pop ebx
				retn
			}
		}

		void gameInitializePatches() {
			//Write the hook
			calls::patchValue<byte>(0x45B3A8, 0xE8);
			calls::WriteSimpleHook(0x45B3A9, nakedScenarioInitializeWrapper);
			calls::nopBytes(0x45B3AD, 0x45B42E-0x45B3AD);
		}

		void interfaceInitializePatches() {
			//These all need equivalent "for new map" fixes :grimacing:
			hudInitializePatches();
			gameInitializePatches();
			constexpr uintptr_t size_of_fp_weapons = 0x1EA0 * MAX_PLAYER_COUNT_LOCAL;
			uintptr_t fp_weap_initialize[] = {0x497122, 0x49712F};

			PrintLn("\nAdjustNPatchin' the fp weapons");
			calls::adjustNPatch32(fp_weap_initialize, size_of_fp_weapons);
		}
	};


	static void ValuePatches() {
		//uintptr_t player_spawn = 0x47A9E0; Valid, just not using it...


		//memset , or rather, rep stosd assumes full integer (0x4) size in this case.
		PrintLn("\nPatching the hud messaging state size");

		initializes::interfaceInitializePatches();


		//0x488 og size.

		PrintLn("\nPatching the motion sensor sizeofs");
		uintptr_t motion_sensor_sizeofs[] = {0x4AC8B3, 0x4AC8BC + 0x4};
		calls::adjustNPatch32(motion_sensor_sizeofs, sizeof(s_motion_sensor));


		PrintLn("\nUpgrading the max player count locals to %d", MAX_PLAYER_COUNT_LOCAL);

		for (auto i: regular_player_clamps) {
			calls::patchValue<byte>(i, MAX_PLAYER_COUNT_LOCAL);
		}

		for (auto elem: MPPARBs) {
			calls::patchValue(elem.first, elem.second);
		}

		constexpr auto shortPatches = sizeof(short_patches) / sizeof(::std::pair<uintptr_t, short>);
		PrintLn("\nRunning the generic short patches, num: %d", shortPatches);

		for (auto elem: short_patches) {
			calls::patchValue<short>(elem.first, elem.second);
		}

		PrintLn("\nRender player frame clamp cmp patch - nopBytes");
		//render player frame
		calls::nopBytes(0x50F5F0, 0xA); //render_player_frame_cmp_patch
		//end renderplayerframeclamp

		//_rasterizer_detail_objects_draw51EF90
		//THIS ONE IS THE SAME AS XBOX BETA ALMOST.
		constexpr uintptr_t get_render_window_ct_patch_6 = 0x51EE07;

		static_assert((uint)MAX_PLAYER_COUNT_LOCAL < (uint)0xFF);
		calls::patchValue<short>(get_render_window_ct_patch_6, (short)MAX_PLAYER_COUNT_LOCAL);

		patchOnPlayerUpdate();
	}

	void naked data_iterator_next_wrapper() {
#ifndef _MSC_VER
		static_assert(false, "only msvc is supported, update asm for data_iterator_next_wrapper to match compiler differences");
#endif
		__asm {
		push ebx
		push ebp
		push esi
		push edi

		call Yelo::blam::data_iterator_next

		pop edi
		pop esi
		pop ebp
		pop ebx
		retn
		}
	}



	inline void playerUpdateB4GameSrverIteratorHk() {
		constexpr uintptr_t players_update_before_game_server_iterator_hook = 0x476CB0;
		calls::WriteSimpleHook(players_update_before_game_server_iterator_hook,
							   data_iterator_next_wrapper);
	}

	inline void luaPostInitializeHookMainSetupConnInit() {
		//main_setup_connection_init is ALSO fucked.
		constexpr uintptr_t post_initialize_hook = 0x4CAABA;

		PrintLn("\nWriting the lua main setup connection init hook");
		calls::WriteSimpleHook(post_initialize_hook, main_setup_connection_init);
	}

	inline void gameTickHook() {
		constexpr uintptr_t game_tick_hook = 0x473815;

		PrintLn("Writing the game tick hook");
		calls::WriteSimpleHook(game_tick_hook, game_tick);
	}

	inline void playersInitializeForNewMap() {
		constexpr uintptr_t player_init_for_new_map_hook = 0x46C4EE;
		PrintLn("Writing PlayerInitForNew Map hook");
		calls::WriteSimpleHook(player_init_for_new_map_hook,
							   spcore::initializations::players_initialize_for_new_map);
	}

	inline void countPlayersBeforeGameServerUpdatePatch() {
		calls::patchValue<byte>(0x477106, 0xE8);

		constexpr uintptr_t theHook = 0x477107;
		calls::WriteSimpleHook(theHook, spcore::player_updates::compute_combined_pvs_local);
		calls::nopBytes(0x47710C, 0x477119 - 0x47710C);
	}

	inline void playerControlInitForNewMapHook() {
		//Gotta be able to loop over all the players + input devices, no?.
		//"E8 4E 9A 01 00 E8 .69 7D 01 00 8B 15 44 C8 68 00"
		constexpr uintptr_t player_control_init_new_map_hook = 0x45BC33;

		PrintLn("Writing the player controls hook");
		calls::WriteSimpleHook(player_control_init_new_map_hook,
							   spcore::player_control::player_control_initialize_for_new_map);
	}

	void CE110::WriteHooks() {
		ValuePatches();

		//TODO: Conceptually separate these out..

		//Hooks
		playerUpdateB4GameSrverIteratorHk();

		//Lua Hooks
		luaPostInitializeHookMainSetupConnInit();
		gameTickHook();
		playerControlInitForNewMapHook();

		// OnUnitControlUpdate hook "works" the first time, dies the second.
		PrintLn("Writing the CE110 OnUnitControl Update hook for lua");
		calls::WriteSimpleHook(0x4770CF, CE110::OnUnitControlUpdate);

		//Broken

		// constexpr uintptr_t scenario_load_hookA = 0x4CC9AD; //inside main_new_map
		// addr = calc_addr_offset(scenario_load_hookA, (int)&scenario_load);
		// patchValue<uintptr_t>(scenario_load_hookA, addr); //Gotta be able to loop over all the players + input devices
		//
		// constexpr uintptr_t scenario_load_hookB = 0x4E1C28; //inside network_game_create_game_objects
		// addr = calc_addr_offset(scenario_load_hookB, (int)&scenario_load);
		// patchValue<uintptr_t>(scenario_load_hookB, addr); //Gotta be able to loop over all the players + input devices.

		//This hook is called right before the game enters the actual main loop

		//Nothing wrong's with the hook, just the function.
		//			real_address        = (int)&initializations::motion_sensor_initialize_for_new_map;
		//			real_address_offset = ( real_address ) - ((int)0x4AC98E);																					// + (int)4);
		//
		//			patchValue<uintptr_t>(((unsigned int)0x4AC98E ), (unsigned int)( real_address_offset ) - 4);

		// insertRenderWindowCountHooks();

		overrides::override_main_get_window_count_calls();
		PrintLn("\nWritten shit");
	}

	defined_functionrange *CE110::GetFunctionMap() {
#include "function_map.txt"
//Basic correctness validation/checks
		static_assert(hce110_function_map[0].contains(hce110_function_map[0].begin), "Should be able to find itself");
		static_assert(!hce110_function_map[0].contains(0x0), "Should not find 0x0");
		return const_cast<defined_functionrange *>(hce110_function_map);
	}
};

