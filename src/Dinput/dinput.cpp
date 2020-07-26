//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"
#include <Windows.h>

#ifdef __GNUC__
#define __in
#define __out
#define __reserved
#endif

#include <xinput.h>

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
	void RegisterLuaFunctions(::LuaScriptManager *mgr) {
		mgr->registerGlobalLuaFunction("GetControllerState", [](lua_State *L) {
			auto         idx = getLuaInt(L);
			XINPUT_STATE state;

			ZeroMemory(&state, sizeof(XINPUT_STATE));

			auto result = XInputGetState(idx, &state);

			if (result != ERROR_SUCCESS) {
				lua_pushinteger(L, -1);
				lua_pushinteger(L, -1);
				return 2;
			}

			lua_createtable(L, 0, 7);

			lua_pushinteger(L, state.dwPacketNumber);
			lua_setfield(L, -2, "dwPacketNumber");

			lua_pushinteger(L, state.Gamepad.wButtons);
			lua_setfield(L, -2, "buttons");

			lua_pushinteger(L, state.Gamepad.bLeftTrigger);
			lua_setfield(L, -2, "leftTrigger");

			//The::std::string might seem redundant, and it probably is. But, I'm pretty sure std string will prune uninterpretable characters.
			lua_pushinteger(L, state.Gamepad.bRightTrigger);
			lua_setfield(L, -2, "rightTrigger");

			lua_pushinteger(L, state.Gamepad.sThumbLX);
			lua_setfield(L, -2, "thumbLX");

			lua_pushinteger(L, state.Gamepad.sThumbLY);
			lua_setfield(L, -2, "thumbLY");

			lua_pushinteger(L, state.Gamepad.sThumbRX);
			lua_setfield(L, -2, "thumbRX");

			lua_pushinteger(L, state.Gamepad.sThumbRY);
			lua_setfield(L, -2, "thumbRY");

			lua_pushinteger(L, 0);

			return 2;

		});
	}
};

namespace gamepads {
	__int16 *player_ui_globals_single_player_local_player_controllers = (__int16 *) 0x6AFE26;

	signed __int16 __fastcall player_ui_get_single_player_local_player_from_controller(__int16 player) {
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

		if (local_player_index > CurrentEngine.GetLocalPlayerCount()) {
			return datum_index::null();
		}

		return CurrentEngine.GetLocalPlayers()[local_player_index];
	}
};
