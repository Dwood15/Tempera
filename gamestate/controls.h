#pragma once

#include "../enums/ControlTypes.h"
#include "../headers/macros_generic.h"
#include "../ce_base_types.h"

namespace Input {
	void Initialize();

	void Dispose();

	void Update();

	struct ControlSettings {
		bool Initialized;
		char : 8;
		int16 Device; // ControlDevice
		PAD16;
		int16 Type; // ControlType
		int16 Index; // Key / GamepadButton / GamepadAxis / MouseButton / MouseAxis
		int16 Direction; // GamepadAxisDirection / GamepadDpadDirection / MouseAxisDirection
	};

	struct GamepadAxisState {
		byte  Speed; // not even used
		sbyte Direction; // -16 to 16, screen coords
	};

	int16 SettingsGetDevice(enums::PlayerControl control);

	int16 SettingsGetType(enums::PlayerControl control);

	int16 SettingsGetIndex(enums::PlayerControl control);

	int16 SettingsGetDirection(enums::PlayerControl control);

	// device = [enums::_ControlDeviceKeyboard]
	//	* type = UNUSED
	//	* index = [enums::Key]
	//	* direction = UNUSED
	//
	// device = [enums::_ControlDeviceMouse]
	//	* type = [enums::_ControlTypeButton]
	//		- index = [enums::MouseButton]
	//		- direction = UNUSED
	//	* type = [enums::_ControlTypeAxis]
	//		- index = [enums::MouseAxis]
	//		- direction = [enums::MouseAxisDirection]
	//
	// device = [enums::_ControlDeviceGamepad]
	//	* type = enums::_ControlTypeButton
	//		- index = [enums::GamepadButton]
	//		- direction = UNUSED
	//	* type = enums::_ControlTypeAxis
	//		- index = [enums::GamepadAxis]
	//		- direction = [enums::GamepadAxisDirection]
	//	* type = enums::_ControlTypeDpad
	//		- index = [enums::GamepadDpad]
	//		- direction = [enums::GamepadDpadDirection]

	int32 GetControlState(int16 device, int16 type, int16 index, int16 direction);

	void SetControlState(int16 device, int16 type, int16 index, int32 state);

	// Returns >0 if [control] is active on any input devices currently being used
	// API_INLINE int32 GetControlState(enums::PlayerControl control) { return GetControlState(SettingsGetDevice(control), SettingsGetType(control), SettingsGetIndex(control), SettingsGetDirection(control) );
}

// Sets the state of [control] on all currently active input devices
// API_INLINE void SetControlState(enums::PlayerControl control, int32 state) { SetControlState(SettingsGetDevice(control), SettingsGetType(control), SettingsGetIndex(control), state); }
//
// // Returns true if [control] is assigned to the keyboard
// API_INLINE bool IsKeyboardControl(enums::PlayerControl control) { return SettingsGetDevice(control) == enums::_ControlDeviceKeyboard; }
//
// // Returns true if [control] is assigned to the mouse
// API_INLINE bool IsMouseControl(enums::PlayerControl control) { return SettingsGetDevice(control) == enums::_ControlDeviceMouse; }
//
// // Returns true if [control] is assigned to the gamepad
// API_INLINE bool IsGamepadControl(enums::PlayerControl control) { return SettingsGetDevice(control) == enums::_ControlDeviceGamepad; }

// Gets the current state of [key]
// The larger the return value, the longer its been held down
byte GetKeyState(enums::key_code key);

// The larger the value of [state], the longer its been held down
void SetKeyState(enums::key_code key, byte state);

// Gets the current state of mouse [button]
// The larger the return value, the longer its been held down
byte GetMouseButtonState(enums::MouseButton button);

// Sets the current state of mouse [button]
// The larger the value of [state], the longer its been held down
void SetMouseButtonState(enums::MouseButton button, byte state);

// Gets the current state of mouse [axis]
// The larger the return value, the longer its been held down
int32 GetMouseAxisState(enums::MouseAxis axis);

// Sets the current state of mouse [axis]
// The larger the value of [state], the longer its been held down
void SetMouseAxisState(enums::MouseAxis axis, int32 state);

// Gets the current state of gamepad [button]
// The larger the return value, the longer its been held down
byte GetGamepadButtonState(enums::GamepadButton button);

// Sets the current state of gamepad [button]
// The larger the value of [state], the longer its been held down
void SetGamepadButtonState(enums::GamepadButton button, byte state);

// Gets the current state of gamepad [axis]
// The larger the return value, the longer its been held down
sbyte GetGamepadAxisState(enums::GamepadAxis axis);

// Sets the current state of gamepad [axis]
// The larger the value of [state], the longer its been held down
void SetGamepadAxisState(enums::GamepadAxis axis, sbyte state);

// Gets the current state of [dpad]
// The larger the return value, the longer its been held down
int32 GetGamepadDpadState(enums::GamepadDpad dpad);

// Sets the current state of [dpad]
// The larger the value of [state], the longer its been held down
void SetGamepadDpadState(enums::GamepadDpad dpad, int32 state);

};

struct s_unit_control_data {
	byte           animation_state;
	byte           aiming_speed;
	unsigned short control_flags;
	int16          weapon_index;
	int16          grenade_index;
	int16          zoom_index;
	PAD16;
	real_vector3d throttle;
	real          primary_trigger;
	real_vector3d facing_vector, aiming_vector, looking_vector;
}; static_assert(sizeof(s_unit_control_data) == 0x40, STATIC_ASSERT_FAIL);
