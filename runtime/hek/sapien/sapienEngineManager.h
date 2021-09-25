#pragma once

#include <versions.h>
#include "../../../src/core_types.h"
#include "../../../src/gamestate/objects.h"

namespace feature_management::engines {
	class Sapien : public IEngine {
	public:
		Sapien() {
			Major = major::HEK;
			Minor = minor::sapien;
			Supported = features::LUA_HOOKS;
		}
		~Sapien() = default;

		const char* GetLogFileName() {
			return "tempera.hek.sapien.debug.log";
		}

		bool IsSapien() {
			return true;
		}

		defined_functionrange * GetFunctionMap();
		LPCoreAddressList GetCoreAddressList();
		virtual void WriteHooks();
	};
};
