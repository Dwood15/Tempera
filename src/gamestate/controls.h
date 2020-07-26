#pragma once

#include "units/unit_controls.h"

namespace Input {
	struct ControlSettings {
		bool  Initialized;
		char  pad0;
		int16 Device; // ControlDevice
		int16 pad1;
		int16 Type; // ControlType
		int16 Index; // Key / GamepadButton / GamepadAxis / MouseButton / MouseAxis
		int16 Direction; // GamepadAxisDirection / GamepadDpadDirection / MouseAxisDirection
	};
	static auto *const Settings = (reinterpret_cast<Input::ControlSettings *>(0x6AD802));

	struct GamepadAxisState {
		byte  Speed; // not even used
		sbyte Direction; // -16 to 16, screen coords
	};


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

	// Returns >0 if [control] is active on any input devices currently being used
	// API_INLINE int32 GetControlState(enums::PlayerControl control) { return GetControlState(SettingsGetDevice(control), SettingsGetType(control), SettingsGetIndex(control), SettingsGetDirection(control) );
}

struct s_unit_control_data;
namespace Control {
	extern void HandleActionOverride(ushort idx, s_unit_control_data * from);
	extern void UnitControl(ushort unit_idx, s_unit_control_data *from, int client_update_idx);
}
