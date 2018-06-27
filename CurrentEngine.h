#pragma once

#include <precompile.h>
#include "runtime/versions.h"
#include "src/core.h"
#include "src/lua/script_manager.h"

class LuaScriptManager;

namespace feature_management::engines {
	class GlobalEngine {
		features              CurrentSupported = features::NOPE;
		major                 CurrentMajor     = major::NO;
		minor                 CurrentMinor     = minor::nope;
		defined_functionrange *current_map;
		Core                  *eCore = (Core *)-1;

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


		bool IsHek();

		bool IsSapien();

		bool IsCustomEd();

		bool IsCoreInitialized();

		void WriteHooks();

		bool HasSupport();

		bool SupportsFeature(features feat) {
			return (this->GetSupported() & feat) == feat;
		}

		bool SupportsFeature(uint feat) {
			return (this->GetSupported() & feat) == feat;
		}

		void MakePlayerGoForward();


		void InitializeLuaState();

		void LuaFirstRun() {
			if (LuaState) {
				LuaState->DoFirstRun();
			} else {
				Print(true, "Lua Failed to initialize & run!\n");
			}
		}

		GlobalEngine();

		void RefreshCore();

		minor GetMinorVersion() {
			return this->CurrentMinor;
		}

		const char *GetCurrentMajorVerString();

		Core *GetCore() {
			return this->eCore;
		}

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
