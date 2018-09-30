#pragma once

#include <cstring>
#include <enums/materials.h>
#include "../../scenario/scenario_location.h"
#include "../../memory/datum_index.h"
#include "../../math/real_math.h"
#include "enums/generic_enums.h"

namespace Yelo::Objects {
	struct s_damage_data {
		datum_index effect_definition_index;         // 0x0
		long_flags  flags;                        // 0x4
		datum_index responsible_player_index;         // 0x8
		datum_index responsible_unit_index;            // 0xC
		game_team   responsible_units_team;      // 0x10
		short : 16;                                 // 0x12
		Scenario::s_scenario_location location;               // 0x14
		real_point3d                  damage_position;               // 0x1C
		real_vector3d                 unkA; // position		// 0x28
		real_vector3d                 unkB; // direction?		// 0x34
		// scale examples:
		// AOE distance scale (via radius)
		// unit blip type (medium=.5, small=.25, large=1)
		real                          scale;                              // 0x40
		real                          damage_multiplier;                     // 0x44
		UNKNOWN_TYPE(real);                        // 0x48 shield or body amount prior to the damage was applied
		Enums::material_type          material;               // 0x4C
		short : 16;                                 // 0x4E
		UNKNOWN_TYPE(int32); // only seen this written to, and when it was it was set to a s_projectile_material_response_definition*
	};
	STAT_ASSERT(s_damage_data, 0x54);
};

namespace Yelo::blam {
	using namespace Yelo::Objects;

	inline void damage_data_new(s_damage_data &data, datum_index damage_effect_definiton_index);

	bool object_restore_body(datum_index object_index);

	void object_deplete_body(datum_index object_index);

	void object_deplete_shield(datum_index object_index);

	void object_double_charge_shield(datum_index object_index);

	void area_of_effect_cause_damage(s_damage_data &data, datum_index unused_object_index = datum_index::null());

	void object_cause_damage(s_damage_data &data, datum_index damaged_object_index,
									 int16 node_index = NONE, int16 region_index = NONE, int16 damage_materials_element_index = NONE,
									 real_vector3d *normal = NULL);
};
