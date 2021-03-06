#define WIN32_LEAN_AND_MEAN

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
 *	Copyright � 2018 Dwood15/Zoru (Dason Woodhouse)
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
static volatile bool loaded = false;
static inline void *orig_DirectInput8Create = nullptr;

//Used in order to proxy direct input.
naked void WINAPI Tempera_DirectInput8Create() {
#ifdef __GNUC__
	throw "what the fuck did you say to me you little bitch? I have you know I graduated top of my class in the MIT";
	#else
	__asm { jmp orig_DirectInput8Create };
#endif
}

using feats = feature_management::features;

static void *init(HMODULE *reason) {

//	InitializeDllImageContext((PVOID)(*reason));
	ExceptionHandlerHandle = AddVectoredExceptionHandler(CALL_FIRST, CEInternalExceptionHandler);

	// Dbg output before we get any further (initializes the console)
	if (::AllocConsole() != 0) {
		freopen_s(&debug_out, "CONOUT$", "w", stdout);
	}

	CurrentEngine = feature_management::engines::GlobalEngine::GetGlobalEngine();

	//Initializes the file log for debug output.
	InitAddLog(*reason, feature_management::engines::GlobalEngine::DEBUG_FILENAME);

	if (!feature_management::engines::GlobalEngine::HasSupport()) {
		PrintLn("Tempera could not detect that the current runtime has any feature support.");
		return nullptr;
	}

	PrintLn("The Current runtime does, in fact, have support.");

	//TODO: dinput-agnostic tempera.
	char path[MAX_PATH];
	GetSystemDirectoryA(path, sizeof(path));
	strcat(path, "\\dinput8.dll");

	PrintLn("Loading og dinput8 library.");

	*reason = LoadLibraryA(path);

	if (!*reason) {
		Print("Failed to load the real dinput8 library!");
		return nullptr;
	}

#define SUPPORTSFEAT(FEAT) CurrentEngine->SupportsFeature(feats::FEAT)
#define SUPPORTSFEATS(FEATA, FEATB) CurrentEngine->SupportsFeature((uint)(feats::FEATA | feats::FEATB))
	if (SUPPORTSFEAT(LUA_HOOKS)) {
		feature_management::engines::GlobalEngine::InitializeLuaState();
		feature_management::engines::GlobalEngine::LuaFirstRun();
		PrintLn("\nLua state initialized and FirstRun called");
	}

	orig_DirectInput8Create = (void*)GetProcAddress(*reason, "DirectInput8Create");
	PrintLn("\nDinput8Create");

	DWORD old;

	PrintLn("\nCalling page execute readwrite");

	bool succ = VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READWRITE, &old);

	PrintLn("\nVirtualProtect called, result: [%d]", succ);

	CurrentEngine->WriteHooks();
	PrintLn("\nCurrentEngine.WriteHooks completed");

	//We need to protect memory, I suppose.
	// VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READ, &old);

	PrintLn("\nreturning the original dinput8 create now");

	return orig_DirectInput8Create;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH && !loaded) {
		DisableThreadLibraryCalls(hinstDLL);
		if (!init(&hinstDLL)) {
			PrintLn("Failed to Initialize properly. Exiting");
			return false;
		}

		//Don't setup and run a forge thread for an unsupported runtime target.
		if (SUPPORTSFEATS(DX_PROXY, FORGE_MODE)) {
			//CreateThread(0, 0, (LPTHREAD_START_ROUTINE) forgeMain, 0, 0, 0);
			//PrintLn("Created Forge Thread!");
		}

		PrintLn("\nDllMain loaded = true");

		loaded = true;

	} else if (fdwReason == DLL_PROCESS_DETACH && loaded) {
		PrintLn("\nDll process detach detected");

		ExitAddLog();
		RemoveVectoredExceptionHandler(ExceptionHandlerHandle);
		FreeLibrary(hinstDLL);
		loaded = false;
	}
	return true;
}
#undef SUPPORTSFEAT
#undef SUPPORTSFEATS
