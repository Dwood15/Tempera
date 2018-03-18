#pragma once
#ifndef ceinternal
#define ceinternal
#include "patching_hooking.h"
#include "function_rewrite.h"
#include "shitty_header_ports.h"
#include "haloforge/headers/addlog.h"
#include "haloforge/headers/forgeMain.h"
//steam::hook::dx9 core::render::steam_dx9;

#define MOTION_SENSOR_LOCATION 0x6B44C8

namespace spcore {

	FILE *debug_out;

	void init(HMODULE reason);

	void unload();

	namespace memory {

		//signature: "81 EC B4 00 00 00 8B 0D ?? ?? ?? ?? 53 8B D8"
		uintptr_t player_spawn = 0x47A9E0;
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

		//"66 8B 41 0C 66 3D 01 00 7C EA .7F E8 0F BF C0 C3"
		uintptr_t main_get_window_ct = 0x4CC5BA;

		//"E8 4E 9A 01 00 .E8 69 7D 01 00 8B 15 44 C8 68 00"
		uintptr_t player_control_init_new_map_hook = 0x45BC33;

		//, "74 15 66 83 F9 .01 7D 0F 8B 15 18 59 81 00");
		uintptr_t get_player_input_blob_clamping_patch = 0x473C86;
		uintptr_t player_ui_get_single_player_local_player_from_controller = 0x498470;

		void get_mem_and_patch();
	};
};
#endif