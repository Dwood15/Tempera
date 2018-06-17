#pragma once

#include "../ceinternal.h"
#include "../enums/ControlTypes.h"


//I don't understand how this works and I'm lazy so I'm just going to straight copy it in for now.

// Pad a structure using a byte[] field named pad[num] by [count] bytes
#define PAD(num, count) byte pad##num##[ count ]
namespace input {

	long GetControlState(short device, short type, short index, short direction);

	void SetControlState(short device, short type, short index, long state);

	namespace enums {
		enum ControlDevice {
			_ControlDeviceUndefined,
			_ControlDeviceKeyboard,
			_ControlDeviceMouse,
			_ControlDeviceGamepad,
			_ControlDevice,
		};

		enum ControlType {
			_ControlTypeButton,
			_ControlTypeAxis,
			_ControlTypeDpad,
			_ControlType,
		};

		enum GamepadButton // k_maximum_gamepad_buttons
		{
			_GamepadButton1,
			_GamepadButton2,
			_GamepadButton3,
			_GamepadButton4,
			_GamepadButton5,
			_GamepadButton6,
			_GamepadButton7,
			_GamepadButton8,
			_GamepadButton9,
			_GamepadButton10,
			_GamepadButton11,
			_GamepadButton12,
			_GamepadButton13,
			_GamepadButton14,
			_GamepadButton15,
			_GamepadButton16,
			_GamepadButton17,
			_GamepadButton18,
			_GamepadButton19,
			_GamepadButton20,
			_GamepadButton21,
			_GamepadButton22,
			_GamepadButton23,
			_GamepadButton24,
			_GamepadButton25,
			_GamepadButton26,
			_GamepadButton27,
			_GamepadButton28,
			_GamepadButton29,
			_GamepadButton30,
			_GamepadButton31,
			_GamepadButton32,
			_GamepadButton,
		};

		enum GamepadAxis {
			_GamepadAxisRightY,
			_GamepadAxisRightX,
			_GamepadAxisLeftY,
			_GamepadAxisLeftX,
			_GamepadAxis,
		};

		enum GamepadDpad {
			_GamepadDpad1,
			_GamepadDpad2,
			_GamepadDpad3,
			_GamepadDpad4,
			_GamepadDpad,
		};

		enum GamepadAxisDirection {
			_GamepadAxisDirectionNone,
			_GamepadAxisDirectionDown,
			_GamepadAxisDirectionRight = _GamepadAxisDirectionDown,
			_GamepadAxisDirectionUp,
			_GamepadAxisDirectionLeft  = _GamepadAxisDirectionUp,
			_GamepadAxisDirection,
		};

		// TODO: transition to pov_direction (input_windows) and remove
		enum GamepadDpadDirection {
			_GamepadDpadDirectionNone = -1,
			_GamepadDpadDirectionNorth,
			_GamepadDpadDirectionNorthEast,
			_GamepadDpadDirectionEast,
			_GamepadDpadDirectionSouthEast,
			_GamepadDpadDirectionSouth,
			_GamepadDpadDirectionSouthWest,
			_GamepadDpadDirectionWest,
			_GamepadDpadDirectionNorthWest,
			_GamepadDpadDirection,
		};

		// TODO: transition to mouse_button (input_windows) and remove
		enum MouseButton {
			// Left
					_MouseButton1,
			// Middle
					_MouseButton2,
			// Right
					_MouseButton3,
			// Side back
					_MouseButton4,
			// Side forward
					_MouseButton5,
					_MouseButton6,
					_MouseButton7,
					_MouseButton8,
					_MouseButton,
		};

		// TODO: transition to mouse_axes (input_windows) and remove
		enum MouseAxis {
			_MouseAxisX,
			_MouseAxisY,
			_MouseAxisWheel,
			_MouseAxis,
		};

		enum MouseAxisDirection {
			_MouseAxisDirectionNone,
			_MouseAxisDirectionUp,
			_MouseAxisDirectionRight = _MouseAxisDirectionUp,
			_MouseAxisDirectionDown,
			_MouseAxisDirectionLeft  = _MouseAxisDirectionDown,
			_MouseAxisDirection,
		};

		enum PlayerControl // TODO: transition to control_buttons (input_windows) and remove
		{
			_PlayerControlJump,
			_PlayerControlSwitchGrenade,
			_PlayerControlAction,
			_PlayerControlSwitchWeapon,
			_PlayerControlMelee,
			_PlayerControlFlashlight,
			_PlayerControlThrowGrenade,
			_PlayerControlFire,
			_PlayerControlAccept,
			_PlayerControlBack,
			_PlayerControlCrouch,
			_PlayerControlZoom,
			_PlayerControlShowScores,
			_PlayerControlReload,
			_PlayerControlPickup,
			_PlayerControlSay,
			_PlayerControlSayToTeam,
			_PlayerControlSayToVehicle,
			_PlayerControlScreenshot,
			_PlayerControlMoveForward,
			_PlayerControlMoveBackward,
			_PlayerControlMoveLeft,
			_PlayerControlMoveRight,
			_PlayerControlLookUp,
			_PlayerControlLookDown,
			_PlayerControlLookLeft,
			_PlayerControlLookRight,
			_PlayerControlShowRules,
			_PlayerControlShowHomies,
			_PlayerControl,
		};

		enum KeyModifier {
			_KeyModifierNone,
			_KeyModifierShift,
			_KeyModifierCtrl,
			_KeyModifierAlt,
			_KeyModifierShiftCtrl,
			_KeyModifierShiftAlt,
			_KeyModifierCtrlAlt,
			_KeyModifierShiftCtrlAlt,
			_KeyModifier,
		};
	};
};

namespace Flags {
	enum input_state {
		_input_state_enabled_bit,
		_input_state_menu_bit,
		_input_state_chat_bit,
	};
};

namespace players {
	enum input_state_flags {
		_input_state_enabled_bit,
		_input_state_menu_bit,
		_input_state_chat_bit,
	};

	static auto *const InputStateFlags    = (reinterpret_cast<byte *>(0x6AD572));


	struct ControlStates {
		// key states (how long its been pressed until 0xFF, 0 if not pressed)
		byte Keys[enums::k_number_of_keys];

		byte pad0[sizeof(byte) * 383];

		int32 MouseAxis[enums::_MouseAxis];
		byte  MouseButton[enums::_MouseButton];

		byte pad1[sizeof(byte) * 4680];

		byte             GamepadButton[enums::_GamepadButton];
		Input::GamepadAxisState GamepadAxis[enums::_GamepadAxis];

		byte pad2[sizeof(byte) * 56];

		int32 GamepadDpad[enums::_GamepadDpad];
	};

	static auto *const ControlState       = (reinterpret_cast<ControlStates *>(0x64C550));

	struct PositionState {
		//PAD16;
		//bool Moving; // true during mouse movement
		//PAD8;
		long pad_a;
		long Position[2]; // menu space coordinates (0,0) to (640,480)
	};
	static auto *const MousePositionState = (reinterpret_cast<Input::PositionState *>(0x6B4008));

};
