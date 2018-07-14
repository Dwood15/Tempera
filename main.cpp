#include "main.h"
#include "src/CurrentEngine.h"
#include "src/exceptions/exception_handler.h"
#include "src/lua/script_manager.h"
#include <iostream>
#include <addlog.h>

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


static bool loaded = false;
static void *orig_DirectInput8Create;

//Used in order to proxy direct input.
__declspec(naked) void WINAPI Tempera_DirectInput8Create() {
	__asm { jmp orig_DirectInput8Create };
}

static inline void *ConnectToSqlDB(const char *host, const char *usr, const char *pwd) {
	// MYSQL *con = mysql_init(NULL);
	//
	// if (!con) {
	// 	Print("Failed to init mysql connection.");
	// 	return NULL;
	// }
	//
	// if (mysql_real_connect(con, host, usr, pwd, NULL, 3309, NULL, 0) == NULL) {
	// 	PrintLn("Failed to connect. Error: %s", mysql_error(con));
	// 	mysql_close(con);
	// 	return NULL;
	// }

	return NULL;
}

using feats = feature_management::features;

static inline void *init(HMODULE *reason) {
	h        = AddVectoredExceptionHandler(CALL_FIRST, CEInternalExceptionHandler);

	// Dbg output before we get any further (initializes the console)
	if (::AllocConsole() != 0) {
		freopen_s(&debug_out, "CONOUT$", "w", stdout);
	}

	CurrentEngine = feature_management::engines::GlobalEngine();

	//Initializes the file log for debug output.
	InitAddLog(*reason, CurrentEngine.DEBUG_FILENAME);

	if (!CurrentEngine.HasSupport()) {
		PrintLn("Tempera could not detect that the current runtime has any feature support.");
		return false;
	}

	Print("Current runtime was detected!");

	//TODO: dinput-agnostic tempera.
	char path[MAX_PATH];
	GetSystemDirectoryA(path, sizeof(path));
	strcat(path, "\\dinput8.dll");

	*reason = LoadLibraryA(path);

	if (!*reason) {
		Print("Failed to load the real dinput8 library!");
		return false;
	}

#define SUPPORTSFEAT(FEAT) CurrentEngine.SupportsFeature(feats::FEAT)
#define SUPPORTSFEATS(FEATA, FEATB) CurrentEngine.SupportsFeature((uint)(feats::FEATA | feats::FEATB))
	if (SUPPORTSFEAT(LUA_HOOKS)) {
		CurrentEngine.InitializeLuaState();
		CurrentEngine.LuaFirstRun();
	}

	orig_DirectInput8Create = GetProcAddress(*reason, "DirectInput8Create");

	DWORD old;
	VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READWRITE, &old);

	CurrentEngine.WriteHooks();

	//We need to protect memory, I suppose.
	// VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READ, &old);

	//run the post_dll_load lua hook
	if (SUPPORTSFEAT(LUA_HOOKS)) {
		CurrentEngine.GetLuaState()->call_lua_event_by_type(LuaCallbackId::post_dll_init);
	}

	return orig_DirectInput8Create;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH && !loaded) {
		if (!init(&hinstDLL)) {
			Print("Failed to Initialize properly. Exiting");
			return false;
		}

		//Don't setup and run a forge thread for an unsupported runtime target.
		if (SUPPORTSFEATS(DX_PROXY, FORGE_MODE)) {
			//DisableThreadLibraryCalls(hinstDLL);
			//CreateThread(0, 0, (LPTHREAD_START_ROUTINE) forgeMain, 0, 0, 0);
			//PrintLn("Created Forge Thread!");
		}

		if (SUPPORTSFEAT(MARIADB_LOGGING)) {
			//auto sqlCon = ConnectToSqlDB("localhost" );
			// if (sqlCon != NULL) {
			// 	Print("Successfully Connected to Database!");
			// 	mysql_close(sqlCon);
			// }
		}

		loaded = true;

	} else if (fdwReason == DLL_PROCESS_DETACH && loaded) {
		ExitAddLog();
		RemoveVectoredExceptionHandler(h);
		FreeLibrary(hinstDLL);
		loaded = false;
	}
	return true;
}
