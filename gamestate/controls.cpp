#pragma once#include "${HEADER_FILENAME}"

#include "./controls.h"
//#include "Memory/_EngineLayout.inl"

namespace Flags {
	enum input_state {
		_input_state_enabled_bit,
		_input_state_menu_bit,
		_input_state_chat_bit,
	};
};

namespace Input {
	struct ControlStates {
		// key states (how long its been pressed until 0xFF, 0 if not pressed)
		byte Keys[enums::k_number_of_keys];

		PAD(0, sizeof(byte) * 383);

		int32 MouseAxis[enums::_MouseAxis];
		byte  MouseButton[enums::_MouseButton];

		PAD(1, sizeof(byte) * 4680);

		byte GamepadButton[enums::_GamepadButton];
		GamepadAxisState GamepadAxis[enums::_GamepadAxis];

		PAD(2, sizeof(byte) * 56);

		int32 GamepadDpad[enums::_GamepadDpad];
	};

	struct PositionState {
		//PAD16;
		//bool Moving; // true during mouse movement
		//PAD8;
		PAD32;
		int32 Position[2]; // menu space coordinates (0,0) to (640,480)
	};


	void Initialize() {
		Memory::CreateHookRelativeCall(&Input::Update, GET_FUNC_VPTR(INPUT_UPDATE_HOOK), enums::_x86_opcode_ret);
	}

	void Dispose() {
	}

	bool IsInGame() { return TEST_FLAG(GET_PTR(InputStateFlags), Flags::_input_state_enabled_bit); }

	bool IsInMenu() { return TEST_FLAG(GET_PTR(InputStateFlags), Flags::_input_state_menu_bit); }

	bool IsInChat() { return TEST_FLAG(GET_PTR(InputStateFlags), Flags::_input_state_chat_bit); }

	void AllowMovement(bool allow) { SET_FLAG(GET_PTR(InputStateFlags), Flags::_input_state_enabled_bit, allow); }

	int32 GetControlState(int16 device, int16 type, int16 index, int16 direction) {
		int32 state = 0;

		if (device == enums::_ControlDeviceKeyboard) state = GetKeyState((enums::key_code) index);
		else if (device == enums::_ControlDeviceMouse) {
			if (type == enums::_ControlTypeButton) state = GetMouseButtonState((enums::MouseButton) index);
			else if (type == enums::_ControlTypeAxis) state = GetMouseAxisState((enums::MouseAxis) index);
		} else if (device == enums::_ControlDeviceGamepad) {
			if (type == enums::_ControlTypeButton) state = GetGamepadButtonState((enums::GamepadButton) index);
			else if (type == enums::_ControlTypeAxis) state = GetGamepadAxisState((enums::GamepadAxis) index);
			else if (type == enums::_ControlTypeDpad) state = GetGamepadDpadState((enums::GamepadDpad) index);
		}

		// Check if direction matches setting
		if (type == enums::_ControlTypeAxis &&
			 direction &&
			 state != (direction > 0 ? 1 : 2))
			state = 0;
		else if (type == enums::_ControlTypeDpad &&
					state != direction)
			state = 0;

		return state;
	}

	void SetControlState(int16 device, int16 type, int16 index, int32 state) {
		if (device == enums::_ControlDeviceKeyboard) SetKeyState((enums::key_code) index, (byte) state);
		else if (device == enums::_ControlDeviceMouse) {
			if (type == enums::_ControlTypeButton) SetMouseButtonState((enums::MouseButton) index, (byte) state);
			else if (type == enums::_ControlTypeAxis) SetMouseAxisState((enums::MouseAxis) index, state);
		} else if (device == enums::_ControlDeviceGamepad) {
			if (type == enums::_ControlTypeButton) SetGamepadButtonState((enums::GamepadButton) index, (byte) state);
			else if (type == enums::_ControlTypeAxis) SetGamepadAxisState((enums::GamepadAxis) index, (sbyte) state);
			else if (type == enums::_ControlTypeDpad) SetGamepadDpadState((enums::GamepadDpad) index, state);
		}
	}

	int16 SettingsGetDevice(enums::PlayerControl control) { return GET_PTR2(Settings)[control].Device; }

	int16 SettingsGetType(enums::PlayerControl control) { return GET_PTR2(Settings)[control].Type; }

	int16 SettingsGetIndex(enums::PlayerControl control) { return GET_PTR2(Settings)[control].Index; }

	int16 SettingsGetDirection(enums::PlayerControl control) { return GET_PTR2(Settings)[control].Direction; }

	byte GetKeyState(enums::key_code key) { return GET_PTR2(ControlState)->Keys[key]; }

	void SetKeyState(enums::key_code key, byte state) { GET_PTR2(ControlState)->Keys[key] = state; }

	byte GetMouseButtonState(enums::MouseButton button) { return GET_PTR2(ControlState)->MouseButton[button]; }

	void SetMouseButtonState(enums::MouseButton button, byte state) { GET_PTR2(ControlState)->MouseButton[button] = state; }

	int32 GetMouseAxisState(enums::MouseAxis axis) { return GET_PTR2(ControlState)->MouseAxis[axis]; }

	void SetMouseAxisState(enums::MouseAxis axis, int32 state) { GET_PTR2(ControlState)->MouseAxis[axis] = state; }

	int32 GetMousePositionState(enums::MouseAxis axis) {
		if (axis > enums::_MouseAxisY) return 0;
		return GET_PTR2(MousePositionState)->Position[axis];
	}

	void SetMousePositionState(enums::MouseAxis axis, int32 position) {
		if (axis > enums::_MouseAxisY) return;
		GET_PTR2(MousePositionState)->Position[axis] = position;
	}

	byte GetGamepadButtonState(enums::GamepadButton button) { return GET_PTR2(ControlState)->GamepadButton[button]; }

	void SetGamepadButtonState(enums::GamepadButton button, byte state) { GET_PTR2(ControlState)->GamepadButton[button] = state; }

	sbyte GetGamepadAxisState(enums::GamepadAxis axis) { return GET_PTR2(ControlState)->GamepadAxis[axis].Direction; }

	void SetGamepadAxisState(enums::GamepadAxis axis, sbyte state) {
		GET_PTR2(ControlState)->GamepadAxis[axis].Direction = state;
		GET_PTR2(ControlState)->GamepadAxis[axis].Speed = 0;
	}

	int32 GetGamepadDpadState(enums::GamepadDpad dpad) { return GET_PTR2(ControlState)->GamepadDpad[dpad]; }

	void SetGamepadDpadState(enums::GamepadDpad dpad, int32 state) { GET_PTR2(ControlState)->GamepadDpad[dpad] = state; }
};

