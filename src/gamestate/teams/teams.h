#pragma once

#include <macros_generic.h>
#include <enums/generic_enums.h>
#include "../../cseries/MacrosCpp.h"

namespace Yelo::GameState {
	struct s_allegiance {
		::game_team this_team;
		::game_team other_team;
		int16       threshold;
		unsigned short:16;   // 0x6
		unsigned char :8;      // 0x8
		unsigned char :8;      // 0x9
		bool is_broken;         // 0xA
		unsigned char:8;      // 0xB
		unsigned char:8;      // 0xC
		unsigned char:8;;
		int16 current_incidents;// 0xE
		unsigned short:16;   // 0x10
	};
	STAT_ASSERT(s_allegiance, 0x12);

	struct s_game_allegiance_globals {
		int16        allegiances_count;
		s_allegiance allegiances[8];
		short        padA;
		long_flags   ally_mapping_flags[((::k_number_of_game_teams * ::k_number_of_game_teams) + 31) >> 5]; // 0x94
		long_flags   enemy_mapping_flags[((::k_number_of_game_teams * ::k_number_of_game_teams) + 31) >> 5]; // 0xA4
	};
	STAT_ASSERT(s_game_allegiance_globals,
					0xB4);
};
