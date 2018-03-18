#pragma once
#include "ceinternal.h"

namespace spcore {
	void init(HMODULE reason) {
		// some debug outputz
		if (::AllocConsole() != 0) {
			freopen_s(&debug_out, "CONOUT$", "w", stdout);
		}
		printf("init_for_new_map_overwrite addr: 0x%x\n", 0xBEEF);//init_for_new_map_overwrite);

		DWORD old;
		VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READWRITE, &old);
		memory::get_mem_and_patch();

		DisableThreadLibraryCalls(reason);
		InitAddLog(reason);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) hkMain, 0, 0, 0);
		printf("Created LPTHREAD\n");//init_for_new_map_overwrite);

		return;
	}

	void unload() {
		ExitAddLog();
	}

	void memory::get_mem_and_patch() {
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
}