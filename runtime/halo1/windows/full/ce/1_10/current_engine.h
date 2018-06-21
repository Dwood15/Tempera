#pragma once
#include <versions.h>
#include <winnt.h>
#include <precompile.h>

//I recognize this isn't ideal constexpr setup.

namespace feature_management::engines {

	class CE110 : public CustomEd {
		static const uint supportedFeats = features::LUA_HOOKS | features::DX_PROXY | features::FORGE_MODE;
	public:

		const char* DEBUG_FILENAME = "tempera.hce.1_10.debug.log";

		static major MajorVersion() {
			return major::CE;
		}

		static minor MinorVersion() {
			return minor::nope;
		}
	};
};
//TODO: Something like "GetGameRegistryPath"
