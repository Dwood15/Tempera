#pragma once

#include <precompile.h>
#include <d3dx9math.inl>
#include "window.h"
#include "abstraction.h"
#include <memory_locations.h>

namespace Yelo::Flags {
	enum input_state {
		_input_state_enabled_bit,
		_input_state_menu_bit,
		_input_state_chat_bit,
	};
};

namespace Yelo::Input {
	struct GamepadAxisState {
		byte  Speed; // not even used
		sbyte Direction; // -16 to 16, screen coords
	};

	struct ControlStates {
		// key states (how long its been pressed until 0xFF, 0 if not pressed)
		byte Keys[Enums::k_number_of_keys];

		PAD(0, sizeof(byte) * 383);

		long MouseAxis[Enums::_MouseAxis];
		byte MouseButton[Enums::_MouseButton];

		PAD(1, sizeof(byte) * 4680);

		byte             GamepadButton[Enums::_GamepadButton];
		GamepadAxisState GamepadAxis[Enums::_GamepadAxis];

		PAD(2, sizeof(byte) * 56);

		long GamepadDpad[Enums::_GamepadDpad];
	};

	struct PositionState {
		//unsigned short : 16;  //0x6B4008
		//bool Moving; // true during mouse movement  //0x6B400A
		//unsigned char : 8;   								//0x6B400B
		unsigned long : 32;
		long PositionX; // menu space coordinates (0,0) to (640,480) 0x6B4010
		long PositionY; //0x6B4014
	};

	// #include "Memory/_EngineLayout.inl"
};

namespace Yelo::Input {
	void Initialize() {
		//Memory::CreateHookRelativeCall(&Input::Update, GET_FUNC_VPTR(INPUT_UPDATE_HOOK), Enums::_x86_opcode_ret);
	}

	void Dispose() {}

	void Update() {}

	struct ControlSettings {
		bool Initialized;
		unsigned char : 8;
		short Device; // ControlDevice
		unsigned short : 16;
		short Type; // ControlType
		short Index; // Key / GamepadButton / GamepadAxis / MouseButton / MouseAxis
		short Direction; // GamepadAxisDirection / GamepadDpadDirection / MouseAxisDirection
	};

	bool IsInGame() { return TEST_FLAG(*InputStateFlags, Flags::_input_state_enabled_bit); }

	bool IsInMenu() { return TEST_FLAG(*InputStateFlags, Flags::_input_state_menu_bit); }

	bool IsInChat() { return TEST_FLAG(*InputStateFlags, Flags::_input_state_chat_bit); }

	void AllowMovement(bool allow) { SET_FLAG(*InputStateFlags, Flags::_input_state_enabled_bit, allow); }

	short SettingsGetDevice(Enums::PlayerControl control) { return Settings[control].Device; }

	short SettingsGetType(Enums::PlayerControl control) { return Settings[control].Type; }

	short SettingsGetIndex(Enums::PlayerControl control) { return Settings[control].Index; }

	short SettingsGetDirection(Enums::PlayerControl control) { return Settings[control].Direction; }

	// device = [Enums::_ControlDeviceKeyboard]
	//	* type = UNUSED
	//	* index = [Enums::Key]
	//	* direction = UNUSED
	//
	// device = [Enums::_ControlDeviceMouse]
	//	* type = [Enums::_ControlTypeButton]
	//		- index = [Enums::MouseButton]
	//		- direction = UNUSED
	//	* type = [Enums::_ControlTypeAxis]
	//		- index = [Enums::MouseAxis]
	//		- direction = [Enums::MouseAxisDirection]
	//
	// device = [Enums::_ControlDeviceGamepad]
	//	* type = Enums::_ControlTypeButton
	//		- index = [Enums::GamepadButton]
	//		- direction = UNUSED
	//	* type = Enums::_ControlTypeAxis
	//		- index = [Enums::GamepadAxis]
	//		- direction = [Enums::GamepadAxisDirection]
	//	* type = Enums::_ControlTypeDpad
	//		- index = [Enums::GamepadDpad]
	//		- direction = [Enums::GamepadDpadDirection]

	// Returns true if [control] is assigned to the keyboard
	inline bool IsKeyboardControl(Enums::PlayerControl control) { return SettingsGetDevice(control) == Enums::_ControlDeviceKeyboard; }

	// Returns true if [control] is assigned to the mouse
	inline bool IsMouseControl(Enums::PlayerControl control) { return SettingsGetDevice(control) == Enums::_ControlDeviceMouse; }

	// Returns true if [control] is assigned to the gamepad
	inline bool IsGamepadControl(Enums::PlayerControl control) { return SettingsGetDevice(control) == Enums::_ControlDeviceGamepad; }

	// Gets the current state of [key]
	// The larger the return value, the longer its been held down
	byte GetKeyState(Enums::key_code key) { return ControlState->Keys[key]; }

	// The larger the value of [state], the longer its been held down
	void SetKeyState(Enums::key_code key, byte state) { ControlState->Keys[key] = state; }

	// Gets the current state of mouse [button]
	// The larger the return value, the longer its been held down
	byte GetMouseButtonState(Enums::MouseButton button) { return ControlState->MouseButton[button]; }

	// Sets the current state of mouse [button]
	// The larger the value of [state], the longer its been held down
	void SetMouseButtonState(Enums::MouseButton button, byte state) { ControlState->MouseButton[button] = state; }

	// Gets the current state of mouse [axis]
	// The larger the return value, the longer its been held down
	long GetMouseAxisState(Enums::MouseAxis axis) { return ControlState->MouseAxis[axis]; }

	// Sets the current state of mouse [axis]
	// The larger the value of [state], the longer its been held down
	void SetMouseAxisState(Enums::MouseAxis axis, long state) { ControlState->MouseAxis[axis] = state; }

	long GetMousePositionState(Enums::MouseAxis axis) {
		if (axis == Enums::_MouseAxisX) {
			return MousePositionState->PositionX;
		}

		if (axis == Enums::_MouseAxisY) {
			return MousePositionState->PositionY;
		}

		return 0;
	}

	void SetMousePositionState(Enums::MouseAxis axis, long position) {
		if (axis == Enums::_MouseAxisX) {
			MousePositionState->PositionX = position;
		}

		if (axis == Enums::_MouseAxisY) {
			MousePositionState->PositionY = position;
		}

		return;
	}

	// Gets the current state of gamepad [button]
	// The larger the return value, the longer its been held down
	byte GetGamepadButtonState(Enums::GamepadButton button) { return ControlState->GamepadButton[button]; }

	// Sets the current state of gamepad [button]
	// The larger the value of [state], the longer its been held down
	void SetGamepadButtonState(Enums::GamepadButton button, byte state) { ControlState->GamepadButton[button] = state; }

	// Gets the current state of gamepad [axis]
	// The larger the return value, the longer its been held down
	sbyte GetGamepadAxisState(Enums::GamepadAxis axis) { return ControlState->GamepadAxis[axis].Direction; }

	// Sets the current state of gamepad [axis]
	// The larger the value of [state], the longer its been held down
	void SetGamepadAxisState(Enums::GamepadAxis axis, sbyte state) {
		ControlState->GamepadAxis[axis].Direction = state;
		ControlState->GamepadAxis[axis].Speed     = 0;
	}

	// Gets the current state of [dpad]
	// The larger the return value, the longer its been held down
	long GetGamepadDpadState(Enums::GamepadDpad dpad) {
		return ControlState->GamepadDpad[dpad];
	}

	// Sets the current state of [dpad]
	// The larger the value of [state], the longer its been held down
	void SetGamepadDpadState(Enums::GamepadDpad dpad, long state) {
		ControlState->GamepadDpad[dpad] = state;
	}

	// Returns the string version of [value]
	const char * ControlTypeToString(Enums::ControlType value) {
		static const char * string_list[] = {
			"button",
			"axis",
			"pov",
		};

		return string_list[value];
	}

	// Returns the string version of [value]
	const char * GamepadDpadDirectionToString(Enums::GamepadDpadDirection value) {
		static const char * string_list[Enums::k_number_of_pov_directions] = {
			"north",
			"northeast",
			"east",
			"southeast",
			"south",
			"southwest",
			"west",
		};

		return string_list[value];
	}

	// Returns the string version of [value]
	const char * PlayerControlToString(Enums::PlayerControl value) {
		static const char * string_list[Enums::k_number_of_total_control_buttons] = {
			"jump",
			"switch_grenade",
			"action",
			"switch_weapon",
			"melee",
			"flashlight",
			"throw_grenade",
			"fire",
			"accept",
			"back",
			"crouch",
			"zoom",
			"showscores",
			"reload",
			"exchange_weapon",
			"say",
			"sayteam",
			"sayvehicle",
			"screenshot",
			"forward",
			"backward",
			"left",
			"right",
			"look_up",
			"look_down",
			"look_left",
			"look_right",
			"showrules",
			"show_homies",
		};

		return string_list[value];
	}

	long GetControlState(short device, short type, short index, short direction) {
		long state = 0;

		if (device == Enums::_ControlDeviceKeyboard) state = GetKeyState((Enums::key_code) index);
		else if (device == Enums::_ControlDeviceMouse) {
			if (type == Enums::_ControlTypeButton) state = GetMouseButtonState((Enums::MouseButton) index);
			else if (type == Enums::_ControlTypeAxis) state = GetMouseAxisState((Enums::MouseAxis) index);
		} else if (device == Enums::_ControlDeviceGamepad) {
			if (type == Enums::_ControlTypeButton) state = GetGamepadButtonState((Enums::GamepadButton) index);
			else if (type == Enums::_ControlTypeAxis) state = GetGamepadAxisState((Enums::GamepadAxis) index);
			else if (type == Enums::_ControlTypeDpad) state = GetGamepadDpadState((Enums::GamepadDpad) index);
		}

		// Check if direction matches setting
		if (type == Enums::_ControlTypeAxis && direction && state != (direction > 0 ? 1 : 2)) {
			state = 0;
		} else if (type == Enums::_ControlTypeDpad && state != direction) {
			state = 0;
		}

		return state;
	}

	void SetControlState(short device, short type, short index, long state) {
		if (device == Enums::_ControlDeviceKeyboard) {
			SetKeyState((Enums::key_code) index, (byte) state);
			return;
		}

		if (device == Enums::_ControlDeviceMouse) {
			switch(type) {
				case Enums::_ControlTypeButton:
					SetMouseButtonState((Enums::MouseButton) index, (byte) state);
					return;

				case Enums::_ControlTypeAxis:
					SetMouseAxisState((Enums::MouseAxis) index, state);
					return;

				default:
					return;
			}
		}

		if (device == Enums::_ControlDeviceGamepad) {
			switch(type) {
				case Enums::_ControlTypeButton:
					SetGamepadButtonState((Enums::GamepadButton) index, (byte) state);
					return;

				case Enums::_ControlTypeAxis:
					SetGamepadAxisState((Enums::GamepadAxis) index, (sbyte) state);
					return;

				case 	Enums::_ControlTypeDpad:
					SetGamepadDpadState((Enums::GamepadDpad) index, state);
					return;
			}
		}
	}

	// Returns >0 if [control] is active on any input devices currently being used
	inline long GetControlState(Enums::PlayerControl control) {
		return GetControlState(SettingsGetDevice(control), SettingsGetType(control), SettingsGetIndex(control), SettingsGetDirection(control));
	}

	// Sets the state of [control] on all currently active input devices
	inline void SetControlState(Enums::PlayerControl control, long state) {
		SetControlState(SettingsGetDevice(control), SettingsGetType(control), SettingsGetIndex(control), state);
	}
};
