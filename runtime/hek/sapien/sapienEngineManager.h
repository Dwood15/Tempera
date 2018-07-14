#pragma once

#include <versions.h>
#include "../../../src/core.h"

namespace feature_management::engines {
	class Sapien : public IEngine<Sapien> {

	public:
		Sapien() = default;

		~Sapien() = default;

		static void WriteHooks();

		static const defined_functionrange * GetFunctionMap();

		static void OnPlayerActionUpdate();

		static Core GetCoreAddressList();

		static inline char *DEBUG_FILENAME = const_cast<char*>("tempera.hek.sapien.debug.log");

		// static const char* LUAFILE = "hek.sapien.init.txt";
		static features SupportedFeatures() {
			return features::LUA_HOOKS;
		}
	};
};
