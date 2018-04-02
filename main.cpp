/*
 * Insert GPLv3 license
 */
#pragma once
#define WIN32_LEAN_AND_MEAN

#include "ceinternal.h"
#include <vector>
#include <Windows.h>
#include <dbghelp.h>

static bool loaded = false;

static inline void init(HMODULE reason) {
	h = AddVectoredExceptionHandler(CALL_FIRST, CEInternalExceptionHandler);
	InitAddLog(reason);
	// some debug outputz
	if( ::AllocConsole( ) != 0 ) {
		freopen_s(&debug_out, "CONOUT$", "w", stdout);
	}
//		printf("init_for_new_map_overwrite addr: 0x%x\n", 0xBEEF);//init_for_new_map_overwrite);

	DWORD old;
	VirtualProtect((void*)0x400000, 0x215000, PAGE_EXECUTE_READWRITE, &old);
	spcore::memory::get_mem_and_patch( );

	DisableThreadLibraryCalls(reason);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hkMain, 0, 0, 0);
	printf("Created LPTHREAD\n");//init_for_new_map_overwrite);

}

static inline void detach() {
	ExitAddLog( );
	RemoveVectoredExceptionHandler(h);
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if( fdwReason == DLL_PROCESS_ATTACH && !loaded ) {
		init(hinstDLL);
		loaded = true;

	} else if( fdwReason == DLL_PROCESS_DETACH && loaded ) {
		detach( );
		loaded = false;
	}
	return true;
}
