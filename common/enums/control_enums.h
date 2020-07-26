#pragma once
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <macros_generic.h>

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

	// k_maximum_gamepad_buttons
	enum GamepadButton {
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

	// TODO: transition to control_buttons (input_windows) and remove
	enum PlayerControl {
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

	enum {
		k_maximum_buffered_keystrokes = 64, // MAXIMUM_BUFFERED_KEYSTROKES

		k_maximum_gamepads             = 4,
		k_maximum_gamepad_axes         = 32,
		k_maximum_gamepad_buttons      = 32,
		k_maximum_gamepad_povs         = 16,
		k_maximum_enumerated_joysticks = 8,

		k_number_of_virtual_codes = 256,
	};

	enum {
		_axis_direction_positive,
		_axis_direction_negative,

		k_number_of_axis_directions, // NUMBER_OF_AXIS_DIRECTIONS
	};

	enum {
		_pov_direction_none = NONE,

		_pov_direction_north,
		_pov_direction_north_east,
		_pov_direction_east,
		_pov_direction_south_east,
		_pov_direction_south,
		_pov_direction_south_west,
		_pov_direction_west,
		_pov_direction_north_west,

		k_number_of_pov_directions = 8, // NUMBER_OF_POV_DIRECTIONS
	};

	enum {
		_mouse_button_left,
		_mouse_button_middle,
		_mouse_button_right,
		_mouse_button4,
		_mouse_button5,
		_mouse_button6,
		_mouse_button7,
		_mouse_button8,

		k_number_of_mouse_buttons, // NUMBER_OF_MOUSE_BUTTONS
	};

	enum {
		_mouse_axes_horiz,
		_mouse_axes_vert,
		_mouse_axes_wheel,

		k_number_of_mouse_axes, // NUMBER_OF_MOUSE_AXES
	};

	enum key_code : short {
		_key_code_invalid = NONE,

		_key_code_escape        = 0x0,
		_key_code_f1            = 0x1,
		_key_code_f2            = 0x2,
		_key_code_f3            = 0x3,
		_key_code_f4            = 0x4,
		_key_code_f5            = 0x5,
		_key_code_f6            = 0x6,
		_key_code_f7            = 0x7,
		_key_code_f8            = 0x8,
		_key_code_f9            = 0x9,
		_key_code_f10           = 0xA,
		_key_code_f11           = 0xB,
		_key_code_f12           = 0xC,
		_key_code_print_screen  = 0xD,
		_key_code_scroll_lock   = 0xE,
		_key_code_pause         = 0xF,
		_key_code_tilde         = 0x10,
		_key_code_1             = 0x11,
		_key_code_2             = 0x12,
		_key_code_3             = 0x13,
		_key_code_4             = 0x14,
		_key_code_5             = 0x15,
		_key_code_6             = 0x16,
		_key_code_7             = 0x17,
		_key_code_8             = 0x18,
		_key_code_9             = 0x19,
		_key_code_0             = 0x1A,
		_key_code_minus         = 0x1B,
		_key_code_plus          = 0x1C,
		_key_code_backspace     = 0x1D,
		_key_code_tab           = 0x1E,
		_key_code_Q             = 0x1F,
		_key_code_W             = 0x20,
		_key_code_E             = 0x21,
		_key_code_R             = 0x22,
		_key_code_T             = 0x23,
		_key_code_Y             = 0x24,
		_key_code_U             = 0x25,
		_key_code_I             = 0x26,
		_key_code_O             = 0x27,
		_key_code_P             = 0x28,
		_key_code_left_bracket  = 0x29,
		_key_code_right_bracket = 0x2A,
		_key_code_pipe          = 0x2B,
		_key_code_caps          = 0x2C,
		_key_code_A             = 0x2D,
		_key_code_S             = 0x2E,
		_key_code_D             = 0x2F,
		_key_code_F             = 0x30,
		_key_code_G             = 0x31,
		_key_code_H             = 0x32,
		_key_code_J             = 0x33,
		_key_code_K             = 0x34,
		_key_code_L             = 0x35,
		_key_code_colon         = 0x36,
		_key_code_quote         = 0x37,
		_key_code_enter         = 0x38,
		_key_code_left_shift    = 0x39,
		_key_code_Z             = 0x3A,
		_key_code_X             = 0x3B,
		_key_code_C             = 0x3C,
		_key_code_V             = 0x3D,
		_key_code_B             = 0x3E,
		_key_code_N             = 0x3F,
		_key_code_M             = 0x40,
		_key_code_comma         = 0x41,
		_key_code_period        = 0x42,
		_key_code_slash         = 0x43,
		_key_code_right_shift   = 0x44,
		_key_code_left_ctrl     = 0x45,
		_key_code_left_windows  = 0x46,
		_key_code_left_alt      = 0x47,
		_key_code_space         = 0x48,
		_key_code_right_alt     = 0x49,
		_key_code_right_windows = 0x4A,
		_key_code_apps_menu     = 0x4B,
		_key_code_right_ctrl    = 0x4C,
		_key_code_up            = 0x4D,
		_key_code_down          = 0x4E,
		_key_code_left          = 0x4F,
		_key_code_right         = 0x50,
		_key_code_insert        = 0x51,
		_key_code_home          = 0x52,
		_key_code_page_up       = 0x53,
		_key_code_delete        = 0x54,
		_key_code_end           = 0x55,
		_key_code_page_down     = 0x56,
		_key_code_nums_lock     = 0x57,
		_key_code_num_divide    = 0x58,
		_key_code_num_multiply  = 0x59,
		_key_code_Num0          = 0x5A,
		_key_code_Num1          = 0x5B,
		_key_code_Num2          = 0x5C,
		_key_code_Num3          = 0x5D,
		_key_code_Num4          = 0x5E,
		_key_code_Num5          = 0x5F,
		_key_code_Num6          = 0x60,
		_key_code_Num7          = 0x61,
		_key_code_Num8          = 0x62,
		_key_code_Num9          = 0x63,
		_key_code_num_minus     = 0x64,
		_key_code_num_add       = 0x65,
		_key_code_num_enter     = 0x66,
		_key_code_num_dot       = 0x67,
		_key_code_68            = 0x68, // DIK_ABNT_C2
		_key_code_69            = 0x69, // DIK_AT
		_key_code_6A            = 0x6A, // DIK_COLON
		_key_code_6B            = 0x6B, // DIK_PREVTRACK
		_key_code_6C            = 0x6C, // OEM_102
		k_number_of_keys        = 0x6D,

		_key_code_shift   = 0x6E,
		_key_code_ctrl    = 0x6F,
		_key_code_windows = 0x70,
		_key_code_alt     = 0x71,
	};

	namespace Flags {
		enum buffered_key_flags : unsigned char {
			_buffered_key_shift_bit,   // SHIFT key pressed
			_buffered_key_control_bit,   // CTRL key pressed
			_buffered_key_alt_bit,      // ALT key pressed
		};
	};

	namespace Input {
		struct s_buffered_key {
			Flags::buffered_key_flags flags;
			byte                      state;         // how long its been pressed until 0xFF, 0 if not pressed
			enums::key_code           key_code;
		};
		static_assert(sizeof(s_buffered_key) == 0x4);
	};

	namespace blam {
		bool __cdecl  input_key_is_down(short key_code);
	};

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
		k_first_new_control_button   =      // FIRST_NEW_CONTROL_BUTTON
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

namespace enums {
	//_unit_animation_state_asleep
	//_unit_animation_state_alert
	//_unit_animation_state_alert2
	//_unit_animation_state_stand
	//_unit_animation_state_stand2
	//_unit_animation_state_flee
	//_unit_animation_state_flaming
	//NUMBER_OF_UNIT_ANIMATION_STATES = 7

	enum {
		_unit_aiming_speed_alert,
		_unit_aiming_speed_casual, // infection forms use this

		k_number_of_unit_aiming_speeds // NUMBER_OF_UNIT_AIMING_SPEEDS
	};
};

namespace flags {
	enum unit_control_flags : unsigned short {
		_unit_control_crouch_bit,
		_unit_control_jump_bit,
		_unit_control_user1_bit,
		_unit_control_user2_bit,
		_unit_control_light_bit,
		_unit_control_exact_facing_bit,
		_unit_control_action_bit,
		_unit_control_melee_bit,//_unit_control_equipment_bit, // debug strings still list this as 'equipment', but usage is melee

		_unit_control_look_dont_turn_bit,
		_unit_control_force_alert_bit,
		_unit_control_reload_bit,
		_unit_control_primary_trigger_bit,
		_unit_control_secondary_trigger_bit,
		_unit_control_grenade_bit,
		_unit_control_swap_weapon_bit,

		k_number_of_unit_control_flags // NUMBER_OF_UNIT_CONTROL_FLAGS

	};
	// static_assert( k_number_of_unit_control_flags_yelo <= BIT_COUNT());
};
