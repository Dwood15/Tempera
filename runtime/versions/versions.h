#pragma once

#include <filesystem>
#include <lua.h>
#include "../../src_generic/extended/addlog.h"

namespace feature_management {
	namespace engines {
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
			nope = -1,
			dedi      = 'DEDI',
			stubbz    = 'STBZ',
			trial     = 'TRIAL',
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

		static major CurrentMajorVer;
		static minor CurrentMinorVer;

		static bool IsCurrentEnginePlatformSupported() {
			bool res         = false;

			char dir[MAX_PATH];
			auto currentPath = std::experimental::filesystem::current_path();
			if (currentPath.has_filename() && currentPath.has_extension()) {
				Print(false, "Current Path: %s", currentPath.c_str());
				std::experimental::filesystem::path fileName = currentPath.filename();
				if (strcmp(reinterpret_cast<const char * >(fileName.c_str()), "sapien.exe")) {
					return true;
				}

				if (strcmp(reinterpret_cast<const char * >(fileName.c_str()), "haloce.exe")) {
					return true;
				}
			}
		}

		static const char *GetCurrentMajorVerString(major MAJ) {
			switch (MAJ) {
				case engines::major::DEDI:
					return "DEDI";
				case engines::major::HEK:
					return "HEK";
				case engines::major::PC:
					return "PC";
				case engines::major::MAC:
					return "HEK";
				case engines::major::STUBBZ:
					return "PC";
				case engines::major::TRIAL:
					return "MAC";
				case engines::major::CE:
					return "STUBBZ";
				default:
					return "";
			}
		}

		class IEngine {
		public:
			virtual ~IEngine() {}

			static bool HasSupport() { return false; }

			virtual static major MajorVersion() { return major::NO; }
			virtual static minor MinorVersion() { return minor::nope; }

			virtual static int l_GetVersionData(lua_State *L) {
				lua_pushinteger(L, MajorVersion());
				lua_pushinteger(L, MinorVersion());
				lua_pushboolean(L, false);
				return 3;
			};
		};

		class Sapien : IEngine {
		public:
			virtual static bool  HasSupport() {
				return true;
			}

			virtual static major MajorVersion() {
				return major::HEK;
			}

			virtual static minor MinorVersion() {
				return minor::sapien;
			}
		};

		class CustomEd : IEngine {
		public:
			virtual static bool  HasSupport() {
				return true;
			}

			virtual static major MajorVersion() {
				return major::HEK;
			}

			virtual static minor MinorVersion() {
				return minor::sapien;
			}
		};
	};

	static engines::IEngine currentEngine;
};

extern const char *DEBUG_FILENAME;
// static const char *K_EVENT_LOG_FILENAME = "tempera.Log.txt";
//I was going to have current_engine.h override this func but that...
//didn't quite work out the way I hoped. Need better thing
//I know this isn't ideal.

//TODO: Something like: "GetGameRegistryPath"
extern const char *GAME_REGISTRY_PATH;

#include<current_engine.h>

#define HAS_SUPPORT(val) val
