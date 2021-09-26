#define WIN32_LEAN_AND_MEAN

#include "main.h"
#include "src/RuntimeManager.h"
#include "src/exceptions/exception_handler.h"
#include <addlog.h>
#include <thread>

/**
 *	Project: Tempera
 *	File: main.cpp
 *	Copyright � 2021 Dwood15 (Dason Woodhouse)
 *
 *  This file is part of Tempera.
 *
 *  Tempera is not under jurisdiction of Microsft Content usage
 * guidelines. It contains no Microsoft code, and is not created nor
 * affiliated with Microsoft, Bungie, Gearbox, 343 Industries, or
 * other entity for whom has been granted license to use or otherwise
 * redistribute the Halo Intellectual properties.
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

	CurrentRuntime = feature_management::engines::GetRuntimeManager();

	//Initializes the file log for debug output.
	InitAddLog(*reason, CurrentEngine->GetLogFileName());

	if (!feature_management::engines::RuntimeManager::HasSupport()) {
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

#define SUPPORTSFEAT(FEAT) CurrentRuntime->SupportsFeature(feats::FEAT)
#define SUPPORTSFEATS(FEATA, FEATB) CurrentRuntime->SupportsFeature((uint)(feats::FEATA | feats::FEATB))

	orig_DirectInput8Create = (void*)GetProcAddress(*reason, "DirectInput8Create");
	PrintLn("\nDinput8Create");

	DWORD old;

	PrintLn("\nCalling page execute readwrite");

	bool succ = VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READWRITE, &old);

	PrintLn("\nVirtualProtect called, result: [%d]", succ);

	CurrentRuntime->WriteHooks();
	PrintLn("\nCurrentRuntime.WriteHooks completed");

	//We need to protect memory, I suppose.
	// VirtualProtect((void *) 0x400000, 0x215000, PAGE_EXECUTE_READ, &old);

	PrintLn("Returning the original dinput8 create now");

	return orig_DirectInput8Create;
}

void loadProxy(HINSTANCE hinstDLL) {
	DisableThreadLibraryCalls(hinstDLL);
	if (!init(&hinstDLL)) {
		PrintLn("Failed to Initialize properly. Exiting");
		loaded = false;
	}

	//Don't setup and run a forge thread for an unsupported runtime target.
	if (SUPPORTSFEATS(DX_PROXY, FORGE_MODE)) {
		//Shouldn't need to use the windows API stuff for creating and running the forge thread any more
		//std::thread loopThread(forge::MainLoop);
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE) forge::MainLoop, 0, 0, 0);
		//PrintLn("Created Forge Thread!");
	}

	loaded = true;
	PrintLn("DllMain loaded!");
}

void cleanUp(HINSTANCE mod) {
	PrintLn("Dll process detach detected");
	ExitAddLog();
	RemoveVectoredExceptionHandler(ExceptionHandlerHandle);
	FreeLibrary(mod);
	loaded = false;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		if (loaded) {
			auto err = "Trying to load the DLL despite already being loaded wtf";
			PrintLn(err);
			return false;
		}
		loadProxy(hinstDLL);
	} else if (fdwReason == DLL_PROCESS_DETACH) {
		cleanUp(hinstDLL);
	}
	return loaded;
}
#undef SUPPORTSFEAT
#undef SUPPORTSFEATS
