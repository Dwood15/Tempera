#pragma once

#include <versions.h>

namespace feature_management::engines {
	class CE110 : public IEngine<CE110> {
#include "function_map.inl"

	public:
		CE110() {}
		~CE110() {}

		static const defined_functionrange * GetFunctionMap() {
			return hce110_function_map;
		}

		static inline char *DEBUG_FILENAME = const_cast<char*>("tempera.hce.1_10.debug.log");

		static LPCoreAddressList GetCoreAddressList();

		static features SupportedFeatures() {
			return features::EVERYTHING;
		}

		static void WriteHooks();
	};
};
