#pragma once

namespace engines {
	enum major {
		CE,
		PC,
		STUBBZ,
		TRIAL
	};

	//No plans to fully support more than CE 1_10 at this time,
	//just preparing for the possibility.
	enum with_minor {
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
		halo_1_10,
		stubbz,
		trial,
	};
};

#define ADDR static constexpr uintptr_t
static const char *K_DEBUG_FILENAME     = "tempera.debug.log";
// static const char *K_EVENT_LOG_FILENAME = "tempera.Log.txt";

//I know this isn't ideal.
extern engines::major ENGINE_TARGET;
extern engines::with_minor GAME_MINOR;

//TODO: Something like: "GetGameRegistryPath"
extern const char * GAME_REGISTRY_PATH;
