#pragma once
#include "ceinternal.h"

namespace spcore {
	FILE *debug_out;

	void constexpr initCEI(HMODULE reason) {
		// some debug outputz
		if (::AllocConsole() != 0) {
			freopen_s(&debug_out, "CONOUT$", "w", stdout);
		}
		printf("init_for_new_map_overwrite addr: 0x%x\n", 0xBEEF);//init_for_new_map_overwrite);

		DWORD old;
		VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READWRITE, &old);
		memory::get_mem_and_patch();

		DisableThreadLibraryCalls(reason);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) hkMain, 0, 0, 0);
		printf("Created LPTHREAD\n");//init_for_new_map_overwrite);

		return;
	}

	void constexpr unload() {
	}
};