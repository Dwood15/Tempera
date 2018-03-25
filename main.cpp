/*
 * Insert GPLv3 license
 */

#define WIN32_LEAN_AND_MEAN

#include "ceinternal/ceinternal.h"
#include <vector>
#include <Windows.h>
#include <dbghelp.h>

static bool loaded = false;

static inline void init(HMODULE reason) {
	h = AddVectoredExceptionHandler(CALL_FIRST, CEInternalExceptionHandler);
	InitAddLog(reason);
	loaded = true;
	spcore::initCEI(reason);
	//initialize_client();
}

static inline void detach() {
	loaded = false;
	spcore::unload();
//    uninitialize_client();
	ExitAddLog();
	RemoveVectoredExceptionHandler(h);
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if( fdwReason == DLL_PROCESS_ATTACH && !loaded ) {
		init(hinstDLL);
	} else if( fdwReason == DLL_PROCESS_DETACH && loaded ) {
		detach();
	}
	return true;
}
