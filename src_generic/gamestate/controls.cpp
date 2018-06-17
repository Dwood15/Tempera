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

		byte             GamepadButton[enums::_GamepadButton];
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
};

