#include "shitty_header_ports.h"
#include "queued_sigscanner.h"
#include "function_rewrite.h"

//steam::hook::dx9 core::render::steam_dx9;
namespace core {
	FILE *debug_out;

	namespace memory {
		util::queued_sigscanner scanner;
		auto handle = GetModuleHandleA("haloce.exe");

		void unprotect_memory() {
			DWORD old;
			VirtualProtect((void*)0x400000, 0x215000, PAGE_EXECUTE_READWRITE, &old);
		}

		template<typename T>
		void patchValue(uintptr_t to_patch, T replace_with) {
			*(T *)to_patch = replace_with;
		}

		template<typename T1>
		auto& addsignature(T1 out, const std::string &signature) {
			return scanner.add_signature(handle, out, signature);
		}

		void get_mem_and_patch() {
//			uintptr_t player_spawn{};
//			uintptr_t init_for_new_map_overwrite{};
//			uintptr_t pub_server_patch{};
//			uintptr_t main_game_render_patch{};
//			uintptr_t precache_new_map_max_spawn_ct_cmp{};
//			uintptr_t max_players_player_new{};
//			uintptr_t find_unused_player_index_spawn{};
//			uintptr_t main_get_window_ct{};
//			uintptr_t get_player_input_blob_clamping_patch{};
//
//			//Function call
//			uintptr_t player_control_init_new_map_hook{};
//			//uintptr_t main_game_render_patch2{};
//			addsignature(&player_spawn, "81 EC B4 00 00 00 8B A1 ?? ?? ?? ?? 85 C0 53 B3 01 C6");
//			addsignature(&init_for_new_map_overwrite, "8B 35 20 59 81 00 57 8B FA B9 .26 00 00 00 F3 AB 83 CF FF");
//			addsignature(&pub_server_patch, "33 C0 83 F9 FF 74 05 B8 .01 00 00 00 66 89 46 0C");
//			addsignature(&main_game_render_patch, "FF FF FF 7D 05 89 75 F4 EB 0D .7E 05 89 75 F4 EB");
//			addsignature(&precache_new_map_max_spawn_ct_cmp, "E8 64 85 FE FF 66 83 3D 9C 4A 62 00 01 .75 3C A1 1C FE 6A 00");
//			addsignature(&max_players_player_new, "90 39 1C B1 74 0A 46 83 FE .01 7C F5 5E 5B 59 C3");
//			addsignature(&find_unused_player_index_spawn, "46 83 FE .04 7C F1 8B C7 5F 5E C3 5F 8B C6 5E C3");
//			addsignature(&main_get_window_ct, "66 8B 41 0C 66 3D 01 00 7C EA .7F E8 0F BF C0 C3");
//			addsignature(&player_control_init_new_map_hook, "E8 4E 9A 01 00 .E8 69 7D 01 00 8B 15 44 C8 68 00");
//			addsignature(&get_player_input_blob_clamping_patch, "74 15 66 83 F9 .01 7D 0F 8B 15 18 59 81 00");
//			scanner.scan();
//
//			unprotect_memory();
//			//Relying on sizeof allows us to redefine MAX_PLAYER variables/defines
//			patchValue<int>(init_for_new_map_overwrite, sizeof(s_players_globals_data));
//			patchValue<int>(pub_server_patch, MAX_PLAYER_COUNT_LOCAL);  //max_player_count_local_patch
//			patchValue<int>(main_get_window_ct, 0x9090);				//We nop the greater than count so we actually get the proper window renderings.
//
//			//Byte Patches
//			patchValue<byte>(main_game_render_patch, 0xEB);				//main_game_render's jle effectively clamps us to 1, so we just do unconditional jmp
//			patchValue<byte>(precache_new_map_max_spawn_ct_cmp, 0x7C);	//75 is jne, we're gonna replace it with jl.
//			patchValue<byte>(max_players_player_new, MAX_PLAYER_COUNT_LOCAL);		//
//			patchValue<byte>(find_unused_player_index_spawn, MAX_PLAYER_COUNT_LOCAL);	//This patch isn't technically necessary but I'd rather have this sig documented than not.
//			patchValue<byte>(player_control_init_new_map_hook, 0x9A);	//This patch isn't technically necessary but I'd rather have this sig documented than not.
//			patchValue<byte>(get_player_input_blob_clamping_patch, MAX_PLAYER_COUNT_LOCAL);
//
//			//Hooks
//			patchValue<int>(player_control_init_new_map_hook+1, (int)&complete_replacements::player_control_initialize_for_new_map);	//Gotta be able to loop over all the players + input devices, no?.

			printf("player_spawn addr: 0x%x\n", 0xDEADBEEF);
			printf("init_for_new_map_overwrite addr: 0x%x\n", 0xBEEF);//init_for_new_map_overwrite);
		}
	};

	void init() {
		// some debug outputz
		if (::AllocConsole() != 0) {
			freopen_s(&debug_out, "CONOUT$", "w", stdout);
		}

		memory::get_mem_and_patch();
	}
}
