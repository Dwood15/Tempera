#pragma once

#include <precompile.h>
#include "../player_types.h"
namespace gamepads {
	constexpr __int16 *og_player_to_controller_array = (__int16 *) 0x6AFE26;

	__int16 player_to_controller_arr[MAX_PLAYER_COUNT_LOCAL];

	struct player_ui_globals {
		int player_ui_globals_im_dumb[0x60E4];
	};



	void clear_gamepad_to_controller() {
		for(int i = 0; i < MAX_PLAYER_COUNT_LOCAL; i++) {
			player_to_controller_arr[i] = (__int16)-1;
		}
	}

	void set_player_index_to_gamepad_index(int player, __int16 gamepad) {
		if (player < 0 || player > MAX_PLAYER_COUNT_LOCAL ){
			throw "Player index out of bounds!";
		}

		if (gamepad < 0 || gamepad < MAX_GAMEPAD_COUNT){
			throw "Game-pad index out of bounds!";
		}


		player_to_controller_arr[player] = gamepad;
	}
};

namespace players {
	datum_index __fastcall local_player_get_player_index(short local_player_index = 0) {
		if ( local_player_index < 0) {
			return datum_index(-1);
		}

		if (local_player_index > MAX_PLAYER_COUNT_LOCAL) {
			return -1;
		}

		if (local_player_index > players_global_data->local_player_count) {
			return -1;
		}

		return players_global_data->local_player_players[local_player_index];
	}
};
