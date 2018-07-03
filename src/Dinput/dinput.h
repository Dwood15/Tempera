#pragma once
#include <dinput.h>
#include "../CurrentEngine.h"

namespace Input {
	namespace DInput {
		static IDirectInputDevice8A *GetJoystick(unsigned int idx = 0) {
			if (idx > 7) {
				return NULL;
			}

			return (CurrentEngine.GetJoystickInputs())[idx];
		}

		//because static doesn't work with the current compilation order I guess.
		extern void RegisterLuaFunctions(::LuaScriptManager * mgr);
	}
}
