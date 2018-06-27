#pragma once

#include "../../precompile.h"

#include <engine_interface.h>
#include "../../src_generic/extended/addlog.h"
#include "../../src_generic/gamestate/core.h"
#include "../../src_generic/headers/hce_addresses.h"
#include "../../src_generic/lua/script_manager.h"
#include "../../src_generic/gamestate/forge.h"

/**
 * I'm going to go on a bit of a lecture here everyone who reads this code
 * can potentially understand what I'm trying to do here.
 *
 * Tempera is a different project structure for large and expansive modding
 * of the blam engine. Inherited and a bit of a mishmash between two old
 * projects where one never really got off the ground (haloforge) and the
 * other only sees modest use from SPv3 and CMT, my goal for the library
 * is to create a truly extensible (and readable) system for creating and
 * managing large mods of a game engine.
 *
 * Open Sauce's primary failing was that it was so large and complex,
 * it's practically unusable. Additionally, while I have mad respect for
 * the creator's work, not only is Open Sauce not nearly as widely used as
 * it should be for the massive amount of power it gives the developer,
 * Open Sauce is frozen in time. Kornman00 has gone on to work for 343i.
 *
 * I believe is safe to assume that Kornman00 will not be providing updates
 * to Open Sauce.
 *
 * --------------------
 *
 * With that preface, let's dive into the idea behind the managing systems.
 *
 * IEngine - The Generic interface for utilizing features provided within
 * Tempera. Every implementation of the engine which is added should be
 * a descendant of this interface.
 *
 */

namespace feature_management {
	/**
	 * features - enumerations representing various (large) sets of features.
    * Minor differences in implementations are delegated to the * implementing
    * class.
	 */
	enum features : unsigned int {
		NOPE            = 0x0,
		LUA_HOOKS       = 0x1,
		DX_PROXY        = 0x2,
		FORGE_MODE      = 0x4,
		MARIADB_LOGGING = 0x8,
		EVERYTHING      = LUA_HOOKS | DX_PROXY | FORGE_MODE | MARIADB_LOGGING
	};

	namespace engines {
		/*
		 * major, minor - enumerations representing different versions of the
		 * game engine. the HEK, TRIAL, & STUBBZ are all odd ducks WRT versioning.
		 */
		enum major {
			NO = -1,
			CE,
			HEK,
			PC,
			MAC,
			STUBBZ,
			TRIAL,
			DEDI,
		};

		//No plans to fully support more than CE 1_10 at this time,
		//just preparing for the possibility of dynamic feature support based on version.
		enum minor {
			nope      = -1,
			dedi      = 'DEDI',
			stubbz    = 'STBZ',
			trial     = 'TRIL',
			//Basically no support planned, but these are potentially useful anyhow.
			halo_1_00 = 0x1000,
			halo_1_01 = 0x1001,
			halo_1_02 = 0x1002,
			halo_1_03 = 0x1003,
			halo_1_04 = 0x1004,
			halo_1_05 = 0x1005,
			halo_1_06 = 0x1006,
			halo_1_07 = 0x1007,
			halo_1_08 = 0x1008,
			halo_1_09 = 0x1009,
			//Primary platform - Major support
			halo_1_10 = 0x1010,

			//Minor support - Open Sauce compatibility
				tool     = 'TOOL',
			//No plans to support guerilla at this time.
				guerilla = 'GUER',
			//Useful for debugging, testing player input
				sapien   = 'SAPI',
		};

		template <class T>
		class IEngine {
		public:
			const char *LUAFILE        = "init.txt";

			static bool SupportsFeature(features feat) {
				return (feat & T::SupportedFeatures()) > 0;
			}

			bool SupportsFeature(uint feat) {
				return (feat & T::SupportedFeatures()) == feat;
			}

			static bool HasSupport() { return T::SupportedFeatures() != features::NOPE; }

		};

		class Sapien : public IEngine<Sapien> {
#include <hek/sapien/sapien_func_map.inl>

		public:
			Sapien() {}

			~Sapien() {}

			static const defined_functionrange * GetFunctionMap() {
				return sapien_function_map;
			}

			static inline char *DEBUG_FILENAME = const_cast<char*>("tempera.hek.sapien.debug.log");

			// static const char* LUAFILE = "hek.sapien.init.txt";
			static features SupportedFeatures() {
				return features::LUA_HOOKS;
			}
		};

		class CustomEd : public IEngine<CustomEd> {
		public:
			CustomEd() {}

			~CustomEd() {}

			static inline char *DEBUG_FILENAME     = const_cast<char*>("tempera.ce.unk.debug.log");
			const char *GAME_REGISTRY_PATH = R"(Software\Microsoft\Microsoft Games\Halo CE)";
			// static const char* HCE_LUAFILE = "hce.init.txt";

			static features SupportedFeatures() {
				return features::LUA_HOOKS;
			}

		};

		class CE110 : public IEngine<CE110> {
#include <hce110_func_map.inl>
		public:
			CE110() {
				printf("Creating haloce engine.\n");
			}

			~CE110() {}

			static const defined_functionrange * GetFunctionMap() {
				return hce110_function_map;
			}

			static inline char *DEBUG_FILENAME = const_cast<char*>("tempera.hce.1_10.debug.log");

			static LPCoreAddressList GetCoreAddressList();

			static features SupportedFeatures() {
				return features::EVERYTHING;
			}

			static inline void WriteHooks();
		};

		class GlobalEngine {
			features  CurrentSupported = features::NOPE;
			major CurrentMajor     = major::NO;
			minor CurrentMinor     = minor::nope;
			defined_functionrange * current_map;
			Core * eCore;
			//Support Attempted

			// std::string GetCurrentFileName(char * args) {
			std::string GetCurrentFileName();

			bool VerSupported();

			features GetSupported();

		public:
			char * DEBUG_FILENAME = const_cast<char*>("tempera.unk.unk.debug.log");
			static inline const char * LUA_FILENAME = const_cast<char*>("tempera.init.txt");

			bool IsHek() {
				return this->CurrentMajor == major::HEK;
			}

			bool IsSapien() {
				return this->IsHek() && this->CurrentMinor == minor::sapien;
			}

			bool IsCustomEd() {
				return this->CurrentMajor == major::CE;
			}

			bool HasSupport();

			bool SupportsFeature(features feat) {
				return (this->GetSupported() & feat) == feat;
			}

			bool SupportsFeature(uint feat) {
				return (this->GetSupported() & feat) == feat;
			}

			GlobalEngine();

			minor GetMinorVersion() {
				return this->CurrentMinor;
			}

			const char *GetCurrentMajorVerString();

			Core * GetCore() {
				return this->eCore;
			}

			constexpr bool equal(const char *lhs, const char *rhs) {
				while (*lhs || *rhs) {
					if (*lhs++ != *rhs++) {
						return false;
					}
				}
				return true;
			}

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
};

static feature_management::engines::GlobalEngine CurrentEngine;



