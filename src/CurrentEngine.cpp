#include <filesystem>

#include "CurrentEngine.h"
#include "lua/script_manager.h"
#include "ce/1_10/110EngineManager.h"
#include "exceptions/exception_handler.h"
#include <hek/sapien/sapienEngineManager.h>
#include <addlog.h>
#include <DbgHelp.h>

using namespace feature_management;
using namespace feature_management::engines;

//If it's not here, it won't ever update...
static volatile s_player_action ActionOverrides[MAX_PLAYER_COUNT_LOCAL];

std::string GlobalEngine::GetCurrentFileName() {
	static char          name[MAX_PATH];
	static ::std::string shortName;
	static bool          nameGot = false;

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

		GetModuleFileNameA(0, name, ::std::size(name));
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

		shortName = ::std::string(name).substr(till + 1, num - (till)).c_str();
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
	return core_initialized;
}

//void GlobalEngine::InitializeMemoryUpgrades() {
//	if (this->HasSupport() && !this->IsHek()) {
//		if (this->IsCustomEd() && this->CurrentMinor == feature_management::engines::minor::halo_1_10) {
//			//CE110::InitializeMemoryUpgrades();
//		}
//
//	}
//}

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
	if (!core_initialized || !scenario_globals) {
		return static_cast<Yelo::Scenario::s_scenario_globals *>(nullptr);
	}

	return scenario_globals;
}

void GlobalEngine::InitializeLuaState() {
	PrintLn("Attempting to initialize LuaScript manager");
	LuaState->InitializeLua(LUA_FILENAME);
}

LuaScriptManager *GlobalEngine::GetLuaState() {
	if (!LuaState->IsLoaded()) {
		InitializeLuaState();
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
	}
	return NULL;
}

IDirectInputDevice8A **GlobalEngine::GetJoystickInputs() {
	if (this->IsCustomEd()) {
		return CE110::GetJoystickInputs();
	}
	return NULL;
}

void GlobalEngine::WriteHooks() {
	PrintLn("\nInitializing to prepare to write the game hooks now!");
	switch (this->CurrentMajor) {
		case major::CE:
			PrintLn("Custom Edition detected");
			switch (this->CurrentMinor) {
				case minor::halo_1_10:
					PrintLn("CE_110 detected!");
					CE110::WriteHooks();
				default:
					return;
			}
		case major::HEK:
			PrintLn("Sapien detected");
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
	if (this->current_map != nullptr) {
		PrintLn("Setting up GlobalEngine despite already having current map");
	}

	auto filename = GetCurrentFileName();

	printf("Filename: %s\n", filename.c_str());

	auto fName = ::std::string(filename.c_str());

	if (fName == "sapien.exe") {
		printf("Detected sapien!\n");
		this->CurrentMajor   = major::HEK;
		this->CurrentMinor   = minor::sapien;
		this->DEBUG_FILENAME = const_cast<char *>(Sapien::DEBUG_FILENAME);
		this->current_map    = const_cast<defined_functionrange *>(Sapien::GetFunctionMap());
	}

	if (fName == "haloce.exe") {
		printf("Detected haloce!\n");
		this->CurrentMajor   = major::CE;
		this->CurrentMinor   = minor::halo_1_10;
		this->DEBUG_FILENAME = const_cast<char*>(CE110::DEBUG_FILENAME);
		this->current_map    = const_cast<defined_functionrange *>(CE110::GetFunctionMap());
	}

	if (this->HasSupport()) {
		CurrentSupported = this->GetSupported();
	}
}

size_t GlobalEngine::GetNumberOfFunctionTableReferences() {
	if (this->IsCustomEd()) {
		return 354u;
	}

	return 0;
}

void GlobalEngine::SetCoreAddressList(LPCoreAddressList add_list) {
	if (core_initialized) {
		return;
	}

	core_0 = reinterpret_cast<_core_0 *>(add_list.core_0);
	core_1 = reinterpret_cast<_core_1 *>(add_list.core_1);
	core_2 = reinterpret_cast<_core_2 *>(add_list.core_2);
	core_3 = reinterpret_cast<_core_3 *>(add_list.core_3);
	core_4 = reinterpret_cast<_core_4 *>(add_list.core_4);
	core_5 = reinterpret_cast<_core_5 *>(add_list.core_5);
	core_6 = reinterpret_cast<_core_6 *>(add_list.core_6);
	core_7 = reinterpret_cast<_core_7 *>(add_list.core_7);

	main_globals_game_connection_type = reinterpret_cast<ushort *>(add_list.game_globals_conn_type);

	game_state_globals_location_ptr = reinterpret_cast<uintptr>(add_list.game_state_globals_location_ptr);
	at_main_menu                    = reinterpret_cast<bool *>(add_list.at_main_menu);
	player_control_globals_data     = *reinterpret_cast<s_player_control_globals_data **>(add_list.player_control_globals_data); // = player_control_globals *
	game_time_globals               = *reinterpret_cast<Yelo::GameState::s_game_time_globals **>(add_list.game_time_globals);

	core_initialized = true;
}

void GlobalEngine::RefreshCore(bool force) {
	if (core_initialized && !force) {
		return;
	}

	if (!this->HasSupport()) {
		throw "WTF? this platform has no support. Bye.";
	}

	if (this->IsSapien()) {
		PrintLn("IsSapien SetCoreAddressList");

		SetCoreAddressList(Sapien::GetCoreAddressList());
	}

	PrintLn("GetCoreAddressList");
	if (this->IsCustomEd()) {
		PrintLn("CustomEd SetCoreAddressList");
		SetCoreAddressList(CE110::GetCoreAddressList());
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

void GlobalEngine::registerLuaCallback(const ::std::string &cb_name, LuaCallbackId cb_type) {
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

template <typename T>
void ClampIndex(T &idx) {
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

short GlobalEngine::GetElapsedTime() {
	return game_time_globals->elapsed_time;
}

datum_index *GlobalEngine::GetLocalPlayers() {
	return players_globals->local_player_players;
}

short GlobalEngine::GetLocalPlayerCount() {
	return players_globals->local_player_count;

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

	dfr *funcList = current_map;

	for (dfr *item = funcList; item; item++) {
		if (equal(needle, item->funcName)) {
			return item->begin;
		}
	}

	return (uintptr_t) -1;
}

const char *GlobalEngine::getMemoryRegionDescriptor(const void * addr) {

	PrintLn("\tSearching for address: [0x%X]", addr);

	unsigned int endImgRng = NtHeader.baseImageLocation + NtHeader.imageSize;

	if ((uintptr_t)addr >= NtHeader.baseImageLocation && (uintptr_t)addr <= (uintptr_t)endImgRng) {
		PrintLn("\tException Location inside of tempera dinput dll, probably.");
		return "probably_in_tempera_dll_space";
	}

	if ((uintptr_t)addr < 0x200000) {
		PrintLn("\tAddress on the stack");
		return "probably_in_stack";
	}

	if ((uintptr_t)addr < 0x400000) {
		PrintLn("\tAn unmapped region");
		return "unmapped_region";
	}

	using dfr = defined_functionrange;

	dfr *funcList = current_map;

	int i = 0;

	for (dfr *item = funcList; item; item++) {
		++i;

		if (item->end < (uintptr_t)addr)
			continue;

		bool contains = item->contains((uintptr_t)addr);
		//The function map should be pre-sorted, so this SHOULD mean that we can
		// break out of the loop.
		if (item->begin <= (uintptr_t)addr) {
			if (!contains) {
				PrintLn("\tContains disagrees with the manual calculation");
			}

			return item->funcName;
		}
	}

	if ((uintptr_t)addr < 0x5E9020) {
		PrintLn("\tUnmapped function space, probably");
		return "function_space_unmapped";
	}

	PrintLn("\tUnmappedRegion");
	return "unmapped_region";
}

//Calls every registered lua function by that event.
#include "enums/generic_enums.h"

#define PRINTED(str) #str
#define CALL_LUA_BY_EVENT(event) state->call_lua_event_by_type(LuaCallbackId::PRINTED(event))

void main_setup_connection_init() {
	//If alreadyChecked is true, we shouldn't have to update 'funcFound' ever again.
	static bool alreadyChecked = false;
	static ::std::optional<uintptr_t> funcFound = FUNC_GET(main_setup_connection);

	 PrintLn("main_setup_connection_init hook called");

	if (!alreadyChecked) {
		PrintLn("Getting main setup connection");

		if (funcFound)
			PrintLn("Main setup connection found");

		alreadyChecked = true;
	}

	if (funcFound) {
		calls::DoCall<Convention::m_cdecl>(*funcFound);
	}

	static LuaScriptManager *state = 0;

	if (state == 0) {
		state = CurrentEngine.GetLuaState();
	}

	CurrentEngine.RefreshCore();
	state->call_lua_event_by_type(LuaCallbackId::post_initialize);
	// CALL_LUA_BY_EVENT(post_initialize);
}

void game_tick(int current_frame_tick) {
	static LuaScriptManager *state = 0;

	if (state == 0) {
		state = CurrentEngine.GetLuaState();
	}

	state->call_lua_event_by_type(LuaCallbackId::before_game_tick);

	state->lua_on_tick(current_frame_tick, CurrentEngine.GetElapsedTime());
	static ::std::optional<uintptr_t> funcFound = FUNC_GET(game_tick);

	if (funcFound) {
		// PrintLn("Game_tick call");
		calls::DoCall<Convention::m_cdecl, void, int>(*funcFound, current_frame_tick);
		// PrintLn("Post-Game_tick call");
	}

	// PrintLn("Lua_post_tick call");
	state->call_lua_event_by_type(LuaCallbackId::after_game_tick);
	// PrintLn("Post-Lua-post-tick call");
}

#undef CALL_LUA_BY_EVENT
#undef PRINTED

typedef std::string string;

typedef void (*pConsoleCMD)(char *command);

pConsoleCMD oConsoleCMD;

typedef void (*pConsoleText)(const char *pString, const HaloColor *fColor);

pConsoleText oConsoleText;


////////////////////////////////////////
// Core Methods
////////////////////////////////////////

////////////////////////////////////////
// Player Methods
namespace feature_management::engines {

	s_player_control *GlobalEngine::GetPlayerControl(unsigned short idx) {
		if (idx < MAX_PLAYER_COUNT_LOCAL) {
			return &player_control_globals_data->local_players[idx];
		}

		return nullptr;
	}

	// Returns a player structure address, by player index
	player *GlobalEngine::GetPlayer(short index) {
		return reinterpret_cast<player *>((unsigned long) core_0->Players->first + (index * core_0->Players->size));
	}

	// Check to see if a player is spawned && biped object is valid?
	bool GlobalEngine::IsPlayerSpawned(short index) {
		player *newplayer = GetPlayer(index);
		return (newplayer->SlaveUnitIndex.index != 0 && newplayer->SlaveUnitIndex.index != INVALID);
	}

	// Check to see if a player is valid
	bool GlobalEngine::IsPlayerValid(short index) {
		player *newplayer = GetPlayer(index);
		return (newplayer->playerid != 0 && newplayer->playerid != INVALID);
	}

	// Returns a player's name by player index
	wchar_t *GlobalEngine::GetPlayerName(short player_index) {
		player *newplayer = GetPlayer(player_index);
		if (!newplayer)
			return NULL;
		else
			return newplayer->PlayerName0;
	}

	// Returns a player team by player index
	long GlobalEngine::GetPlayerTeam(short player_index) {
		player *newplayer = GetPlayer(player_index);
		return newplayer->Team;
	}

	void GlobalEngine::TryLogPlayer(short index, bool toConsole) {
		if (!IsPlayerSpawned(index)) {
			return;
		}
		Print<true>("Tried to log the player, but obv, that failed, lol");
		//GetPlayer(index)->DumpData(toConsole);
	}

	// Returns a player object ident by player index
	datum_index GlobalEngine::GetPlayerObjectIdent(short player_index) {
		player *newplayer = GetPlayer(player_index);
		return newplayer->SlaveUnitIndex;
	}
	////////////////////////////////////////
	// Object Methods

	/**
	 * @param player_index 0-15 index of player to try to get.
	 * @returns the address of a players biped object structure
	 */
	// biped_data *GlobalEngine::GetBiped(short player_index) {
	// 	short object_index = GetPlayerObjectIdent(player_index).index;
	//
	// 	if (object_index == INVALID)
	// 		return NULL;
	//
	// 	return (biped_data *) GetObjectHeader(object_index)->address;
	// }

	// Returns an object_header structure by object index
	object_header *GlobalEngine::GetObjectHeader(short object_index) {
		object_header *objectheader = (object_header *) ((unsigned long) core_1->Object->first + (object_index * core_1->Object->size));
		return objectheader;
	}

	// Returns a generic object_data structure by object index
	object_data *GlobalEngine::GetGenericObject(short object_index) {
		return GetObjectHeader(object_index)->address;
	}

	// Returns the x,y,z coordinates of an object by object index
	vect3 &GlobalEngine::GetObjectCoord(short object_index) {
		return GetGenericObject(object_index)->World;
	}

	// Returns an object name by object index
	const char *GlobalEngine::GetObjectName(short object_index) {
		return GetObjectName(GetGenericObject(object_index));
	}

	//Returns an object name by object structure
	const char *GlobalEngine::GetObjectName(object_data *obj) {
		short  metaind = obj->Meta.index;
		char   *name   = TagIndexHeader->FirstTag[metaind].TName->Name;
		string str     = name;

		return name + str.rfind("\\") + 1;
	}

	////////////////////////////////////////
	// Map Methods
	// Returns the loaded map name
	char *GlobalEngine::GetMapName() {
		return MapHeader->MapName;
	}

	const bool GlobalEngine::AreWeInMainMenu() {
		if (!HasSupport()) {

			PrintLn("No Support for Current Engine");

			throw "Main menu check called invalid fashion";
		}

		if (!IsCoreInitialized()) {

			PrintLn("Core is not initialized for CurrentEngine");

			throw "AreWeInMainMenu called way too early in the cycle.";
		}

		if (IsSapien()) {
			PrintLn("Can't be in main menu if we're in sapien");
			return false;
		}

		if (!this->at_main_menu) {
			PrintLn("Not in main menu, so we can force-RefreshCore");

			//Calling refreshCore here is problematic as it semi-mildly relies on side effects in order to
			//get the core refreshed
			this->RefreshCore();

			if (!this->at_main_menu) {
				PrintLn("RefreshCore- What the fuck are we even doing here");

				throw "This is complete bullshit. We cannot continue. RefreshCore check fails after refresh.";
			}
		}

		return this->at_main_menu;
	}

	////////////////////////////////////////
	// Console Methods
	// Toggles console on / off
	void GlobalEngine::ToggleConsole(bool bSwitch) {
		// DWORD dwOldProtect = NULL;
		if (bSwitch) {

		} else {

		}
	}

	short GlobalEngine::GetMaxObjects() { return this->core_1->Object->max; }

	// Toggles devmode on / off
	void GlobalEngine::ToggleDevmode(bool bSwitch) {
		ConsoleText(hRed, "Dev-mode toggling isn't enabled.");
		//	DWORD dwOldProtect = NULL;
		//	BYTE bEnable[1] = {0xEB};
		//	BYTE bDisable[1] = {0x74};
		//	VirtualProtect((void *) DEVMODE_HOOK_ADDRESS, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		//
		//	if (bSwitch)
		//		memcpy((void *) DEVMODE_HOOK_ADDRESS, (void *) bEnable, 1);
		//	else
		//		memcpy((void *) DEVMODE_HOOK_ADDRESS, (void *) bDisable, 1);
		//
		//	VirtualProtect((void *) DEVMODE_HOOK_ADDRESS, 1, dwOldProtect, &dwOldProtect);
	}

	// Toggles devmode flycam on / off
	void GlobalEngine::ToggleFlycam(char onoff) {
		static bool on = true; // first run means turn on.

		// if -1, toggle. Else, follow what was given.
		if (onoff == -1)
			on = !on;
		else
			on = (bool) onoff;

		if (on) {
			ConsoleCMD((char *) "debug_camera_load");
			//ConsoleCMD("camera_control 1"); // not necessary
		} else {
			ConsoleCMD((char *) "debug_camera_save");
			ConsoleCMD((char *) "camera_control 0");
		}
	}

	// Hooked console function
	void hkConsoleCMD(char *command) {
		DEBUG("Executing console command: %s", command);
#ifdef __GNUC__
		IMPLEMENTATION_REQUIRED
#elif _MSC_VER
		__asm {
		PUSH 0
		MOV EDI, command
		CALL DWORD PTR DS:[oConsoleCMD]
		ADD ESP, 0x4
		}
#endif
	}

	// Calls the requested console command
	void GlobalEngine::ConsoleCMD(char *command) {
		DWORD dwOldProtect    = (DWORD) NULL;
		BYTE  bConsoleOrig[8] = {0x8A, 0x07, 0x81, 0xEC, 0x00, 0x05, 0x00, 0x00};

		VirtualProtect((void *) CONSOLE_HOOK_ADDRESS, 8, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		// oConsoleCMD = (pConsoleCMD) DetourFunction((PBYTE) CONSOLE_HOOK_ADDRESS, (PBYTE) hkConsoleCMD);

		ToggleDevmode(1);

		hkConsoleCMD(command);

		//ToggleDevmode( 0 );

		memcpy((void *) CONSOLE_HOOK_ADDRESS, (void *) bConsoleOrig, 8);
		VirtualProtect((void *) CONSOLE_HOOK_ADDRESS, 8, dwOldProtect, &dwOldProtect);
	}

	// Hooked console output function
	void hkConsoleText(const char *cFmt, HaloColor *fColor) {
#ifdef __GNUC__
		IMPLEMENTATION_REQUIRED
#elif _MSC_VER
		__asm {
		MOV EAX, fColor
		PUSH cFmt
		CALL DWORD PTR DS:[oConsoleText]
		ADD ESP, 04h
		}
#endif
	}

	// Outputs console text, with custom colors and formatting
	void GlobalEngine::ConsoleText(HaloColor fColor, const char *cFmt, ...) {
		va_list mvalist;
		char    cBuffer[256] = {0};

		Print("Atttempting to print to Console!");
		DWORD dwOldProtect = 0;
		BYTE  bOrig[6]     = {0x83, 0xEC, 0x10, 0x57, 0x8B, 0xF8};

		VirtualProtect((void *) CONSOLE_TEXT_HOOK_ADDRESS, 10, PAGE_EXECUTE_READWRITE, &dwOldProtect);

			va_start(mvalist, cFmt);
		_vsnprintf(cBuffer, sizeof(cBuffer), cFmt, mvalist);
		//DEBUG(cFmt, mvalist);
			va_end(mvalist);

		// EZ PZ Detours.h replacement, yea?
		// oConsoleText = (pConsoleText) DetourFunction((PBYTE) CONSOLE_TEXT_HOOK_ADDRESS, (PBYTE) &hkConsoleText);
		void *color = &fColor;
#ifdef __GNUC__
		IMPLEMENTATION_REQUIRED
#elif _MSC_VER
		__asm MOV EAX, color
#endif
		calls::DoCall<Convention::m_cdecl, void, char *>(CONSOLE_TEXT_HOOK_ADDRESS, cBuffer);
		// #ifdef __GNUC__
		// #elif _MSC_VER
		// 	__asm ADD ESP, 04h
		// #endif


		memcpy((void *) CONSOLE_TEXT_HOOK_ADDRESS, (void *) bOrig, 6);
		VirtualProtect((void *) CONSOLE_TEXT_HOOK_ADDRESS, 10, dwOldProtect, &dwOldProtect);
	}
};
