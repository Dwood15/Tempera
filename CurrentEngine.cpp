#pragma once

#include "CurrentEngine.h"
#include "src/lua/script_manager.h"
#include <ce/1_10/110EngineManager.h>
#include <hek/sapien/sapienEngineManager.h>

using namespace feature_management;
using namespace feature_management::engines;

std::string GlobalEngine::GetCurrentFileName() {
	static char        name[MAX_PATH];
	static std::string shortName;
	static bool        nameGot = false;

	if (!nameGot) {
		// static int nameSz     = GetModuleFileNameA(0, name, MAX_PATH);

		//This section copy/pasted form StackOverflow
		//https://stackoverflow.com/a/940743
		// LPBYTE lpBuffer  = NULL;
		// int    verHandle = 0;
		// int verSize   = GetFileVersionInfoSizeA(name, (LPDWORD) &verHandle);
		// uint   size      = 0;

		// if (verSize != 0) {
		// 	char *verData = new char[verSize];
		//
		// 	if (GetFileVersionInfo(name, verHandle, verSize, verData)) {
		// 		if (VerQueryValue(verData, "\\", (void far * far *) &lpBuffer, &size)) {
		// 			if (size) {
		// 				VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *) lpBuffer;
		// 				if (verInfo->dwSignature == 0xfeef04bd) {
		// 					printf("File Version: %d.%d.%d.%d\n",
		// 							 ( verInfo->dwFileVersionMS >> 16 ) & 0xffff,
		// 							 ( verInfo->dwFileVersionMS >>  0 ) & 0xffff,
		// 							 ( verInfo->dwFileVersionLS >> 16 ) & 0xffff,
		// 							 ( verInfo->dwFileVersionLS >>  0 ) & 0xffff
		// 					);
		// 				}
		// 			}
		// 		}
		// 	}
		// }

		GetModuleFileNameA(0, name, std::size(name));
		char *lpCmdline = GetCommandLineA();
		printf("CommandLine Args: %s\n", lpCmdline);
		printf("File Name: %s\n", name);

		int num = MAX_PATH - 1;
		while (name[num] != 'e') {
			num--;
		}
		int till = num;
		while (name[till] != '\\') {
			till--;
		}

		shortName = std::string(name).substr(till + 1, num - (till)).c_str();
		nameGot   = true;
	}
	return shortName;
}

bool GlobalEngine::VerSupported() {
	if (this->CurrentMajor == major::CE) {
		switch (this->CurrentMinor) {
			case minor::halo_1_10:
				return (CE110::SupportedFeatures() > features::NOPE);
			default:
				return false;
		}
	}

	return false;
}

bool GlobalEngine::IsHek() {
	return this->CurrentMajor == major::HEK;
}

bool GlobalEngine::IsSapien() {
	return this->IsHek() && this->CurrentMinor == minor::sapien;
}

bool GlobalEngine::IsCustomEd() {
	return this->CurrentMajor == major::CE;
}

bool GlobalEngine::IsCoreInitialized() {
	return this->eCore != reinterpret_cast<Core *>(-1);
}

features GlobalEngine::GetSupported() {
	switch (this->CurrentMajor) {
		case major::CE:
			switch (this->CurrentMinor) {
				case minor::halo_1_10:
					return CE110::SupportedFeatures();
				default:
					return features::NOPE;
			}
		case major::HEK:
			switch (this->CurrentMinor) {
				case minor::sapien:
					return Sapien::SupportedFeatures();
				default:
					return features::NOPE;
			}
		default:
			return features::NOPE;
	}
}

constexpr bool GlobalEngine::equal(const char *lhs, const char *rhs) {
	while (*lhs || *rhs) {
		if (*lhs++ != *rhs++) {
			return false;
		}
	}
	return true;
}

void GlobalEngine::InitializeLuaState() {
	Print(true, "Attempting to initialize LuaScript manager\n");
	LuaState->InitializeLua(LUA_FILENAME);
}

LuaScriptManager *GlobalEngine::GetLuaState() {
	if (!LuaState->IsLoaded()) {
		this->InitializeLuaState();
	}

	return LuaState;
}

void GlobalEngine::WriteHooks() {
	Print(true, "\nWriting the game hooks now!");
	switch (this->CurrentMajor) {
		case major::CE:
			switch (this->CurrentMinor) {
				case minor::halo_1_10:
					CE110::WriteHooks();
				default:
					return;
			}
		case major::HEK:
			switch (this->CurrentMinor) {
				case minor::sapien:
					Sapien::WriteHooks();
				default:
					return;
			}
		default:
			return;
	}
}

GlobalEngine::GlobalEngine() {
	//Get the current path.
	//auto currentPath = std::experimental::filesystem::current_path();
	//TODO: Something like: "GetGameRegistryPath"

	static wchar_t currentPath[MAX_PATH];

	// GetModuleFileNameA(hmodule, currentPath, 512);
	//Check that the path is valid.

	if (std::experimental::filesystem::_Current_get(currentPath)) {
		printf("Current Path: %ls\n", currentPath);
	}

	auto filename = GetCurrentFileName();

	printf("Filename: %s\n", filename.c_str());

	auto fName = std::string(filename.c_str());

	auto found = fName.find("sapien.exe");

	if (fName == "sapien.exe") {
		printf("Detected sapien!\n");
		this->CurrentMajor   = major::HEK;
		this->CurrentMinor   = minor::sapien;
		this->DEBUG_FILENAME = Sapien::DEBUG_FILENAME;
		this->current_map    = const_cast<defined_functionrange *>(Sapien::GetFunctionMap());

	}

	if (fName == "haloce.exe") {
		printf("Detected haloce!\n");
		this->CurrentMajor   = major::CE;
		this->CurrentMinor   = minor::halo_1_10;
		this->DEBUG_FILENAME = CE110::DEBUG_FILENAME;
		this->current_map    = const_cast<defined_functionrange *>(CE110::GetFunctionMap());
	}

	if (this->HasSupport()) {
		CurrentSupported = this->GetSupported();
	}
}

void GlobalEngine::RefreshCore() {
	if (this->HasSupport()) {
		if (this->IsSapien()) {
			this->eCore = new Core;

			this->eCore->core_0 = new _core_0;
			this->eCore->core_1 = new _core_1;

			this->eCore->core_0->PlayersGlobals = *(s_players_globals_data **) 0x1057538;
			this->eCore->core_0->Teams          = *(data_header<void> **) 0x1057548;
			this->eCore->core_0->Players        = *(data_header<player> **) 0x105754C;

			// object_list_header_data
			this->eCore->core_1->Object = *(data_header<object_header> **) 0x107F2E4;

			this->eCore->at_main_menu                = (bool *) 0xDBD909;
			this->eCore->player_control_globals_data = *(s_player_control_globals_data **) 0xDF76B0; // = player_control_globals *

			return;
		}

		if (this->IsCustomEd()) {
			if (this->IsCoreInitialized()) {
				delete this->eCore;
				this->eCore = (Core *) -1;
			}

			this->eCore = new Core(CE110::GetCoreAddressList());
		}
	}
}

const char *GlobalEngine::GetCurrentMajorVerString() {
	switch (this->CurrentMajor) {
		case engines::major::DEDI:
			return "DEDI";
		case engines::major::HEK:
			switch (this->CurrentMinor) {
				case engines::minor::sapien:
					return "SAPI";
				default:
					return "HEK";
			}
		case engines::major::PC:
			return "PC";
		case engines::major::MAC:
			return "MAC";
		case engines::major::STUBBZ:
			return "STBZ";
		case engines::major::TRIAL:
			return "TRIL";
		case engines::major::CE:
			return "CE";
		default:
			return "";
	}
}

bool GlobalEngine::SupportsFeature(features feat) {
	return (this->GetSupported() & feat) == feat;
}

bool GlobalEngine::SupportsFeature(uint feat) {
	return (this->GetSupported() & feat) == feat;
}

void GlobalEngine::registerLuaCallback(const std::string &cb_name, LuaCallbackId cb_type) {
	Print(false, "Should be registering callback: %s", cb_name.c_str());
	LuaState->registerLuaCallback(cb_name, cb_type);
}

bool GlobalEngine::ShouldOverrideAction() {
	return ShouldOverride;
}

void GlobalEngine::LuaFirstRun() {
	if (LuaState) {
		LuaState->DoFirstRun();
	} else {
		Print(true, "Lua Failed to initialize & run!\n");
	}
}

void GlobalEngine::ResetOverride() {
	ShouldOverride = false;
}

bool GlobalEngine::HasSupport() {
	if (this->CurrentMajor == major::NO) {
		return false;
	}

	if (IsHek()) {
		return this->IsSapien();
	}

	if (IsCustomEd()) {
		return this->VerSupported();
	}

	return false;
}

s_player_action &GlobalEngine::GetPlayerActionOverride(unsigned short idx) {
	Print(true, "Getting Player Action Override.\n");
	if (idx > MAX_PLAYER_COUNT_LOCAL) {
		idx = 0;
	}

	return this->ActionOverrides[idx];
}

Core *GlobalEngine::GetCore() {
	return this->eCore;
}

void GlobalEngine::MakePlayerJump() {
	if (IsSapien() || IsCustomEd()) {
		ShouldOverride = true;
	} else {
		Print(true, "Can't make player go forward because the version is unsupported");
	}
}

uintptr_t GlobalEngine::getFunctionBegin(const char *needle) {
	using dfr = defined_functionrange;

	const dfr *funcList = current_map;

	for (dfr *item = const_cast<dfr *>(funcList); item++; item) {
		if (equal(needle, item->funcName)) {
			return item->begin;
		}
	}

	return (uintptr_t) -1;
}

const char *GlobalEngine::getMemoryRegionDescriptor(const uintptr_t addr) {

	printf("searching for address: 0x%X\n", addr);

	if (addr < 0x200000) {
		return "probably_in_stack";
	}

	if (addr < 0x400000) {
		return "unmapped_region";
	}
	using dfr = defined_functionrange;

	const dfr *funcList = current_map;
	for (dfr  *item     = const_cast<dfr *>(funcList); item++; item) {
		if (item->contains(addr)) {
			return item->funcName;
		}
	}

	if (addr < 0x5E9020) {
		return "function_space_unmapped";
	}

	return "unmapped_region";
}

void post_dll_load() {
	// Print(true, "Postdll-load\n");
	CurrentEngine.GetLuaState()->call_lua_event_by_type(LuaCallbackId::post_dll_init);
}

#define FUNC_GET(funcName)       CurrentEngine.getFunctionBegin(#funcName)
//Calls every registered lua function by that event.
#define CALL_LUA_BY_EVENT(event) state->call_lua_event_by_type(LuaCallbackId::##event)

void main_setup_connection_init() {
	static uint got = 0;

	if (got == 0) {
		got = FUNC_GET(main_setup_connection);
	}

	if (got != (uint) -1) {
		calls::DoCall<Convention::m_cdecl>(got);
	}

	static LuaScriptManager *state = 0;

	if (state == 0) {
		state = CurrentEngine.GetLuaState();
	}

	CurrentEngine.RefreshCore();

	CALL_LUA_BY_EVENT(post_initialize);
}

void game_tick(int current_frame_tick) {
	static LuaScriptManager *state = 0;

	if (state == 0) {
		state = CurrentEngine.GetLuaState();
	}

	CALL_LUA_BY_EVENT(before_game_tick);

	static uint got = 0;
	if (got == 0) {
		got = FUNC_GET(game_tick);
	}
	if (got != (uint) -1) {
		// Print(true, "Game_tick call\n");
		calls::DoCall<Convention::m_cdecl, void, int>(got, current_frame_tick);
		// Print(true, "Post-Game_tick call\n");
	}

	// Print(true, "Lua_post_tick call\n");
	CALL_LUA_BY_EVENT(after_game_tick);
	// Print(true, "Post-Lua-post-tick call\n");
}

#undef CALL_LUA_BY_EVENT
#undef FUNC_GET
