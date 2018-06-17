/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <precompile.h>

namespace Yelo
{
	namespace Enums
	{
		enum hud_chat_type : signed long
		{
			_hud_chat_type_none = CAST(signed long,NONE),

			_hud_chat_type_all = 0,
			_hud_chat_type_team,
			_hud_chat_type_vehicle,
			_hud_chat_type_server,
			_hud_chat_type_info_msg, // used for messages pulled from Strings.dll. Convert 'hud_chat_network_data.message' to a long using 'wtol' to get the resource ID
		};
	};

	namespace GameUI
	{
		struct s_hud_chat_globals
		{
			bool active; unsigned char : 8; unsigned short : 16;
			Enums::hud_chat_type chat_type;

			Console::s_terminal_state terminal_state;

			uint line_timers[8];
		}; static_assert( sizeof(s_hud_chat_globals) == 0x1E8 );

		s_hud_chat_globals*		HudChatGlobals();
		long HudChatLineCount();
	};

	namespace blam
	{
		void __cdecl hud_chat_to_network(long player_number, signed long chat_type, wconst char * text);

		void __cdecl hud_chat_display_message(wconst char * message);
	};
};
