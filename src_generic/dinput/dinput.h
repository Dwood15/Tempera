#pragma once

#include <dinput.h>
#include "../headers/macros_generic.h"

namespace Input {
	namespace DInput {
		inline class Dinput8Manager {
			static IDirectInput8       *DInput8Device;
			static IDirectInputDevice8 *KeyboardInput;
			static IDirectInputDevice8 *MouseInput;
			static IDirectInputDevice8 ** JoystickInput;

			Dinput8Manager() {
				DInput8Device = *(IDirectInput8 **) 0x64C52C;
				KeyboardInput = *(IDirectInputDevice8 **) 0x64C730;
				MouseInput    = *(IDirectInputDevice8 **) 0x64C734;
				JoystickInput = (IDirectInputDevice8 **) 0x64C778;
			}

		public:
			IDirectInputDevice8 * GetJoystick(unsigned int idx = 0) {
				if (idx > 7) {
					return NULL;
				}

				return (JoystickInput)[idx];
			}
		};
	}
}
