/**
 *	Project: Tempera
 *	File: main.cpp
 *	Copyright � 2009 SilentK, Abyll
 *	Copyright � 2009 Kornman00 (Sean Cooper)
 *	Copyright � 2018 Dwood15 (Dason Woodhouse)
 *
 *  This file is part of Tempera.
 *
 * Tempera is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tempera is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. *
 *
 *  Tempera is under jurisdiction of Microsft Content usage guidelines.
 * It contains no Microsoft code, and is in no way affiliated with
 * Microsfot, Bungie, Gearbox, Saber Interactive, 343 Industries, or
 * any other entity for whom has been granted license to use or otherwise
 * redistribute the Halo Intellectual properties.
 *
 *  This project contains code that has been published and licensed
 * under the GNU General Public License, V3. haloforge and Open Sauce.
 * As such, this project (Tempera) is also licensed as Free Software,
 * under the same GPLv3 license. It is expected that all publications
 * or usages of this codebase will similarly abide by the terms of
 * the GPLv3.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tempera. If not, see <http://www.gnu.org/licenses/>.
 **/

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
	printf("Created LPTHREAD\n");

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
