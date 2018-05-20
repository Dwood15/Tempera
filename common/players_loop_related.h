#pragma once
// player_ui_get_single_player_local_player_from_controller

__int16 * player_ui_globals_single_player_local_player_controllers = (__int16 *)0x6AFE26;

signed __int16 __fastcall player_ui_get_single_player_local_player_from_controller(__int16 player) {
	__int16 result = 0;

	while ( player_ui_globals_single_player_local_player_controllers[result] != player )
	{
		if ( ++result >= 1 )
			return -1;
	}
	return result;
}

