#pragma once


#include <dinput.h>
#include "versions.h"
#include "gamestate/objects.h"
#include "core.h"
#include "lua/script_manager.h"

class LuaScriptManager;

static bool ShouldOverride[MAX_PLAYER_COUNT_LOCAL];
struct s_player_action;
class Core;

namespace feature_management::engines {
	class GlobalEngine {
		features              CurrentSupported = features::NOPE;
		major                 CurrentMajor     = major::NO;
		minor                 CurrentMinor     = minor::nope;
		defined_functionrange *current_map;
		//Support Attempted
		// std::string GetCurrentFileName(char * args) {
		std::string GetCurrentFileName();

		bool VerSupported();

		features GetSupported();

		LuaScriptManager * LuaState = &mgr;

	public:

		char                     *DEBUG_FILENAME = const_cast<char *>("tempera.unk.unk.debug.log");
		char *LUA_FILENAME   = const_cast<char *>("tempera.init.lua");

		LuaScriptManager * GetLuaState();

		s_player_action & GetPlayerActionOverride(ushort idx);

		bool IsHek();

		bool IsSapien();

		bool IsCustomEd();

		bool IsCoreInitialized();

		void WriteHooks();

		bool HasSupport();

		IDirectInput8A *GetDInput8Device();
		IDirectInputDevice8A *GetKeyboardInput();
		IDirectInputDevice8A *GetMouseInput();
		IDirectInputDevice8A **GetJoystickInputs();


		bool ShouldOverrideAction(ushort idx = 0);
		void ResetOverride(ushort idx = 0);

		bool SupportsFeature(features feat);

		bool SupportsFeature(uint feat);

		void MakePlayerJump(ushort idx);

		void SetPlayerPrimaryTriggerFlag(ushort idx);
		void SetPlayerTriggerPressure(float t, ushort idx);

		void SetPlayerLookX(float x, ushort idx);
		void SetPlayerLookY(float y, ushort idx);

		void SetPlayerXVelocity(float x, ushort idx);
		void SetPlayerYVelocity(float y, ushort idx);

		void InitializeLuaState();

		void LuaFirstRun();

		GlobalEngine();

		void RefreshCore();

		minor GetMinorVersion() {
			return this->CurrentMinor;
		}

		const char *GetCurrentMajorVerString();

		Core *GetCore();

		/**
 		* Called before VirtualProtect is run.
 		*/
		void registerLuaCallback(const std::string &cb_name, LuaCallbackId cb_type);

		constexpr bool equal(const char *lhs, const char *rhs);

		/**
		 * Runtime-time lookup of function address beginnings.
		 * 	While positive results are (often) correct, take guesses
		 * with a huge grain of salt.
		 * @param needle function name to look up
		 * @return uintptr of the entry point of function.
		 */
		uintptr_t getFunctionBegin(const char *needle);

		/**
		 * Run-time lookup of memory regions.
		 * 	While positive results are (often) correct, take guesses
		 * with a huge grain of salt.
		 * @param addr Address to look up
		 * @return named description of the memory region.
		 */
		const char *getMemoryRegionDescriptor(const uintptr_t addr);
	};
};

static feature_management::engines::GlobalEngine CurrentEngine;

void post_dll_load();

/**
 * Called variably based on fps
 * @param current_frame_tick - ticks remaining before rendering the next frame
 */
void game_tick(int current_frame_tick);

/**
 * Called right before game loop starts, memory has already been initialized
 */
void main_setup_connection_init();
