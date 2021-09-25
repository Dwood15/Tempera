//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"
#include <Windows.h>

#ifdef __GNUC__
#define __in
#define __out
#define __reserved
#endif

#include <XInput.h>

#ifdef __GNUC__
#undef __in
#undef __out
#undef __reserved
#endif

#include <strsafe.h>
//#pragma clang diagnostic pop

#include <addlog.h>
#include "dinput.h"

namespace Input::DInput {
	void PollXinputStateOfController(int idx, XINPUT_STATE &state) {
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		auto res = XInputGetState(idx, &state);

		if (res != ERROR_SUCCESS) {
			PrintLn("Poll Controller idx state: %d failed ", idx);
		}
	}

	static IDirectInputDevice8A *GetJoystick(unsigned int idx) {
		if (idx > 7) {
			return nullptr;
		}

		return (CurrentRuntime->GetJoystickInputs())[idx];
	}
};

namespace gamepads {
	short *player_ui_globals_single_player_local_player_controllers = (__int16 *) 0x6AFE26;
	static short player_to_controller_arr[MAX_PLAYER_COUNT_LOCAL];

	signed short __fastcall player_ui_get_single_player_local_player_from_controller(__int16 player) {
		__int16 result = 0;

		do {
			if (++result >= 1)
				return -1;
		} while (player_ui_globals_single_player_local_player_controllers[result] != player);
		return result;
	}

	void clear_gamepad_to_controller() {
		for (int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++) {
			player_to_controller_arr[i] = (__int16) -1;
		}
	}

	void set_player_index_to_gamepad_index(int player, __int16 gamepad) {
		if (player < 0 || player > MAX_PLAYER_COUNT_LOCAL) {
			throw "Player index out of bounds!";
		}

		if (gamepad < 0 || gamepad < MAX_GAMEPAD_COUNT) {
			throw "Gamepad index out of bounds!";
		}

		player_to_controller_arr[player] = gamepad;
	}
};

namespace players {
	datum_index __fastcall local_player_get_player_index(short local_player_index) {
		if (local_player_index < 0) {
			return datum_index::null();
		}

		if (local_player_index > MAX_PLAYER_COUNT_LOCAL) {
			return datum_index::null();
		}

		if (local_player_index > CurrentRuntime->GetLocalPlayerCount()) {
			return datum_index::null();
		}

		return CurrentRuntime->GetLocalPlayers()[local_player_index];
	}
};
