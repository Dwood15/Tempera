#pragma once
#pragma check_stack(off)

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"
#include <InitGuid.h> //InitGuid allows us to avoid importing Windows.h
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#undef DIRECTINPUT_VERSION
#include <optional>
#include <addlog.h>
#include <engine_interface.h>
//#pragma clang diagnostic pop

#include "versions.h"

#include "core_types.h"
#include "tags/map.h"
#include "tags/tags.h"
#include "gamestate/objects.h"
#include "gamestate/player_types.h"
#include "game/time/time_structures.hpp"
#include "gamestate/camera.h"
#include "gamestate/objects/objectcontroller.h"
#include "Direct3D/d3d9hook.h"

struct s_player_action;
struct s_unit_control_data;
constexpr const char *  K_GAME_GLOBALS_TAG_NAME = "globals\\globals";

extern feature_management::engines::IEngine* CurrentEngine;

namespace feature_management::engines {
	class RuntimeManager {
	public:
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
		static bool  *at_main_menu;

		void **hud_scripted_globals;
		void **hud_messaging_state;

		s_player_control_globals_data *player_control_globals_data;
		s_players_globals_data        *players_globals;
		Yelo::Scenario::s_scenario_globals * scenario_globals;
		s_motion_sensor               *motion_sensor;
		Yelo::GameState::s_game_time_globals	  *game_time_globals = nullptr;
		uintptr						game_state_cpu_allocation_size;
		uintptr						game_state_globals_ptr;
		uintptr						game_state_globals_crc;

		Camera::s_cinematic_globals_data *cinematic_globals;
		void                          **crc_checksum_buffer;

		////////////////////////////////////////
		// Defines
		////////////////////////////////////////
		uintptr_t CONSOLE_HOOK_ADDRESS; //CE + 1
		uintptr_t DEVMODE_HOOK_ADDRESS;
		uintptr_t CONSOLE_TEXT_HOOK_ADDRESS; //CE + 1

	public:
		ushort    *main_globals_game_connection_type;
		_camera   *camera;
		CMyCamera *MyCamera;
		ObjectController *ObjectControl;

		short GetMaxObjects();

		s_player_control * GetPlayerControl(unsigned short idx);

		void SetSpawnCount(short newSpawnNum);
		player *GetPlayer(short index);

		static bool AreWeInMainMenu();

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

		static inline char *LUA_FILENAME   = const_cast<char *>("tempera.init.lua");

		bool AreWeInCutScene() {
			return cinematic_globals->in_progress;
		}

		void WriteHooks();

		//Basically all GameStateMallocs are the same
		//But because C++ is stupid (when this is in CurrentEngine.cpp)
		template<typename T>
		void GameStateMalloc(void **targetGlobal) {
			uint game_state_cpu_allocation_size = 0;
			uint game_state_globals_ptr = 0;
			uint game_state_globals_crc = 0;

			if (CurrentEngine != nullptr) {
				CurrentEngine->GetMallocAddresses(game_state_cpu_allocation_size, game_state_globals_ptr, game_state_globals_crc);
			}

			if (game_state_cpu_allocation_size == NULL) {
				PrintLn("game_state_cpu_allocation_size is nullptr. prolly wanna fix that.");
			}

			auto oldSize = (* (uintptr)game_state_cpu_allocation_size);
			PrintLn("Old Value: 0x%x", oldSize);

			auto newLocation = oldSize + (uintptr_t)game_state_globals_ptr;
			auto currentSizeof = sizeof(T);

			(* (uintptr *)game_state_cpu_allocation_size) += currentSizeof;
			(*targetGlobal) = (void*)newLocation;
			//Call the game engine's malloc crc checksum buffer. Hoping for the best, lmao
			static ::std::optional<uintptr_t> funcFound = getFunctionBegin("malloc_crc_checksum_buffer");
			if ((uint)game_state_globals_crc != (uint)0x67dd94) {
				PrintLn("Game State Globals CRC does not look like the expected address");
			}

			if (funcFound)
				calls::DoCall<Convention::m_cdecl, void, int, int, uint>(*funcFound, (int)game_state_globals_crc, (int)&currentSizeof, (uint)0x4u);
		}

		template<typename T>
		static void ClampIndex(T &idx);

		static bool HasSupport();

		auto ScenarioGlobals();

		static IDirectInput8A *GetDInput8Device();
		IDirectInputDevice8A *GetKeyboardInput();
		IDirectInputDevice8A *GetMouseInput();
		IDirectInputDevice8A **GetJoystickInputs();

		bool SupportsFeature(features feat);

		bool SupportsFeature(uint feat);

		void RefreshCore(bool force = false);

		constexpr bool equal(const char *lhs, const char *rhs);

		/**
		 * Runtime-time lookup of function address beginnings.
		 * 	While positive results are (often) correct, take guesses
		 * with a huge grain of salt.
		 * @param needle function name to look up
		 * @return uintptr of the entry point of function.
		 */
		::std::optional<uintptr_t> getFunctionBegin(const char *needle);

        short GetElapsedTime();
	};

	//Set up our stuff
	RuntimeManager *GetRuntimeManager();

	bool IsCoreInitialized();
}

//Extern's like this are why C++ has a bad rep
extern feature_management::engines::RuntimeManager* CurrentRuntime;

/**
 * Called variably based on fps
 * @param current_frame_tick - ticks remaining before rendering the next frame
 */
void game_tick(int current_frame_tick);

/**
 * Called right before game loop starts, memory has already been initialized
 */
void main_setup_connection_init();


static const char * getMemoryRegionDescriptor(void * addr) {
	PrintLn("\tCurrentRuntime Location: : [0x%X]", CurrentRuntime);
	PrintLn("\tSearching for address: [0x%X]", addr);

	if ((uintptr_t)addr < 0x200000) {
		PrintLn("\tAddress on the stack");
		return "probably_in_stack";
	}

	if ((uintptr_t)addr < 0x400000) {
		PrintLn("\tAn unmapped region");
		return "unmapped_region";
	}

	if ((uintptr_t)addr >=  0x70000000) {
		PrintLn("\tProbably in a loaded DLL memory location");
		return "probably in a loaded DLL memory location";
	}

	using dfr = defined_functionrange;

	static dfr *funcList = nullptr;

	if (!funcList) {
		funcList = CurrentEngine->GetFunctionMap();
	}

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

static CD3D cd3d;
