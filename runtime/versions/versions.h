#pragma once

#include <precompile.h>

#include <memory_map.h>
#include <engine_interface.h>
#include "../../src_generic/extended/addlog.h"
#include "../../src_generic/gamestate/core.h"
#include "../../src_generic/headers/hce_addresses.h"
#include "../../src_generic/lua/script_manager.h"

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

			static LPCoreAddressList GetCoreAddressList() {
				LPCoreAddressList CurrentCore;
				CurrentCore.core_0 = 0x815900;
				CurrentCore.core_1 = 0x7FB6F8;
				CurrentCore.core_2 = 0x7FBE70;

				CurrentCore.core_3 = 0x87A76C;

				CurrentCore.core_4 = 0x81B800;
				CurrentCore.core_5 = 0x81B894;
				CurrentCore.core_6 = 0x653BE4;
				CurrentCore.core_7 = 0x71D0E8;
				CurrentCore.CAMERA = 0x6474E4;
				CurrentCore.MAP_HEADER_ADDRESS = 0x643044;
				CurrentCore.TAG_INDEX_HEADER_ADDRESS = 0x40440000;

				return CurrentCore;
			}

			static features SupportedFeatures() {
				return features::EVERYTHING;
			}

			static inline void WriteHooks() {
				//TODO: Coneceptually separate these out..
				//Function call
				// 			signature: "8B 35 20 59 81 00 57 8B FA B9 .26 00 00 00 F3 AB 83 CF FF"
				//constexpr uintptr_t players_initialize_for_new_map_overwrite = 0x476243; // overwrite the .26 with the size of the 4 player structure.
				//Relying on sizeof allows us to redefine MAX_PLAYER variables/defines
				//patchValue<unsigned int>(players_initialize_for_new_map_overwrite, sizeof(s_players_globals_data) / 4);

				//uintptr_t player_spawn = 0x47A9E0; Valid, just not using it...
				constexpr uintptr_t size_of_fp_weapons = 0x1EA0 * MAX_PLAYER_COUNT_LOCAL;

				uintptr_t fp_weap_initialize[] = {0x497122, 0x49712F};

				spcore::memory::adjustNPatch32(fp_weap_initialize, size_of_fp_weapons);

				//uintptr_t hud_scripted_globals_sizeofs[]          = { 0x4AC7A7, 0x4AC7AF };
				uintptr_t hud_messaging_globals_sizeofs[] = {0x4AC7DD, 0x4AC7EA};
				uintptr_t hud_messaging_state_size        = 0x4AC936;
				//static_assert(sizeof(s_hud_messaging_state)  == 0x122, "stat_assrt_fail: s hud msging state");
				//memset , or rather, rep stosd assumes full integer (0x4) size in this case.
				spcore::memory::patchValue<uintptr_t>(hud_messaging_state_size, sizeof(spcore::s_hud_messaging_state) / 4);
				uintptr_t motion_sensor_sizeofs[] = {0x4AC8B3, 0x4AC8BC + 0x4};

				//			Need to confirm these...
				//			uintptr_t unit_hud_globals_sizeofs[]              = { 0x4AC813, 0x4AC81B + 0x4 };
				//			uintptr_t weapon_hud_globals_sizeofs[]            = { 0x4AC848, 0x4AC850 + 0x4 };
				//			uintptr_t hud_interface_related_globals_sizeofs[] = { 0x4AC87D, 0x4AC885 + 0x4 };

				//adjustNPatch32(hud_scripted_globals_sizeofs, 0x4);
				//0x488 og size.
				spcore::memory::adjustNPatch32(hud_messaging_globals_sizeofs, sizeof(spcore::s_hud_messaging_state));
				//			adjustNPatch32(unit_hud_globals_sizeofs, 0x5C);
				//			adjustNPatch32(weapon_hud_globals_sizeofs, 0x7C);
				//			adjustNPatch32(hud_interface_related_globals_sizeofs, 0x30);
				spcore::memory::adjustNPatch32(motion_sensor_sizeofs, sizeof(s_motion_sensor));

				//Gotta be able to loop over all the players + input devices, no?.
				//"E8 4E 9A 01 00 E8 .69 7D 01 00 8B 15 44 C8 68 00"
				constexpr uintptr_t player_control_init_new_map_hook = 0x45BC33;
				// //Hooks

				calls::WriteSimpleHook(player_control_init_new_map_hook, &spcore::player_control::player_control_initialize_for_new_map);

				//Lua Hooks
				constexpr uintptr_t parse_for_connect_invert_hook = 0x4CAAC4;
				calls::WriteSimpleHook(parse_for_connect_invert_hook, &::parse_for_connect_invert);

				constexpr uintptr_t game_tick_hook = 0x473815;
				calls::WriteSimpleHook(game_tick_hook, &::game_tick);
				// patchValue<uintptr_t>(player_control_init_new_map_hook, addr); //Gotta be able to loop over all the players + input devices, no?.

				// constexpr uintptr_t scenario_load_hookA = 0x4CC9AD; //inside main_new_map
				// addr = calc_addr_offset(scenario_load_hookA, (int)&scenario_load);
				// patchValue<uintptr_t>(scenario_load_hookA, addr); //Gotta be able to loop over all the players + input devices, no?.
				//
				// constexpr uintptr_t scenario_load_hookB = 0x4E1C28; //inside network_game_create_game_objects
				// addr = calc_addr_offset(scenario_load_hookB, (int)&scenario_load);
				// patchValue<uintptr_t>(scenario_load_hookB, addr); //Gotta be able to loop over all the players + input devices, no?.

				//This hook is called right before the game enters the actual main loop


				//Nothing wrong's with the hook, just the function.
				//			real_address        = (int)&initializations::motion_sensor_initialize_for_new_map;
				//			real_address_offset = ( real_address ) - ((int)0x4AC98E);																					// + (int)4);
				//
				//			patchValue<uintptr_t>(((unsigned int)0x4AC98E ), (unsigned int)( real_address_offset ) - 4);


				// insertRenderWindowCountHooks();

				//_rasterizer_detail_objects_draw51EF90
				//THIS ONE IS THE SAME AS XBOX BETA ALMOST.
				//			constexpr uintptr_t get_render_window_ct_patch_6 = 0x51EE05;
				//patchValue<short>(, (short)-1);
				::run_byte_size_patches();
			}
		};

		class GlobalEngine {
			features  CurrentSupported = features::NOPE;
			major CurrentMajor     = major::NO;
			minor CurrentMinor     = minor::nope;
			defined_functionrange * current_map;
			Core * eCore;
			//Support Attempted
			Sapien   sap = Sapien();
			CustomEd hce = CustomEd();

			// std::string GetCurrentFileName(char * args) {
			std::string GetCurrentFileName() {
				static char name[64];
				auto        lpCmdline = GetCommandLineA();

				auto procName = GetModuleFileName(NULL, name, 64);


				//GetModuleFileNameA(hmodule, name, std::size(name));
				printf("CommandLine Args: %s\n", lpCmdline);
				printf("File Name: %s\n", name);

				int num = 63;
				while (name[num] != 'e') {
					num--;
				}
				int till = num;
				while (name[till] != '\\') {
					till--;
				}

				return std::string(name).substr(till + 1, num - (till));
			}

			bool VerSupported() {
				if (this->CurrentMajor == major::CE) {
					switch (this->CurrentMinor) {
						case minor::halo_1_10:
							return (CE110::SupportedFeatures() > features::NOPE);
						default:
							return false;
					}
				}

				return false;
			};

			features GetSupported() {
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

			bool HasSupport() {
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

			bool SupportsFeature(features feat) {
				return (this->GetSupported() & feat) == feat;
			}

			bool SupportsFeature(uint feat) {
				return (this->GetSupported() & feat) == feat;
			}

			GlobalEngine() {
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
					this->CurrentMajor = major::HEK;
					this->CurrentMinor = minor::sapien;
					this->DEBUG_FILENAME = Sapien::DEBUG_FILENAME;
					this->current_map = const_cast<defined_functionrange *>(Sapien::GetFunctionMap());
				}

				if (fName == "haloce.exe") {
					printf("Detected haloce!\n");
					this->CurrentMajor = major::CE;
					this->CurrentMinor = minor::halo_1_10;
					this->DEBUG_FILENAME = CE110::DEBUG_FILENAME;
					this->current_map = const_cast<defined_functionrange *>(CE110::GetFunctionMap());
					this->eCore = new Core(CE110::GetCoreAddressList());
					CE110::WriteHooks();
				}

				if (this->HasSupport()) {
					CurrentSupported = this->GetSupported();
				}
			}

			minor GetMinorVersion() {
				return this->CurrentMinor;
			}

			const char *GetCurrentMajorVerString() {
				switch (this->CurrentMajor) {
					case engines::major::DEDI:
						return "DEDI";
					case engines::major::HEK:
						return "HEK";
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
			uintptr_t getFunctionBegin(const char *needle) {
				using dfr = defined_functionrange;

				const dfr* funcList = current_map;

				for (dfr * item = const_cast<dfr *>(funcList); item++;  item) {
					if (equal(needle, item->funcName)) {
						return item->begin;
					}
				}

				return (uintptr_t) -1;
			}

			/**
			 * Run-time lookup of memory regions.
			 * 	While positive results are (often) correct, take guesses
			 * with a huge grain of salt.
			 * @param addr Address to look up
			 * @return named description of the memory region.
			 */
			const char *getMemoryRegionDescriptor(const uintptr_t addr) {

				printf("searching for address: 0x%X\n", addr);

				if (addr < 0x200000) {
					return "probably_in_stack";
				}

				if (addr < 0x400000) {
					return "unmapped_region";
				}
				using dfr = defined_functionrange;

				const dfr* funcList = current_map;
				for (dfr * item = const_cast<dfr*>(funcList); item++;  item) {
					if (item->contains(addr)) {
						return item->funcName;
					}
				}

				if (addr < 0x5E9020) {
					return "function_space_unmapped";
				}

				return "unmapped_region";
			}

		};
	};
};

static feature_management::engines::GlobalEngine CurrentEngine;

