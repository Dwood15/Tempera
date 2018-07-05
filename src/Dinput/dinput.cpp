#pragma once

#include <Windows.h>
#include "dinput.h"

#include <strsafe.h>
#include <addlog.h>

namespace Input::DInput {

	static char                szName[MAX_PATH];
	static std::vector<char *> enumResult;

	static const char *getGuidInfo(const GUID &gtype) {

		if (gtype == GUID_Unknown) {
			return "Unknown Guid";
		}

		if (gtype == GUID_POV) {
			return "POV Guid";
		}

		if (gtype == GUID_XAxis) {
			return "Guid XAxis";
		}

		if (gtype == GUID_YAxis) {
			return "Guid YAxis";
		}

		if (gtype == GUID_ZAxis) {
			return "Guid ZAxis";
		}

		if (gtype == GUID_RxAxis) {
			return "Guid RxAxis";
		}

		if (gtype == GUID_RyAxis) {
			return "Guid RyAxis";
		}

		if (gtype == GUID_RzAxis) {
			return "Guid RzAxis";
		}

		if (gtype == GUID_Key) {
			return "Guid Button";
		}

		if (gtype == GUID_Button) {
			return "Guid Button";
		}

		return "Literally Unknown";
	}

	char mbstring[32];

	static const char *getMbStringFromGuid(GUID guid) {
		OLECHAR *guidString;

		StringFromCLSID(guid, &guidString);

		wcstombs(mbstring, guidString, 32);

		CoTaskMemFree(guidString);

		return mbstring;
	}

	static int DINPUTERRTOLUA(HRESULT hr) {
		int errVal = 1;

		switch (hr) {
			case DIERR_INPUTLOST:
				PrintLn(" DINPUT: (LOST INPUT)");
				errVal = 2;
				break;
			case DIERR_INVALIDPARAM:
				PrintLn(" DINPUT: (INVALID PARAM)");
				errVal = 3;
				break;
			case DIERR_NOTACQUIRED:
				PrintLn(" DINPUT: (NOT ACQUIRED)");
				errVal = 4;
				break;
			case DIERR_NOTINITIALIZED:
				PrintLn(" DINPUT: (NOT INITIALIZED)");
				errVal = 5;
				break;
			case E_POINTER:
				PrintLn(" DINPUT: (E_POINTER) ");
				errVal = 6;
				break;
			case E_PENDING:
				PrintLn(" DINPUT: (PENDING? HUH?)");
				errVal = 7;
				break;
		}

		return errVal;
	}

	static int l_GetDeviceState(lua_State *L) {
		auto idx = getLuaInt(L);
		auto joy = Input::DInput::GetJoystick(idx);

		if (joy == NULL) {
			lua_pushinteger(L, -1);
			lua_pushinteger(L, -1);
			return 2;
		}


		PrintLn("Attempting to Poll Controller!");

		auto hr = joy->Poll();

		if (hr != 0) {
			PrintLn("Polling failed!");
			DINPUTERRTOLUA(hr);

			lua_pushinteger(L, -1);
			lua_pushinteger(L, -1);
			return 2;
		}

		DIJOYSTATE2 dijs2 = DIJOYSTATE2();

		hr = joy->GetDeviceState(sizeof(DIJOYSTATE2), &dijs2);


		if (hr != 0) {
			DINPUTERRTOLUA(hr);
			lua_pushinteger(L, -1);
			// return 2;
			PrintLn("Trying with state2 instead.");

			DIJOYSTATE pdijs;

			hr = joy->GetDeviceState(sizeof(DIJOYSTATE), &pdijs);



			if (hr != 0) {
				DINPUTERRTOLUA(hr);
				PrintLn("State2 Failed. RIP.");
			} else {
				PrintLn("DIJS2 success!");
			}

			lua_pushinteger(L, -1);
			return 2;
		} else {
			PrintLn("state2 success!.");

		}

		lua_createtable(L, 0, 44);

		auto dijs = dijs2;
		lua_pushinteger(L, dijs.lX);
		lua_setfield(L, -2, "lX");

		lua_pushinteger(L, dijs.lY);
		lua_setfield(L, -2, "lY");

		lua_pushinteger(L, dijs.lZ);
		lua_setfield(L, -2, "lZ");

		lua_pushinteger(L, dijs.lRx);
		lua_setfield(L, -2, "lRX");

		lua_pushinteger(L, dijs.lRy);
		lua_setfield(L, -2, "lRY");

		lua_pushinteger(L, dijs.lRz);
		lua_setfield(L, -2, "lRZ");

		lua_pushinteger(L, dijs.rglSlider[0]);
		lua_setfield(L, -2, "RGLSliderA");

		lua_pushinteger(L, dijs.rglSlider[1]);
		lua_setfield(L, -2, "RGLSliderB");

		lua_createtable(L, 0, 4);

		lua_pushinteger(L, dijs.rgdwPOV[0]);
		lua_setfield(L, -3, "0");
		lua_pushinteger(L, dijs.rgdwPOV[1]);
		lua_setfield(L, -3, "1");
		lua_pushinteger(L, dijs.rgdwPOV[2]);
		lua_setfield(L, -3, "2");
		lua_pushinteger(L, dijs.rgdwPOV[3]);
		lua_setfield(L, -3, "3");

		lua_setfield(L, -2, "RGDwPOV");

		lua_createtable(L, 0, 32);

		lua_pushinteger(L, dijs.rgbButtons[0]);
		lua_setfield(L, -3, "0");

		lua_pushinteger(L, dijs.rgbButtons[1]);
		lua_setfield(L, -3, "1");

		lua_pushinteger(L, dijs.rgbButtons[2]);
		lua_setfield(L, -3, "2");
		lua_pushinteger(L, dijs.rgbButtons[3]);
		lua_setfield(L, -3, "3");
		lua_pushinteger(L, dijs.rgbButtons[4]);
		lua_setfield(L, -3, "4");
		lua_pushinteger(L, dijs.rgbButtons[5]);
		lua_setfield(L, -3, "5");
		lua_pushinteger(L, dijs.rgbButtons[6]);
		lua_setfield(L, -3, "6");
		lua_pushinteger(L, dijs.rgbButtons[7]);
		lua_setfield(L, -3, "7");
		lua_pushinteger(L, dijs.rgbButtons[8]);
		lua_setfield(L, -3, "8");
		lua_pushinteger(L, dijs.rgbButtons[9]);
		lua_setfield(L, -3, "9");
		lua_pushinteger(L, dijs.rgbButtons[10]);
		lua_setfield(L, -3, "10");
		lua_pushinteger(L, dijs.rgbButtons[11]);
		lua_setfield(L, -3, "11");
		lua_pushinteger(L, dijs.rgbButtons[12]);
		lua_setfield(L, -3, "12");
		lua_pushinteger(L, dijs.rgbButtons[13]);
		lua_setfield(L, -3, "13");
		lua_pushinteger(L, dijs.rgbButtons[14]);
		lua_setfield(L, -3, "14");
		lua_pushinteger(L, dijs.rgbButtons[15]);
		lua_setfield(L, -3, "15");
		lua_pushinteger(L, dijs.rgbButtons[16]);
		lua_setfield(L, -3, "16");
		lua_pushinteger(L, dijs.rgbButtons[17]);
		lua_setfield(L, -3, "17");
		lua_pushinteger(L, dijs.rgbButtons[18]);
		lua_setfield(L, -3, "18");
		lua_pushinteger(L, dijs.rgbButtons[19]);
		lua_setfield(L, -3, "19");
		lua_pushinteger(L, dijs.rgbButtons[20]);
		lua_setfield(L, -3, "20");
		lua_pushinteger(L, dijs.rgbButtons[21]);
		lua_setfield(L, -3, "21");
		lua_pushinteger(L, dijs.rgbButtons[22]);
		lua_setfield(L, -3, "22");
		lua_pushinteger(L, dijs.rgbButtons[23]);
		lua_setfield(L, -3, "23");
		lua_pushinteger(L, dijs.rgbButtons[24]);
		lua_setfield(L, -3, "24");
		lua_pushinteger(L, dijs.rgbButtons[25]);
		lua_setfield(L, -3, "25");
		lua_pushinteger(L, dijs.rgbButtons[26]);
		lua_setfield(L, -3, "26");
		lua_pushinteger(L, dijs.rgbButtons[27]);
		lua_setfield(L, -3, "27");
		lua_pushinteger(L, dijs.rgbButtons[28]);
		lua_setfield(L, -3, "28");
		lua_pushinteger(L, dijs.rgbButtons[29]);
		lua_setfield(L, -3, "29");
		lua_pushinteger(L, dijs.rgbButtons[30]);
		lua_setfield(L, -3, "30");
		lua_pushinteger(L, dijs.rgbButtons[31]);
		lua_setfield(L, -3, "31");

		lua_setfield(L, -2, "RGBBUTTONS");
		lua_pushinteger(L, 0);
		return 2;

	}

	static int l_GamePadExists(lua_State *L) {
		auto idx = getLuaInt(L);
		auto joy = Input::DInput::GetJoystick(idx);

		if (joy == NULL) {
			lua_pushboolean(L, false);
			return 1;
		}

		DIDEVCAPS didoi;
		didoi.dwSize = sizeof(DIDEVCAPS);

		auto hr = joy->GetCapabilities(&didoi);

		if (hr == 0) {
			// hr = joy->SetDataFormat(&c_dfDIJoystick2);
			// if(FAILED(hr)) {
			// 	PrintLn("Could not set device format.");
			// 	DINPUTERRTOLUA(hr);
			// }
		}

		lua_pushboolean(L, hr == 0);

		return 1;
	}

	static int l_GetGamePadDetails(lua_State *L) {
		auto idx = getLuaInt(L);
		auto joy = Input::DInput::GetJoystick(idx);

		PrintLn("\tGot Joystick.");

		PrintLn("\tGetting DeviceINFO.");

		static DIDEVICEINSTANCE didi;
		didi.dwSize = sizeof(DIDEVICEINSTANCE);

		int hr = joy->GetDeviceInfo(&didi);

		PrintLn("\tValidating DeviceInfo result.");

		if (hr != DI_OK) {
			Print("\tFailed to get device. ");
			lua_pushinteger(L, -1);

			DINPUTERRTOLUA(hr);
			return 1;
		}

		PrintLn("\t****Got Device.");

		lua_createtable(L, 0, 6);

		lua_pushinteger(L, didi.dwSize);
		lua_setfield(L, -2, "dwSize");

		//The std::string might seem redundant, and it probably is. But, I'm pretty sure std string will prune uninterpretable characters.
		lua_pushstring(L, std::string(getMbStringFromGuid(didi.guidInstance)).c_str());
		lua_setfield(L, -2, "guidInstance");

		//The std::string might seem redundant, and it probably is. But, I'm pretty sure std string will prune uninterpretable characters.
		lua_pushstring(L, std::string(getMbStringFromGuid(didi.guidProduct)).c_str());
		lua_setfield(L, -2, "guidProduct");

		lua_pushinteger(L, didi.dwDevType);
		lua_setfield(L, -2, "dwDevType");

		lua_pushstring(L, didi.tszInstanceName);
		lua_setfield(L, -2, "friendlyInstanceName");

		lua_pushstring(L, didi.tszProductName);
		lua_setfield(L, -2, "friendlyProductName");

		PrintLn("\tReturning Table...");

		return 1;
	}

	static int l_GetGamePadCapabilities(lua_State *L) {
		auto idx = getLuaInt(L);
		auto joy = Input::DInput::GetJoystick(idx);

		if (joy == NULL) {
			lua_pushinteger(L, -1);
			lua_pushinteger(L, -1);
			return 2;
		}

		DIDEVCAPS didc;
		didc.dwSize = sizeof(DIDEVCAPS);

		auto hr = joy->GetCapabilities(&didc);

		if (hr != 0) {
			DINPUTERRTOLUA(hr);
			lua_pushinteger(L, -1);
			lua_pushinteger(L, hr);
			return 2;
		}

		lua_createtable(L, 0, 4);

		lua_pushinteger(L, didc.dwDevType);
		lua_setfield(L, -2, "dwDevType");

		//The std::string might seem redundant, and it probably is. But, I'm pretty sure std string will prune uninterpretable characters.
		lua_pushinteger(L, didc.dwAxes);
		lua_setfield(L, -2, "dwAxes");

		//The std::string might seem redundant, and it probably is. But, I'm pretty sure std string will prune uninterpretable characters.
		lua_pushinteger(L, didc.dwButtons);
		lua_setfield(L, -2, "dwButtons");

		lua_pushinteger(L, didc.dwPOVs);
		lua_setfield(L, -2, "dwPovs");

		lua_pushinteger(L, 0);

		return 2;
	}

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
