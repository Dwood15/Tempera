#pragma once

#include <versions.h>

namespace feature_management::engines {
	//TOTAL AND COMPLETE HACK BECAUSE
	//FUCKING MSVC IS DEFAULTING TO STD C++17 for some god-knows-reason
	//DESPITE FUCKING SPECIFYING C++latest.
	//
	namespace _sapien {
		static const char *DEBUG_FILENAME = const_cast<char*>("tempera.hek.sapien.debug.log");
	}

	class Sapien : public IEngine<Sapien> {
	public:
		Sapien() = default;

		~Sapien() = default;

		static void WriteHooks();

		static const defined_functionrange * GetFunctionMap();

		static void __stdcall OnPlayerActionUpdate();

		static LPCoreAddressList GetCoreAddressList();

		// static const char* LUAFILE = "hek.sapien.init.txt";
		static features SupportedFeatures() {
			return features::LUA_HOOKS;
		}
	};
};
