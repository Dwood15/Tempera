#include <precompile.h>

#pragma once

namespace Yelo {
	namespace GameUI {
		struct s_hud_scripted_globals {
			bool show_hud;
			bool show_hud_help_text;
			unsigned short : 16;
		};

		s_hud_scripted_globals *HudScriptedGlobals();
	};

	namespace blam {
		const char * __cdecl hud_get_item_string(short reference_index);
	};
};
