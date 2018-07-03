#pragma once

#include <versions.h>
#include <dinput.h>

namespace feature_management::engines {
	class CE110 : public IEngine<CE110> {
#include "function_map.inl"

	public:
		CE110() {}

		~CE110() {}

		static const defined_functionrange *GetFunctionMap() {
			return hce110_function_map;
		}

		static inline char *DEBUG_FILENAME = const_cast<char *>("tempera.hce.1_10.debug.log");

		static LPCoreAddressList GetCoreAddressList();

		static features SupportedFeatures() {
			return features::EVERYTHING;
		}

		static IDirectInput8A *GetDInput8Device()          { return *(IDirectInput8A **) 0x64C52C; }

		static IDirectInputDevice8A *GetKeyboardInput()    { return *(IDirectInputDevice8A **) 0x64C730; }

		static IDirectInputDevice8A *GetMouseInput()      { return *(IDirectInputDevice8A **) 0x64C734; }

		static IDirectInputDevice8A **GetJoystickInputs() { return (IDirectInputDevice8A **) 0x64C778; }

		static void OnPlayerActionUpdate();

		static void OnUnitControlUpdate(int client_update_idx);

		static void WriteHooks();
	};
};
