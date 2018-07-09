#pragma once

#include "../../cseries/base.h"

namespace Yelo {
	namespace GameState {
		struct s_game_time_globals {
			bool initialized;         // 0x0
			bool active;            // 0x1
			bool paused;            // 0x2
			unsigned char padA;
			short timePadUnkA;      // 0x4
			short timePadUnkB;      // 0x6
			short timePadUnkC;      // 0x8
			short timePadUnkD;      // 0x8
			uint game_time;      // 0xC
			ushort elapsed_time;   // 0x10
			uint server_time;   // 0x14
			real game_speed;         // 0x18
			real leftover_time_sec;      // 0x1C
		}; static_assert(sizeof(s_game_time_globals) == 0x20);
	};
};
