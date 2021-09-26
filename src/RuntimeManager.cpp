#include "RuntimeManager.h"
#include "exceptions/exception_handler.h"
#include <ce/1_10/110EngineManager.h>
#include <hek/sapien/sapienEngineManager.h>
#include <addlog.h>

#pragma check_stack(off)

//Putting this under a namespace breaks for some reason
feature_management::engines::RuntimeManager *CurrentRuntime = nullptr;
feature_management::engines::IEngine *CurrentEngine = nullptr;

namespace feature_management::engines {
	bool  *RuntimeManager::at_main_menu = nullptr;

	void RuntimeManager::SetSpawnCount(short newSpawnNum) {
		if (spawn_count == nullptr) {
			PrintLn("Spawn Count is nullptr but we're still setting it. Game will die.");
		}
		*spawn_count = newSpawnNum;
	}

	constexpr bool RuntimeManager::equal(const char *lhs, const char *rhs) {
		while (*lhs || *rhs) {
			if (*lhs++ != *rhs++) {
				return false;
			}
		}
		return true;
	}

	auto RuntimeManager::ScenarioGlobals() {
		if (!core_initialized || !scenario_globals) {
			return static_cast<Yelo::Scenario::s_scenario_globals *>(nullptr);
		}

		return scenario_globals;
	}

	IDirectInput8A *RuntimeManager::GetDInput8Device() {
		if (CurrentEngine->IsCustomEd()) {
			return CE110::GetDInput8Device();
		}
		return nullptr;
	}

	IDirectInputDevice8A *RuntimeManager::GetKeyboardInput() {
		if (CurrentEngine->IsCustomEd()) {
			return CE110::GetKeyboardInput();
		}
		return nullptr;
	}

	IDirectInputDevice8A *RuntimeManager::GetMouseInput() {
		if (CurrentEngine->IsCustomEd()) {
			return CE110::GetMouseInput();
		}
		return nullptr;
	}

	IDirectInputDevice8A **RuntimeManager::GetJoystickInputs() {
		if (CurrentEngine->IsCustomEd()) {
			return CE110::GetJoystickInputs();
		}
		return nullptr;
	}

	void RuntimeManager::WriteHooks() {
		PrintLn("\nInitializing to prepare to write the game hooks now!");
		CurrentEngine->WriteHooks();
	}

	std::string GetCurrentFileName() {
		static char name[MAX_PATH];
		static ::std::string shortName;
		static bool nameGot = false;

		if (nameGot) {
			return shortName;
		}
		//Tried to pluck out version information from the file metadata
		{
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
		}
		GetModuleFileNameA(nullptr, name, ::std::size(name));
		char *lpCmdline = GetCommandLineA();
		PrintLn("CommandLine Args: %s", lpCmdline);
		PrintLn("File Name: %s", name);

		int num = MAX_PATH - 1;
		while (name[num] != 'e') {
			num--;
		}
		int till = num;
		while (name[till] != '\\') {
			till--;
		}

		shortName = std::string(name).substr(till + 1, num - (till)).c_str();
		nameGot = true;

		return shortName;
	}

	RuntimeManager *GetRuntimeManager() {
		if (CurrentRuntime == nullptr) {
			CurrentRuntime = new RuntimeManager();
		}

		auto filename = GetCurrentFileName();

		PrintLn("RuntimeManager Filename: %s\n", filename.c_str());

		auto fName = ::std::string(filename);

		if (CurrentEngine == nullptr) {
			CurrentEngine = new IEngine();
			if (fName == "sapien.exe") {
				PrintLn("Detected sapien!");
				CurrentEngine = new Sapien();
			}

			if (fName == "haloce.exe") {
				PrintLn("Detected haloce! Assuming version 110.");
				CurrentEngine = new CE110();
			}

			if (!CurrentEngine->HasSupport()) {
				PrintLn("Current running engine Has No support!");
			}
		}
		return CurrentRuntime;
	}

	short RuntimeManager::GetElapsedTime() {
		if (game_time_globals == nullptr) {
			RefreshCore(true);
		}


		return game_time_globals->elapsed_time;
	}

	void RuntimeManager::RefreshCore(bool force) {
		if (core_initialized && !force) {
			return;
		}

		if (!HasSupport()) {
			throw "WTF? this platform has no support. Bye.";
		}

		PrintLn("GetCoreAddressList");

		auto add_list = CurrentEngine->GetCoreAddressList();

		PrintLn("CoreAddressLists loaded, moving them into ");

		core_0 = reinterpret_cast<_core_0 *>(add_list.core_0);
		core_1 = reinterpret_cast<_core_1 *>(add_list.core_1);
		core_2 = reinterpret_cast<_core_2 *>(add_list.core_2);
		core_3 = reinterpret_cast<_core_3 *>(add_list.core_3);
		core_4 = reinterpret_cast<_core_4 *>(add_list.core_4);
		core_5 = reinterpret_cast<_core_5 *>(add_list.core_5);
		core_6 = reinterpret_cast<_core_6 *>(add_list.core_6);
		core_7 = reinterpret_cast<_core_7 *>(add_list.core_7);

		main_globals_game_connection_type = reinterpret_cast<ushort *>(add_list.game_globals_conn_type);

		at_main_menu = reinterpret_cast<bool *>(add_list.at_main_menu);

		//Some attempts to initialize come out with 0x0 player control globals data.

		auto pglobalData = (s_players_globals_data **)add_list.players_global_data;

		players_globals = *pglobalData;
		player_control_globals_data = *reinterpret_cast<s_player_control_globals_data **>(add_list.player_control_globals_data); // = player_control_globals *

		game_time_globals = *reinterpret_cast<Yelo::GameState::s_game_time_globals **>(add_list.game_time_globals);

		cinematic_globals = *reinterpret_cast<Camera::s_cinematic_globals_data **>(add_list.cinematic_globals);

		CONSOLE_HOOK_ADDRESS = add_list.CONSOLE_HOOK_ADDRESS;
		CONSOLE_TEXT_HOOK_ADDRESS = add_list.CONSOLE_TEXT_HOOK_ADDRESS;

		core_initialized = true;
	}

	bool RuntimeManager::SupportsFeature(features feat) {
		return CurrentEngine->SupportsFeature(feat);
	}

	bool RuntimeManager::SupportsFeature(uint feat) {
		return CurrentEngine->SupportsFeature(feat);
	}

	template<typename T>
	void RuntimeManager::ClampIndex(T &idx) {
		if (idx > MAX_PLAYER_COUNT_LOCAL) {
			Print("Forced player to 0 b/c it %d > %d", idx, MAX_PLAYER_COUNT_LOCAL);
			idx = 0;
		}
	}

	bool RuntimeManager::HasSupport() {
		return (CurrentEngine) && CurrentEngine->HasSupport();
	}

	datum_index *RuntimeManager::GetLocalPlayers() {
		return players_globals->local_player_players;
	}

	short RuntimeManager::GetLocalPlayerCount() {
		return players_globals->local_player_count;
	}

	std::optional<uintptr_t> RuntimeManager::getFunctionBegin(const char *needle) {
		using dfr = defined_functionrange;

		dfr *funcList = CurrentEngine->GetFunctionMap();

		for (dfr *item = funcList; item; item++) {
			if (equal(needle, item->funcName)) {
				return item->begin;
			}
		}

		return (uintptr_t) -1;
	}

//Calls every registered lua function by that event.
	typedef void (*pConsoleCMD)(char *command);

	pConsoleCMD oConsoleCMD;

	typedef void (*pConsoleText)(const char *pString, const HaloColor *fColor);

	pConsoleText oConsoleText;

////////////////////////////////////////
// Core Methods
////////////////////////////////////////

////////////////////////////////////////
// Player Methods

	bool IsCoreInitialized() {
		return CurrentRuntime != nullptr && CurrentRuntime->core_initialized;
	}

	s_player_control *RuntimeManager::GetPlayerControl(unsigned short idx) {
		if (idx < MAX_PLAYER_COUNT_LOCAL) {
			return &player_control_globals_data->local_players[idx];
		}

		return nullptr;
	}

	// Returns a player structure address, by player index
	player *RuntimeManager::GetPlayer(short index) {
		return reinterpret_cast<player *>((unsigned long) core_0->Players->first + (index * core_0->Players->size));
	}

	// Check to see if a player is spawned && biped object is valid?
	bool RuntimeManager::IsPlayerSpawned(short index) {
		player *newplayer = GetPlayer(index);
		return (newplayer->SlaveUnitIndex.index != 0 && newplayer->SlaveUnitIndex.index != INVALID);
	}

	// Check to see if a player is valid
	bool RuntimeManager::IsPlayerValid(short index) {
		player *newplayer = GetPlayer(index);
		return (newplayer->playerid != 0 && newplayer->playerid != INVALID);
	}

	// Returns a player's name by player index
	wchar_t *RuntimeManager::GetPlayerName(short player_index) {
		player *newplayer = GetPlayer(player_index);
		if (!newplayer)
			return nullptr;
		else
			return newplayer->PlayerName0;
	}

	// Returns a player team by player index
	long RuntimeManager::GetPlayerTeam(short player_index) {
		player *newplayer = GetPlayer(player_index);
		return newplayer->Team;
	}

	void RuntimeManager::TryLogPlayer(short index, bool toConsole) {
		if (!IsPlayerSpawned(index)) {
			return;
		}
		Print<true>("Tried to log the player, but obv, that failed, lol");
		//GetPlayer(index)->DumpData(toConsole);
	}

	// Returns a player object ident by player index
	datum_index RuntimeManager::GetPlayerObjectIdent(short player_index) {
		player *newplayer = GetPlayer(player_index);
		return newplayer->SlaveUnitIndex;
	}
	////////////////////////////////////////
	// Object Methods

	/**
	 * @param player_index 0-15 index of player to try to get.
	 * @returns the address of a players biped object structure
	 */
	// biped_data *RuntimeManager::GetBiped(short player_index) {
	// 	short object_index = GetPlayerObjectIdent(player_index).index;
	//
	// 	if (object_index == INVALID)
	// 		return NULL;
	//
	// 	return (biped_data *) GetObjectHeader(object_index)->address;
	// }

	// Returns an object_header structure by object index
	object_header *RuntimeManager::GetObjectHeader(short object_index) {
		auto offset = object_index * core_1->Object->size;
		auto hLocation = (unsigned long)core_1->Object->first + offset;
		return (object_header *)(hLocation) ;
	}

	// Returns a generic object_data structure by object index
	object_data *RuntimeManager::GetGenericObject(short object_index) {
		return GetObjectHeader(object_index)->address;
	}

	// Returns the x,y,z coordinates of an object by object index
	vect3 &RuntimeManager::GetObjectCoord(short object_index) {
		return GetGenericObject(object_index)->World;
	}

	// Returns an object name by object index
	const char *RuntimeManager::GetObjectName(short object_index) {
		return GetObjectName(GetGenericObject(object_index));
	}

	//Returns an object name by object structure
	const char *RuntimeManager::GetObjectName(object_data *obj) {
		short metaind = obj->Meta.index;
		char *name = TagIndexHeader->FirstTag[metaind].TName->Name;
		std::string str = name;

		return name + str.rfind('\\') + 1;
	}

	////////////////////////////////////////
	// Map Methods
	// Returns the loaded map name
	char *RuntimeManager::GetMapName() {
		return MapHeader->MapName;
	}

	bool RuntimeManager::AreWeInMainMenu() {
		if (!IsCoreInitialized()) {
			PrintLn("Core not initialized - defaulting to true for MainMenu check");
			return true;
		}

		if (CurrentEngine->IsHek()) {
			//Can't be in main menu if we're in HEK
			return false;
		}

		return *at_main_menu;
	}

	////////////////////////////////////////
	// Console Methods
	// Toggles console on / off
	void RuntimeManager::ToggleConsole(bool bSwitch) {
		// DWORD dwOldProtect = NULL;
		if (bSwitch) {

		} else {

		}
	}

	short RuntimeManager::GetMaxObjects() { return this->core_1->Object->max; }

	// Toggles devmode on / off
	void RuntimeManager::ToggleDevmode(bool bSwitch) {
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
	void RuntimeManager::ToggleFlycam(char onoff) {
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
	void RuntimeManager::ConsoleCMD(char *command) {
		DWORD dwOldProtect = (DWORD) NULL;
		BYTE bConsoleOrig[8] = {0x8A, 0x07, 0x81, 0xEC, 0x00, 0x05, 0x00, 0x00};

		VirtualProtect((void *) CONSOLE_HOOK_ADDRESS, 8, PAGE_EXECUTE_READWRITE, &dwOldProtect);

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
	void RuntimeManager::ConsoleText(HaloColor fColor, const char *cFmt, ...) {
		if (CONSOLE_TEXT_HOOK_ADDRESS == 0x0) {
			RefreshCore(true);
		}

		va_list mvalist;
		char cBuffer[256] = {0};

			va_start(mvalist, cFmt);
		_vsnprintf(cBuffer, sizeof(cBuffer), cFmt, mvalist);
			va_end(mvalist);

		//These variables are used in the _asm section.
		void *color = &fColor;
#ifdef __GNUC__
		using printf_tproc __attribute__((cdecl, regparm(1))) = void(*)(const void *color, const char* fmt, const char* arg); // pass fmt as "%s"
		const printf_tproc console_printf = reinterpret_cast<printf_tproc>(CONSOLE_TEXT_HOOK_ADDRESS);
		console_printf(color, "%s", cBuffer);
#elif _MSC_VER
		{
			//If you pass these variable directly into the the assembly, shit breaks.
			[[maybe_unused]]auto func = reinterpret_cast<const void *>(CONSOLE_TEXT_HOOK_ADDRESS);
			[[maybe_unused]]const char *fmt = "%s";
			[[maybe_unused]]const char *text = cBuffer;
			__asm {
			pushad
			pushfd

			mov eax, color
			mov ecx, func
			push text
			push fmt
			call ecx
			add esp, 8

			popfd
			popad
			};
		}
#endif
	}
};

#define PRINTED(str) #str
#define CALL_LUA_BY_EVENT(event) state->call_lua_event_by_type(LuaCallbackId::PRINTED(event))

	void game_tick(int current_frame_tick) {
		static ::std::optional<uintptr_t> funcFound = FUNC_GET(game_tick);

		if (funcFound) {
			calls::DoCall<Convention::m_cdecl, void, int>(*funcFound, current_frame_tick);
		}
	}

	void main_setup_connection_init() {
		static bool alreadyChecked = false;
		static ::std::optional<uintptr_t> funcFound = FUNC_GET(main_setup_connection);

		PrintLn("main_setup_connection_init hook called");

		if (!alreadyChecked) {
			{
				auto running = "Build Info: " __DATE__ ", " __TIME__;
				PrintLn(running);
				CurrentRuntime->ConsoleText(hGreen, "Tempera %s", running);
			}
			PrintLn("Getting main setup connection");

			if (funcFound)
				PrintLn("Main setup connection found");

			alreadyChecked = true;
		}

		if (funcFound) {
			calls::DoCall<Convention::m_cdecl>(*funcFound);
		}

		CurrentRuntime->RefreshCore();
	}

#undef CALL_LUA_BY_EVENT
#undef PRINTED
