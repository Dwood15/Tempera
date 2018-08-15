#pragma once

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"
#include <dinput.h>
#include <optional>
//#pragma clang diagnostic pop

#include "versions.h"
#include "lua/script_manager.h"
#include "core_types.h"
#include "tags/map.h"
#include "tags/tags.h"
#include "gamestate/objects.h"
#include "gamestate/player_types.h"
#include "game/time/time_structures.hpp"
#include "gamestate/camera.h"
#include "gamestate/objects/objectcontroller.h"
#include "Direct3D/d3d9hook.h"

class LuaScriptManager;

static bool ShouldOverride[MAX_PLAYER_COUNT_LOCAL];
struct s_player_action;
struct s_unit_control_data;
constexpr const char *  K_GAME_GLOBALS_TAG_NAME = "globals\\globals";

namespace feature_management::engines {
	class GlobalEngine {
		bool core_initialized = false;

	private:
		//TODO: More intelligent division of these members -
		// - Values dependent upon other values or actions should _not_ be writable from outside this class.
		_core_0 *core_0;
		_core_1 *core_1;
		_core_2 *core_2;
		_core_3 *core_3;
		_core_4 *core_4;
		_core_5 *core_5;
		_core_6 *core_6;
		_core_7 *core_7;

		map_header       *MapHeader;
		tag_index_header *TagIndexHeader;

		short *to_respawn_count;
		short *spawn_count;
		short *render_window_count;
		bool  *at_main_menu;

		void **hud_scripted_globals;
		void **hud_messaging_state;

		s_player_control_globals_data *player_control_globals_data;
		s_players_globals_data        *players_globals;
		Yelo::Scenario::s_scenario_globals * scenario_globals;
		s_motion_sensor               *motion_sensor;
		Yelo::GameState::s_game_time_globals	  *game_time_globals;
		uintptr                       game_state_globals_location_ptr;
		uintptr                       game_state_globals_ptr;
		void                          **crc_checksum_buffer;

		////////////////////////////////////////
		// Defines
		////////////////////////////////////////
		uintptr_t CONSOLE_HOOK_ADDRESS; //CE + 1
		uintptr_t DEVMODE_HOOK_ADDRESS;
		uintptr_t CONSOLE_TEXT_HOOK_ADDRESS; //CE + 1

	public:
		ushort                        *main_globals_game_connection_type;
		_camera   *camera;
		CMyCamera *MyCamera;
		ObjectController *ObjectControl;

		short GetMaxObjects();

		s_player_control * GetPlayerControl(unsigned short idx);

		player *GetPlayer(short index);

		bool AreWeInMainMenu();
		auto GetGameTimeGlobals();

		bool IsPlayerSpawned(short index);

		bool IsPlayerValid(short index);

		void TryLogPlayer(short index, bool toConsole = false);

		wchar_t *GetPlayerName(short player_index);

		datum_index * GetLocalPlayers();
		short GetLocalPlayerCount();


		long GetPlayerTeam(short player_index);

		datum_index GetPlayerObjectIdent(short player_index);

		// biped_data *GetBiped(short player_index);

		object_header *GetObjectHeader(short object_index);

		object_data *GetGenericObject(short object_index);

		vect3 &GetObjectCoord(short object_index);

		const char *GetObjectName(short object_index);

		const char *GetObjectName(object_data *obj);

		char *GetMapName();

		// Console Methods
		void ToggleConsole(bool bSwitch); // Toggles Console on / off
		void ToggleDevmode(bool bSwitch); // Toggles Devmode on / off
		void ConsoleCMD(char *command);   // Calls a console / devcommand
		void ConsoleText(HaloColor fColor, const char *cFmt, ...);
		void ToggleFlycam(char = -1);

	private:
		features              CurrentSupported = features::NOPE;
		major                 CurrentMajor     = major::NO;
		minor                 CurrentMinor     = minor::nope;
		defined_functionrange *current_map;
		//Support Attempted
		//::std::string GetCurrentFileName(char * args) {
		::std::string GetCurrentFileName();

		bool VerSupported();

		features GetSupported();

		LuaScriptManager * LuaState = &mgr;

	public:

		void ** GetHsFunctionTableReferences();
		char                     *DEBUG_FILENAME = const_cast<char *>("tempera.unk.unk.debug.log");
		char *LUA_FILENAME   = const_cast<char *>("tempera.init.lua");

		LuaScriptManager * GetLuaState();

		s_player_action GetPlayerActionOverride(ushort idx, s_unit_control_data * from);

		bool IsHek();

		bool IsSapien();

		bool IsCustomEd();

		bool IsCoreInitialized();

		size_t GetNumberOfFunctionTableReferences();

		void WriteHooks();

		bool HasSupport();

		auto ScenarioGlobals();

		IDirectInput8A *GetDInput8Device();
		IDirectInputDevice8A *GetKeyboardInput();
		IDirectInputDevice8A *GetMouseInput();
		IDirectInputDevice8A **GetJoystickInputs();

		void SetCoreAddressList(LPCoreAddressList add_list);
		bool ShouldOverrideAction(ushort idx = 0);
		void ResetOverride(ushort idx = 0);

		bool SupportsFeature(features feat);

		bool SupportsFeature(uint feat);

		void MakePlayerJump(ushort idx);

		void InitializeLuaState();

		void LuaFirstRun();

		GlobalEngine();

		void RefreshCore();

		auto GetHsFunctionTableCountReferences16();
		auto GetHsFunctionTableCountReferences32();

		void InitializeMemoryUpgrades();

		minor GetMinorVersion() {
			return this->CurrentMinor;
		}

		const char *GetCurrentMajorVerString();

		/**
 		* Called before VirtualProtect is run.
 		*/
		void registerLuaCallback(const ::std::string &cb_name, LuaCallbackId cb_type);

		constexpr bool equal(const char *lhs, const char *rhs);

		/**
		 * Runtime-time lookup of function address beginnings.
		 * 	While positive results are (often) correct, take guesses
		 * with a huge grain of salt.
		 * @param needle function name to look up
		 * @return uintptr of the entry point of function.
		 */
		::std::optional<uintptr_t> getFunctionBegin(const char *needle);

		/**
		 * Run-time lookup of memory regions.
		 * 	While positive results are (often) correct, take guesses
		 * with a huge grain of salt.
		 * @param addr Address to look up
		 * @return named description of the memory region.
		 */
		const char *getMemoryRegionDescriptor(const uintptr_t addr);

		short GetElapsedTime();
	};
};

static feature_management::engines::GlobalEngine CurrentEngine;

/**
 * Called variably based on fps
 * @param current_frame_tick - ticks remaining before rendering the next frame
 */
void game_tick(int current_frame_tick);

/**
 * Called right before game loop starts, memory has already been initialized
 */
void main_setup_connection_init();


static CD3D cd3d;
