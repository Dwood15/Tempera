#pragma once

#include <versions.h>
#include "../../../src/core_types.h"
#include "../../../src/gamestate/objects.h"

namespace feature_management::engines {
	class Sapien : public IEngine<Sapien> {
	public:
		Sapien() = default;
		~Sapien() = default;

		static constexpr const char *DEBUG_FILENAME = const_cast<char*>("tempera.hek.sapien.debug.log");
		static const defined_functionrange * GetFunctionMap();
		static LPCoreAddressList GetCoreAddressList();
		static void WriteHooks();

		// static const char* LUAFILE = "hek.sapien.init.txt";
		static features SupportedFeatures() {
			return features::LUA_HOOKS;
		}

		//Sapien doesn't have fancy CutScenes
		bool AreWeInCutScene() { return false; }
	};
};
