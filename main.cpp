/**
 *	Project: Tempera
 *	File: main.cpp
 *	Copyright � 2009 SilentK, Abyll
 *	Copyright � 2015 Kornman00 (Sean Cooper)
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
 *  Tempera is not under jurisdiction of Microsft Content usage
 * guidelines. It contains no Microsoft code, and is not created nor
 * affiliated with Microsoft, Bungie, Gearbox, 343 Industries, or
 * other entity for whom has been granted license to use or otherwise
 * redistribute the Halo Intellectual properties.
 *
 *  This project contains code that has been published and licensed
 * under the GNU General Public License, from multiple groups, notably:
 * haloforge and Open Sauce.
 *
 * It is expected that all usages of the code contained herein will
 * abide by the terms of the GPLv3.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tempera. If not, see <http://www.gnu.org/licenses/>.
 **/

#pragma once
#define WIN32_LEAN_AND_MEAN

#include "src_generic/ceinternal.h"
#include "src_generic/lua/script_manager.h"
#include <vector>
#include <Windows.h>
#include <dbghelp.h>
#include <mysql.h>

static bool loaded = false;

static void *orig_DirectInput8Create;

//Used in order to proxy direct input.
__declspec(naked) void WINAPI Tempera_DirectInput8Create() {
	__asm { jmp orig_DirectInput8Create };
}

static inline MYSQL *ConnectToSqlDB(const char *host, const char *usr, const char *pwd) {
	MYSQL *con = mysql_init(NULL);

	if (!con) {
		Print(true, "Failed to init mysql connection.");
		return NULL;
	}

	if (mysql_real_connect(con, host, usr, pwd, NULL, 3309, NULL, 0) == NULL) {
		Print(true, "Failed to connect. Error: %s", mysql_error(con));
		mysql_close(con);
		return NULL;
	}

	return con;
}


static inline void *init(HMODULE *reason) {
	h = AddVectoredExceptionHandler(CALL_FIRST, CEInternalExceptionHandler);

	Print(true, "Attempting to initialize luascript manager\n");

	char path[MAX_PATH];
	GetSystemDirectoryA(path, sizeof(path));
	strcat(path, "\\dinput8.dll");

	*reason = LoadLibraryA(path);

	if (!*reason) return false;

	orig_DirectInput8Create = GetProcAddress(*reason, "DirectInput8Create");

	InitAddLog(*reason);

	// some debug outputz
	if (::AllocConsole() != 0) {
		freopen_s(&debug_out, "CONOUT$", "w", stdout);
	}

	LuaState = new LuaScriptManager("init.lua");
	LuaState->beginLua();

	//printf("init_for_new_map_overwrite addr: 0x%x\n", 0xBEEF);//init_for_new_map_overwrite);

	DWORD old;
	VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READWRITE, &old);
	spcore::memory::get_mem_and_patch();

	return orig_DirectInput8Create;
}

static inline void detach(HMODULE reason) {
	ExitAddLog();
	RemoveVectoredExceptionHandler(h);
	FreeLibrary(reason);
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH && !loaded) {
		if (!init(&hinstDLL)) {
			Print(true, "Failed to Initialize properly, I guess. Exiting");
			return false;
		}
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) forgeMain, 0, 0, 0);
		Print(true, "Created Forge Thread!\n");

		//auto sqlCon = ConnectToSqlDB("localhost" );
		// if (sqlCon != NULL) {
		// 	Print(true, "Successfully Connected to Database!");
		// 	mysql_close(sqlCon);
		// }
		loaded = true;

	} else if (fdwReason == DLL_PROCESS_DETACH && loaded) {
		detach(hinstDLL);
		loaded = false;
	}
	return true;
}
