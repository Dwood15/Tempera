#pragma once

#include "memory_map.h"
#include "../src/gamestate/camera.h"
#include "../src/scenario/structures.h"
#include "../src/scenario/scenario.h"
#include <exception>
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
		EVERYTHING      = LUA_HOOKS | DX_PROXY | FORGE_MODE
	};

	namespace engines {
		/**
		 * major, minor - enumerations representing different versions of the
		 * game engine. the HEK, TRIAL, & STUBBZ are all odd ducks WRT versioning.
		 */
		enum major {
			NO = 0x0,
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
			nope      = 0x0,
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

		class IEngine {
		protected:
			major                 Major = major::NO;
			minor                 Minor = minor::nope;
			features              Supported = features::NOPE;
		public:
			IEngine() {
				Major = major::NO;
				Minor = minor::nope;
				Supported = features::NOPE;
			}

			virtual const char* GetLogFileName() {
				return "tempera.unk.debug.log";
			}

			virtual features SupportedFeatures() {
				return Supported;
			}

			virtual bool SupportsFeature(features feat) {
				return (feat & SupportedFeatures()) > 0;
			}

			virtual bool SupportsFeature(uint feat) {
				return (feat & SupportedFeatures()) > 0;
			}

			virtual bool HasSupport() { return Supported != features::NOPE; }

			const char *GetCurrentMajorVerString() {
				switch (Major) {
					case engines::major::DEDI:
						return "DEDI";
					case engines::major::HEK:
						switch (Minor) {
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

		public:
			virtual bool IsHek() { return false; }
			virtual bool IsSapien() { return false; }
			virtual bool IsCustomEd() { return false; }
			virtual minor GetMinorVersion() { return Minor; }

		public:
			virtual Yelo::Camera::s_cinematic_globals_data ** GetCinematicGlobals() { return nullptr; };
			virtual Yelo::Scenario::s_scenario_globals ** GetScenarioGlobals() { return nullptr; };
			virtual	Yelo::TagGroups::scenario ** GetGlobalScenario() { return nullptr; };
			virtual Yelo::TagGroups::coll::collision_bsp ** GetGlobalBsp3d() { return nullptr; };

			virtual LPCoreAddressList GetCoreAddressList() {
				return LPCoreAddressList();
			};

			virtual void GetMallocAddresses(uint &cpu_alloc_size, uint &game_state_globals_ptr, uint &game_state_globals_crc) {
				cpu_alloc_size = 0;
				game_state_globals_ptr = 0;
				game_state_globals_crc = 0;
			}

			virtual defined_functionrange* GetFunctionMap() {
				return nullptr;
			}

			virtual void WriteHooks() {
				//PrintLn("WriteHooks for the loaded engine is unsupported");
				throw std::exception("WriteHooks for virtual IEngine class unsupported");
			}
		};
	};
};

