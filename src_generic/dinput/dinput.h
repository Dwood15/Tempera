#pragma once

#include <dinput.h>
#include "macros_generic.h"

namespace Input {
	namespace DInput {
		static IDirectInput8A *GetDInput8Device() { return *(IDirectInput8A **) 0x64C52C; }

		static IDirectInputDevice8A *GetKeyboardInput() { return *(IDirectInputDevice8A **) 0x64C730; }

		static IDirectInputDevice8A *GetMouseInput() { return *(IDirectInputDevice8A **) 0x64C734; }

		static IDirectInputDevice8A **GetJoystickInputs() { return (IDirectInputDevice8A **) 0x64C778; }

		static IDirectInputDevice8A *GetJoystick(unsigned int idx = 0) {
			if (idx > 7) {
				return NULL;
			}

			return (GetJoystickInputs())[idx];
		}

	}
}
