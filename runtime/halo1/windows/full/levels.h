#pragma once

#include <xutility>
#include "../../../../BlamSauce/cseries/base.h"

//hmm...
namespace Yelo::Enums {
	enum {
		_single_player_level_a10,
		_single_player_level_a30,
		_single_player_level_a50,
		_single_player_level_b30,
		_single_player_level_b40,
		_single_player_level_c10,
		_single_player_level_c20,
		_single_player_level_c40,
		_single_player_level_d20,
		_single_player_level_d40,

		k_number_of_single_player_levels, // NUMBER_OF_SINGLE_PLAYER_LEVELS
	};

	enum {
		k_maximum_number_of_campaigns       = 4, // based on Halo2+
		k_maximum_number_of_campaign_levels = 10, // Halo1 is limited to this, Halo2+ allows up to 20
	};

	enum halo1_campaign_level {
		_halo1_campaign_level_a10,
		_halo1_campaign_level_a30,
		_halo1_campaign_level_a50,
		_halo1_campaign_level_b30,
		_halo1_campaign_level_b40,
		_halo1_campaign_level_c10,
		_halo1_campaign_level_c20,
		_halo1_campaign_level_c40,
		_halo1_campaign_level_d20,
		_halo1_campaign_level_d40,

		k_number_of_halo1_campaign_levels,
	}; static_assert(k_number_of_halo1_campaign_levels == k_maximum_number_of_campaign_levels);
};

namespace Yelo::Main {
	// actually scenario paths
	extern const char * k_halo1_campaign_level_names[Enums::k_number_of_halo1_campaign_levels];
};

namespace Yelo::blam {
	short __cdecl  main_get_campaign_level_from_name(const char * level_name);

	inline const char * __cdecl main_get_campaign_level_name(short level_index)(short level_index) {
		if (level_index >= 0 && level_index < std::size(Main::k_halo1_campaign_level_names))
			return Main::k_halo1_campaign_level_names[level_index];

		return nullptr;
	}
};

static Yelo::const char * k_campaign_level_names[Yelo::Enums::k_number_of_halo1_campaign_levels] = {
	R"(levels\a10\a10)",
	R"(levels\a30\a30)",
	R"(levels\a50\a50)",
	R"(levels\b30\b30)",
	R"(levels\b40\b40)",
	R"(levels\c10\c10)",
	R"(levels\c20\c20)",
	R"(levels\c40\c40)",
	R"(levels\d20\d20)",
	R"(levels\4d0\d40)",
};

static inline short __cdecl get_level_from_name(char name[128]) {
	if (strstr(name, "a10")) return Yelo::Enums::_halo1_campaign_level_a10;
	else if (strstr(name, "a30")) return Yelo::Enums::_halo1_campaign_level_a30;
	else if (strstr(name, "a50")) return Yelo::Enums::_halo1_campaign_level_a50;
	else if (strstr(name, "b30")) return Yelo::Enums::_halo1_campaign_level_b30;
	else if (strstr(name, "b40")) return Yelo::Enums::_halo1_campaign_level_b40;
	else if (strstr(name, "c10")) return Yelo::Enums::_halo1_campaign_level_c10;
	else if (strstr(name, "c20")) return Yelo::Enums::_halo1_campaign_level_c20;
	else if (strstr(name, "c40")) return Yelo::Enums::_halo1_campaign_level_c40;
	else if (strstr(name, "d20")) return Yelo::Enums::_halo1_campaign_level_d20;
	else if (strstr(name, "d40")) return Yelo::Enums::_halo1_campaign_level_d40;

	return -1;
}
