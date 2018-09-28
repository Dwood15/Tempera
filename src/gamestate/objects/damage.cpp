#pragma once

#include "damage.h"

namespace Yelo::blam {
	inline void damage_data_new(Yelo::Objects::s_damage_data &data, datum_index damage_effect_definiton_index) {
		memset(&data, 0, sizeof(data));
		data.effect_definition_index  = damage_effect_definiton_index;
		data.material                 = Enums::_material_type_none;
		data.responsible_player_index = datum_index::null();
		data.responsible_unit_index   = datum_index::null();
		data.responsible_units_team   = game_team::_game_team_none;
		data.location.cluster_index   = NONE;
		data.scale                    = 1.0f;
		data.damage_multiplier        = 1.0f;
	}
}
