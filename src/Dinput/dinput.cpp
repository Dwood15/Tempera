#pragma once

#include "dinput.h"
#include <precompile.h>
#include <strsafe.h>

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


	static int l_GetDeviceState(lua_State *L) {
		auto idx = getLuaInt(L);
		auto joy = Input::DInput::GetJoystick(idx);

		if (joy == NULL) {
			lua_pushinteger(L, -1);
			lua_pushinteger(L, -1);
			return 2;
		}

		DIJOYSTATE dijs;


		auto hr = joy->GetDeviceState(sizeof(DIJOYSTATE), &dijs);

		if (hr != DI_OK) {
			int errVal = 1;

			switch (hr) {
				case DIERR_INPUTLOST:
					Print(true, "Could not get controller b/c we lost input");
					errVal = 2;
					break;
				case DIERR_INVALIDPARAM:
					Print(true, " Because Dwood can't into Dinput. (INVALID PARAM)\n");
					errVal = 3;
					break;
				case DIERR_NOTACQUIRED:
					Print(true, " Because You can't into Dinput. (NOT ACQUIRED)\n");
					errVal = 4;
					break;
				case DIERR_NOTINITIALIZED:
					Print(true, " Because You can't into Dinput. (NOT INITIALIZED)\n");
					errVal = 5;
					break;
				case E_PENDING:
					Print(true, " Because wait... what? (PENDING? HUH?)\n");
					errVal = 5;
					break;
			}

			lua_pushinteger(L, -1);
			lua_pushinteger(L, errVal);
			return 2;
		}

		lua_createtable(L, 0, 44);

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

		lua_pushboolean(L, (hr != DIERR_OBJECTNOTFOUND && hr != DIERR_DEVICENOTREG));

		return 1;
	}

	static int l_GetGamePadDetails(lua_State *L) {
		auto idx = getLuaInt(L);
		auto joy = Input::DInput::GetJoystick(idx);

		Print(true, "\tGot Joystick. Getting Device.\n");

		DIDEVICEINSTANCE didi;
		didi.dwSize = sizeof(DIDEVICEINSTANCE);

		auto hr = joy->GetDeviceInfo(&didi);

		if (hr != DI_OK) {
			Print(true, "\tFailed to get device. ");
			lua_pushinteger(L, -1);

			switch (hr) {
				case DIERR_INVALIDPARAM:
					Print(true, " Because Dwood can't into Dinput. (INVALID PARAM)\n");
					break;

				case DIERR_NOTINITIALIZED:
					Print(true, " Because You can't into Dinput. (NOT INITIALIZED)\n");
					break;

				case E_POINTER:
					Print(true, "\tBecause WTF? (E_POINTER) Whatever that means.\n");
					break;
			}

			return 1;
		}

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

		switch (hr) {
			case DI_OK:
				break;
			case DIERR_INVALIDPARAM:
				Print(true, "Unable to enumerate device: Invalid param.");
			case DIERR_NOTINITIALIZED:
				Print(true, "Unable to enumerate device: Not Initialized");
			default:
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

	void RegisterLuaFunctions(::LuaScriptManager *mgr) {
		mgr->registerGlobalLuaFunction("GamePadExists", l_GamePadExists);

		mgr->registerGlobalLuaFunction("GetGamePadDetails", l_GetGamePadDetails);
		mgr->registerGlobalLuaFunction("GetDeviceState", l_GetDeviceState);

		mgr->registerGlobalLuaFunction("GetGamePadCapabilities", l_GetGamePadCapabilities);

	}
};
