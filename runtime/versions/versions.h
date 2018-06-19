#pragma once

namespace engines {
	enum major {
		CE,
		MAC,
		PC,
		STUBBZ,
		TRIAL,
		HEK,
		DEDI,
	};

	//No plans to fully support more than CE 1_10 at this time,
	//just preparing for the possibility.
	enum minor {
		halo_1_00,
		halo_1_01,
		halo_1_02,
		halo_1_03,
		//halo_1_031,
		halo_1_04,
		halo_1_05,
		halo_1_06,
		halo_1_07,
		halo_1_08,
		halo_1_09,
		//Major support
		halo_1_10,
		dedi,
		stubbz,
		trial,
		//Minor support - Open Sauce compatibility
		tool,
		//Major support - Useful for debugging player input
		sapien,
		//No plans to support guerilla at this time.
		guerilla
	};



	template<major MAJ, minor MIN>
	constexpr bool platform_has_support() {
		switch (MIN) {
			case minor::halo_1_10:
				return MAJ == major::CE;
			case minor::sapien:
				return MAJ == major::HEK;
			default:
				break;
		}
		return false;
	}

	static bool IsCurrentEnginePlatformSupported() {
		bool res = false;

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

// These bad boys are probably overkill
// #define IS_PC(MAJ) (MAJ == engines::major::PC) && HAS_SUPPORT(false)
// #define IS_CE(MAJ) (MAJ == engines::major::CE) && HAS_SUPPORT(true)

// #define IS_TOOL(MIN)  (MIN == engines::minor::tool)
// #define IS_GUERILLA(MIN)  (MIN == engines::minor::guerilla)
// #define IS_SAPIEN(MIN)  (MIN == engines::minor::sapien)
// #define IS_HEK(MAJ) (MAJ == engines::major::HEK)
//
// #define OTHER_SUPPORTED(MAJ, MIN)
// #define PC_SUPPORTED(MAJ, MIN) (IS_SAPIEN(MIN) && IS_HEK(MAJ))
// #define HEK_SUPPORTED(MAJ, MIN) (IS_SAPIEN(MIN) && IS_HEK(MAJ))
// #define CE_SUPPORTED(MAJ, MIN) ( MAJ == engines::major::CE && MIN == engines::minor::halo_1_10)
// //Apparently my MVC is out of date and doesn't support all of C++ 17 like I thought it would.
// #define IS_SUPPORTED_TARGET(MAJ, MIN)  HEK_SUPPORTED(MAJ, MIN) || CE_SUPPORTED(MAJ, MIN) || PC_SUPPORTED(MAJ, MIN)
