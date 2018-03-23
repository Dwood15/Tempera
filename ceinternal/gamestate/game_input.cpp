#pragma once
#include "../gamestate_headers/game_input.h"
long input::GetControlState(short device, short type, short index, short direction) {
	long state = 0;

	if( device == enums::_ControlDeviceKeyboard ) state = GetKeyState((enums::key_code)index);
	else if( device == enums::_ControlDeviceMouse ) {
		if( type == enums::_ControlTypeButton ) state = GetMouseButtonState((enums::MouseButton)index);
		else if( type == enums::_ControlTypeAxis ) state = GetMouseAxisState((enums::MouseAxis)index);
	} else if( device == enums::_ControlDeviceGamepad ) {
		if( type == enums::_ControlTypeButton ) state = GetGamepadButtonState((enums::GamepadButton)index);
		else if( type == enums::_ControlTypeAxis ) state = GetGamepadAxisState((enums::GamepadAxis)index);
		else if( type == enums::_ControlTypeDpad ) state = GetGamepadDpadState((enums::GamepadDpad)index);
	}

	// Check if direction matches setting
	if( type == enums::_ControlTypeAxis &&
		 direction &&
		 state != ( direction > 0 ? 1 : 2 ))
		state = 0;
	else if( type == enums::_ControlTypeDpad &&
				state != direction )
		state = 0;

	return state;
}

void input::SetControlState(int16 device, int16 type, int16 index, int32 state) {
	if( device == Enums::_ControlDeviceKeyboard ) SetKeyState((Enums::key_code)index, (byte)state);
	else if( device == Enums::_ControlDeviceMouse ) {
		if( type == Enums::_ControlTypeButton ) SetMouseButtonState((Enums::MouseButton)index, (byte)state);
		else if( type == Enums::_ControlTypeAxis ) SetMouseAxisState((Enums::MouseAxis)index, state);
	} else if( device == Enums::_ControlDeviceGamepad ) {
		if( type == Enums::_ControlTypeButton ) SetGamepadButtonState((Enums::GamepadButton)index, (byte)state);
		else if( type == Enums::_ControlTypeAxis ) SetGamepadAxisState((Enums::GamepadAxis)index, (sbyte)state);
		else if( type == Enums::_ControlTypeDpad ) SetGamepadDpadState((Enums::GamepadDpad)index, state);
	}
}