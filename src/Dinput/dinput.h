#pragma once
#include <dinput.h>
#include "../CurrentEngine.h"

namespace Input {
	namespace DInput {
		static IDirectInput8 * GetDInput() {
			return CurrentEngine.GetDInput8Device();
		}

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

namespace gamepads {
	// constexpr __int16 *og_player_to_controller_array =  static_cast<__int16 *>(0x6AFE26);

	constexpr unsigned int og_player_to_controller_array =  0x6AFE26;
	static __int16 player_to_controller_arr[MAX_PLAYER_COUNT_LOCAL];

	struct player_ui_globals {
		int player_ui_globals_im_dumb[0x60E4];
	};

	void clear_gamepad_to_controller();

	void set_player_index_to_gamepad_index(int player, __int16 gamepad);
};

namespace players {
	datum_index __fastcall local_player_get_player_index(short local_player_index = 0);
};
