#pragma once

namespace Yelo::AI
	{
		struct s_action_fight_state
		{
			UNKNOWN_TYPE(short); // timer
			unsigned short : 16;
		}; static_assert( sizeof(s_action_fight_state) == 0x4 );
	};
