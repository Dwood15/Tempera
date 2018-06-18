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
};

extern const char *DEBUG_FILENAME;
// static const char *K_EVENT_LOG_FILENAME = "tempera.Log.txt";
//I was going to have current_engine.h override this func but that...
//didn't quite work out the way I hoped. Need better thing
extern bool supported_target() { return false; }
//I know this isn't ideal.
extern constexpr engines::major ENGINE_TARGET;
extern constexpr engines::minor GAME_MINOR;

//TODO: Something like: "GetGameRegistryPath"
extern const char * GAME_REGISTRY_PATH;
