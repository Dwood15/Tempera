#pragma once

#include <precompile.h>

namespace Yelo
{
	namespace Enums
	{
		enum {
			_button_none = NONE,

			_button_jump,
			_button_switch_grenade,
			_button_action,
			_button_switch_weapon,
			_button_melee,
			_button_flashlight,
			_button_throw_grenade,
			_button_fire,
			_button_accept,
			_button_back,
			_button_crouch,
			_button_zoom,
			_button_show_scores,
			_button_reload,
			_button_pickup,
			_button_say,
			_button_say_to_team,
			_button_say_to_vehicle,
			_button_screenshot,

			k_number_of_action_control_buttons, // NUMBER_OF_ACTION_CONTROL_BUTTONS

			_control_button_move_forward = k_number_of_action_control_buttons,
			_control_button_move_backward,
			_control_button_move_left,
			_control_button_move_right,
			_control_button_look_up,
			_control_button_look_down,
			_control_button_look_left,
			_control_button_look_right,

			_new_control_button_show_rules,
			_new_control_button_show_homies,

			k_number_of_total_control_buttons, // NUMBER_OF_TOTAL_CONTROL_BUTTONS
			k_first_new_control_button =		// FIRST_NEW_CONTROL_BUTTON
			_new_control_button_show_rules,
		};

		enum {
			_gamepad_a_button,
			_gamepad_b_button,
			_gamepad_x_button,
			_gamepad_y_button,
			_gamepad_black_button,
			_gamepad_white_button,
			_gamepad_left_trigger,
			_gamepad_right_trigger,
			_gamepad_dpad_up,
			_gamepad_dpad_down,
			_gamepad_dpad_left,
			_gamepad_dpad_right,
			_gamepad_start_button,
			_gamepad_back_button,
			_gamepad_left_thumb,
			_gamepad_right_thumb,

			k_number_of_gamepad_buttons // NUMBER_OF_GAMEPAD_BUTTONS
		};
		enum {
			_gamepad_left_stick,
			_gamepad_right_stick,

			k_number_of_gamepad_sticks // NUMBER_OF_GAMEPAD_STICKS
		};
	};

	namespace Enums
	{
		enum ControlDevice
		{
			_ControlDeviceUndefined,
			_ControlDeviceKeyboard,
			_ControlDeviceMouse,
			_ControlDeviceGamepad,
			_ControlDevice,
		};

		enum ControlType
		{
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

		enum GamepadAxis
		{
			_GamepadAxisRightY,
			_GamepadAxisRightX,
			_GamepadAxisLeftY,
			_GamepadAxisLeftX,
			_GamepadAxis,
		};

		enum GamepadDpad
		{
			_GamepadDpad1,
			_GamepadDpad2,
			_GamepadDpad3,
			_GamepadDpad4,
			_GamepadDpad,
		};

		enum GamepadAxisDirection
		{
			_GamepadAxisDirectionNone,
			_GamepadAxisDirectionDown,
			_GamepadAxisDirectionRight = _GamepadAxisDirectionDown,
			_GamepadAxisDirectionUp,
			_GamepadAxisDirectionLeft = _GamepadAxisDirectionUp,
			_GamepadAxisDirection,
		};

		enum GamepadDpadDirection // TODO: transition to pov_direction (input_windows) and remove
		{
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

		enum MouseButton // TODO: transition to mouse_button (input_windows) and remove
		{
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

		enum MouseAxis // TODO: transition to mouse_axes (input_windows) and remove
		{
			_MouseAxisX,
			_MouseAxisY,
			_MouseAxisWheel,
			_MouseAxis,
		};

		enum MouseAxisDirection
		{
			_MouseAxisDirectionNone,
			_MouseAxisDirectionUp,
			_MouseAxisDirectionRight = _MouseAxisDirectionUp,
			_MouseAxisDirectionDown,
			_MouseAxisDirectionLeft = _MouseAxisDirectionDown,
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

		enum KeyModifier
		{
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
