#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <dinput.h>
#include <optional>
#pragma clang diagnostic pop

#include "versions.h"
#include "core.h"
#include "lua/script_manager.h"

class LuaScriptManager;

static bool ShouldOverride[MAX_PLAYER_COUNT_LOCAL];
struct s_player_action;
struct s_unit_control_data;

const char *  K_GAME_GLOBALS_TAG_NAME = "globals\\globals";

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

		void ** GetHsFunctionTableReferences();
		char                     *DEBUG_FILENAME = const_cast<char *>("tempera.unk.unk.debug.log");
		char *LUA_FILENAME   = const_cast<char *>("tempera.init.lua");

		LuaScriptManager * GetLuaState() volatile;

		s_player_action GetPlayerActionOverride(ushort idx, s_unit_control_data * from);

		bool IsHek();

		bool IsSapien();

		bool IsCustomEd();

		bool IsCoreInitialized() volatile ;

		size_t GetNumberOfFunctionTableReferences();

		void WriteHooks();

		bool HasSupport();

		auto ScenarioGlobals();

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

		void RefreshCore() volatile;

		auto GetHsFunctionTableCountReferences16();
		auto GetHsFunctionTableCountReferences32();

		minor GetMinorVersion() {
			return this->CurrentMinor;
		}

		const char *GetCurrentMajorVerString();

		Core *GetCore() volatile;

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
		std::optional<uintptr_t> getFunctionBegin(const char *needle);

		/**
		 * Run-time lookup of memory regions.
		 * 	While positive results are (often) correct, take guesses
		 * with a huge grain of salt.
		 * @param addr Address to look up
		 * @return named description of the memory region.
		 */
		const char *getMemoryRegionDescriptor(const uintptr_t addr) volatile;
	};
};

static feature_management::engines::GlobalEngine CurrentEngine;

static volatile feature_management::engines::GlobalEngine * vEngine = &CurrentEngine;

/**
 * Called variably based on fps
 * @param current_frame_tick - ticks remaining before rendering the next frame
 */
void game_tick(int current_frame_tick);

/**
 * Called right before game loop starts, memory has already been initialized
 */
void main_setup_connection_init();
