#pragma once
#include <array>
#include <precompile.h>
// player_ui_get_single_player_local_player_from_controller
//__int16 * player_ui_globals_single_player_local_player_controllers = (__int16 *)0x6AFE26;

__int16 player_ui_globals_single_player_local_player_controllers[MAX_PLAYER_COUNT_LOCAL];

std::array<uintptr_t, 12> references = {
	0x4791B3,

	//player_ui_initialize
	0x49830C,
	//player_ui_get_single_player_local_player_from_controller
	0x498467

};


signed __int16 __fastcall player_ui_get_single_player_local_player_from_controller(__int16 player) {
	__int16 result = 0;

	while ( player_ui_globals_single_player_local_player_controllers[result] != player )
	{
		if ( ++result >= MAX_PLAYER_COUNT_LOCAL )
			return -1;
	}
	return result;
}

