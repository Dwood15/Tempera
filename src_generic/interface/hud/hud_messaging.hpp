#include <precompile.h>
#pragma once

namespace Yelo
{
	namespace GameUI
	{
		struct s_hud_messaging : public TStruct< 1160 >
		{
		};
		s_hud_messaging*			HudMessaging();
	};

	namespace blam
	{
		void __cdecl hud_print_message(short local_player_index, wconst char * message);
	};
};
