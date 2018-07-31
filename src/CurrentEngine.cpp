
#include "CurrentEngine.h"
#include "lua/script_manager.h"
#include "ce/1_10/110EngineManager.h"
#include <hek/sapien/sapienEngineManager.h>
#include <addlog.h>
#include <experimental/filesystem>

using namespace feature_management;
using namespace feature_management::engines;

//If it's not here, it won't ever update...
static volatile s_player_action ActionOverrides[MAX_PLAYER_COUNT_LOCAL];

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

bool GlobalEngine::IsCoreInitialized() volatile {
	auto IsNegOne = eCore == reinterpret_cast<Core *>(-1);
	auto IsZero   = eCore == reinterpret_cast<Core *>(0);

	return !IsNegOne && !IsZero;
}

void GlobalEngine::InitializeMemoryUpgrades() {
	if(this->HasSupport() && !this->IsHek()) {
		if(this->IsCustomEd() && this->CurrentMinor == feature_management::engines::minor::halo_1_10) {
			CE110::InitializeMemoryUpgrades();
		}



	}
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

auto GlobalEngine::ScenarioGlobals() {
	if(!this->IsCoreInitialized() ||} !eCore->scenario_globals) {
		return static_cast<Yelo::Scenario::s_scenario_globals *>(nullptr);
	}

	return eCore->scenario_globals;
}

void GlobalEngine::InitializeLuaState() {
	PrintLn("Attempting to initialize LuaScript manager");
	LuaState->InitializeLua(LUA_FILENAME);
}

LuaScriptManager *GlobalEngine::GetLuaState() volatile {
	if (!LuaState->IsLoaded()) {
		this->InitializeLuaState();
	}

	return LuaState;
}

IDirectInput8A *GlobalEngine::GetDInput8Device() {
	if (this->IsCustomEd()) {
		return CE110::GetDInput8Device();
	}
	return NULL;
}

IDirectInputDevice8A *GlobalEngine::GetKeyboardInput() {
	if (this->IsCustomEd()) {
		return CE110::GetKeyboardInput();
	}
	return NULL;
}

IDirectInputDevice8A *GlobalEngine::GetMouseInput() {
	if (this->IsCustomEd()) {
		return CE110::GetMouseInput();
	}	return NULL;
}

IDirectInputDevice8A **GlobalEngine::GetJoystickInputs() {
	if (this->IsCustomEd()) {
		return CE110::GetJoystickInputs();
	}
	return NULL;
}

void GlobalEngine::WriteHooks() {
	PrintLn("\nWriting the game hooks now!");
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

auto GlobalEngine::GetHsFunctionTableCountReferences16() {
	if (this->HasSupport()) {
		if (this->IsCustomEd()) {
			return CE110::GetHsFunctionTableCountReferences16();
		}
	}

	return nullptr;
}
auto GlobalEngine::GetHsFunctionTableCountReferences32() {
	if (this->HasSupport()) {
		if (this->IsCustomEd()) {
			return CE110::GetHsFunctionTableCountReferences32();
		}
	}

	return nullptr;
}

auto GlobalEngine::GetHsFunctionTableReferences() {
	if (this->HasSupport()) {
		if (this->IsCustomEd()) {
			return CE110::GetHsFunctionTableReferenceList();
		}
	}

	return nullptr;
}

size_t GlobalEngine::GetNumberOfFunctionTableReferences() {
	if (this->IsCustomEd()) {
		return 354u;
	}

	return 0;
}

void GlobalEngine::RefreshCore() volatile {
	if (this->HasSupport()) {
		if (this->IsSapien()) {
			eCore = new Core;

			eCore->core_0 = new _core_0;
			eCore->core_1 = new _core_1;

			eCore->core_0->PlayersGlobals = *(s_players_globals_data **) 0x1057538;
			eCore->core_0->Teams          = *(data_header<void> **) 0x1057548;
			eCore->core_0->Players        = *(data_header<player> **) 0x105754C;

			// object_list_header_data
			eCore->core_1->Object = *(data_header<object_header> **) 0x107F2E4;

			eCore->at_main_menu                = (bool *) 0xDBD909;
			eCore->player_control_globals_data = *reinterpret_cast<s_player_control_globals_data **>(0xDF76B0); // = player_control_globals *
			eCore->game_time_globals           = *reinterpret_cast<Yelo::GameState::s_game_time_globals **>(0xBD8A18);
			return;
		}

		if (this->IsCustomEd()) {
			if (this->IsCoreInitialized()) {
				delete eCore;
				eCore = (Core *) -1;
			}

			eCore = new Core(CE110::GetCoreAddressList());
			eCore->game_time_globals = *reinterpret_cast<Yelo::GameState::s_game_time_globals **>(0x68CD70);

			eCore->main_globals_game_connection_type = (ushort *) 0x6B47B0;
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
	PrintLn<false>("Should be registering callback: %s", cb_name.c_str());
	LuaState->registerLuaCallback(cb_name, cb_type);
}

bool GlobalEngine::ShouldOverrideAction(ushort idx) {
	return ShouldOverride[idx];
}

void GlobalEngine::LuaFirstRun() {
	if (LuaState) {
		LuaState->DoFirstRun();
	} else {
		PrintLn("Lua Failed to initialize & run!");
	}
}

static void ClampIndex(ushort &idx) {
	if (idx > MAX_PLAYER_COUNT_LOCAL) {
		Print("Forced player to 0 b/c it %d > %d", idx, MAX_PLAYER_COUNT_LOCAL);
		idx = 0;
	}
}

void GlobalEngine::ResetOverride(ushort idx) {
	ClampIndex(idx);

	ShouldOverride[idx] = false;
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

s_player_action GlobalEngine::GetPlayerActionOverride(ushort idx, s_unit_control_data *from) {
	ClampIndex(idx);

	s_player_action newControl;

	newControl.desired_grenade_index = from->grenade_index;
	newControl.primary_trigger       = from->primary_trigger;
	newControl.throttle_forwardback  = from->throttle.x;
	newControl.throttle_leftright    = from->throttle.y;
	newControl.desired_weapon_index  = from->weapon_index;
	newControl.control_flagsA        = from->control_flags;

	this->LuaState->lua_on_player_update(&newControl, idx);

	return newControl;
}

Core *GlobalEngine::GetCore() volatile {
	if (!IsCoreInitialized()) {
		RefreshCore();
	}

	return eCore;
}

void GlobalEngine::SetPlayerPrimaryTriggerFlag(ushort idx = 0) {
	if (!IsSapien() && !IsCustomEd()) {
		return;
	}
	ClampIndex(idx);

	ActionOverrides[idx].control_flagsA.control_flags.primary_fire_button = 1;
	ActionOverrides[idx].control_flagsB.control_flags.primary_fire_button = 1;

	ShouldOverride[idx] = true;
}

void GlobalEngine::SetPlayerTriggerPressure(float t, ushort idx = 0) {
	if (!IsSapien() && !IsCustomEd()) {
		return;
	}
	ClampIndex(idx);

	ActionOverrides[idx].primary_trigger = t;

	ShouldOverride[idx] = true;
}

void GlobalEngine::SetPlayerLookX(float x, ushort idx = 0) {
	if (!IsSapien() && !IsCustomEd()) {
		return;
	}
	ClampIndex(idx);

	ActionOverrides[idx].desired_facing_pitch = x;

	ShouldOverride[idx] = true;

}

void GlobalEngine::SetPlayerLookY(float y, ushort idx = 0) {
	if (!IsSapien() && !IsCustomEd()) {
		return;
	}
	ClampIndex(idx);

	ActionOverrides[idx].desired_facing_yaw = y;

	ShouldOverride[idx] = true;

}

void GlobalEngine::SetPlayerXVelocity(float x, ushort idx = 0) {
	if (!IsSapien() && !IsCustomEd()) {
		return;
	}

	ClampIndex(idx);

	ActionOverrides[idx].throttle_forwardback = x;

	ShouldOverride[idx] = true;

}

void GlobalEngine::SetPlayerYVelocity(float y, ushort idx = 0) {
	if (!IsSapien() && !IsCustomEd()) {
		return;
	}
	ClampIndex(idx);

	ActionOverrides[idx].throttle_leftright = y;

	ShouldOverride[idx] = true;

}

void GlobalEngine::MakePlayerJump(ushort idx = 0) {
	if (!IsSapien() && !IsCustomEd()) {
		Print("Can't make player go forward because the version is unsupported");
		return;
	}
	ClampIndex(idx);

	//TODO: Which is the _real_ jump button?
	ActionOverrides[idx].control_flagsA.control_flags.jump_button = 1;
	ActionOverrides[idx].control_flagsB.control_flags.jump_button = 1;

	ShouldOverride[idx] = true;

}

std::optional<uintptr_t> GlobalEngine::getFunctionBegin(const char *needle) {
	using dfr = defined_functionrange;

	const dfr *funcList = current_map;

	for (dfr *item = const_cast<dfr *>(funcList); item++; item) {
		if (equal(needle, item->funcName)) {
			return item->begin;
		}
	}

	return (uintptr_t) -1;
}

const char *GlobalEngine::getMemoryRegionDescriptor(const uintptr_t addr) volatile {

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

//Calls every registered lua function by that event.
#include "enums/generic_enums.h"

#define CALL_LUA_BY_EVENT(event) state->call_lua_event_by_type(LuaCallbackId::#event)

void main_setup_connection_init() {
	static std::optional<uintptr_t> got = FUNC_GET(main_setup_connection);

	if (got) {
		calls::DoCall<Convention::m_cdecl>(*got);
	}

	static LuaScriptManager *state = 0;

	if (state == 0) {
		state = vEngine->GetLuaState();
	}

	vEngine->RefreshCore();
	state->call_lua_event_by_type(LuaCallbackId::post_initialize);
	// CALL_LUA_BY_EVENT(post_initialize);
}

static Core *mCore = 0x0;

void game_tick(int current_frame_tick) {
	static LuaScriptManager *state = 0;

	if (state == 0) {
		state = vEngine->GetLuaState();
	}

	CALL_LUA_BY_EVENT(before_game_tick);

	if (!mCore) {
		mCore = vEngine->GetCore();
	}

	state->lua_on_tick(current_frame_tick, mCore->game_time_globals->elapsed_time);
	static std::optional<uintptr_t> got = FUNC_GET(game_tick);

	if (got) {
		// PrintLn("Game_tick call");
		calls::DoCall<Convention::m_cdecl, void, int>(*got, current_frame_tick);
		// PrintLn("Post-Game_tick call");
	}

	// PrintLn("Lua_post_tick call");
	CALL_LUA_BY_EVENT(after_game_tick);
	// PrintLn("Post-Lua-post-tick call");
}

#undef CALL_LUA_BY_EVENT
