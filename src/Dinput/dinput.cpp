#pragma once

#include <Windows.h>
#include "dinput.h"

#include <strsafe.h>
#include <addlog.h>

namespace Input::DInput {

#include <xinput.h>
	void RegisterLuaFunctions(::LuaScriptManager *mgr) {
		mgr->registerGlobalLuaFunction("GetControllerState", [](lua_State * L) {
			auto idx = getLuaInt(L);
			XINPUT_STATE state;

			ZeroMemory(&state, sizeof(XINPUT_STATE));

			auto result = XInputGetState(idx, &state);

			if(result != ERROR_SUCCESS) {
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

			//The std::string might seem redundant, and it probably is. But, I'm pretty sure std string will prune uninterpretable characters.
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
