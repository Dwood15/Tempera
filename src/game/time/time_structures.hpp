#pragma once

#include "macros_generic.h"

namespace Yelo::GameState {
		struct s_game_time_globals {
			bool initialized;         // 0x0
			bool active;            // 0x1
			bool paused;            // 0x2
			unsigned char padA;      //0X3
			short timePadUnkA;      // 0x4
			short timePadUnkB;      // 0x6
			short timePadUnkC;      // 0x8
			short timePadUnkD;      // 0xA
			int game_time;      // 0xC
			short elapsed_time;   // 0x10
			short : 16;
			uint server_time;   // 0x14
			real game_speed;         // 0x18
			real leftover_time_sec;      // 0x1C
		};
		STAT_ASSERT(s_game_time_globals, 0x20);
	};
