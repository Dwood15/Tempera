#pragma once
/*
*/

#include "../cseries/base.h"
#include "../math/real_math.h"
#include "../memory/datum_index.h"

namespace Yelo {
	namespace AI {
		struct s_ai_spatial_effect {
			short type;
			unsigned short : 16; // unknown field
			real_vector3d location;
			uint        tick;
		}; static_assert(sizeof(s_ai_spatial_effect) == 0x14);

		struct s_ai_vehicle_enterable_datum {
			datum_index vehicle_index;
			real        distance;
			short       teams_bitvector;
			short       actor_types_bitvector;
			short       actors_count;
			unsigned short : 16;
			datum_index actor_indices[6];
		}; static_assert(sizeof(s_ai_vehicle_enterable_datum) == 0x28);

		struct s_ai_globals_data {
			bool ai_active;
			bool initialized;
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			datum_index first_encounterless_actor_index;
			unsigned long : 32; // unknown field
			bool dialogue_triggers_enabled;
			unsigned char : 8; unsigned short : 16;
			PAD(1, 8); // unknown field
			PAD(2, 8); // unknown field
			PAD(3, 8); // unknown field
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			PAD(4, 256); // unknown field
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			s_ai_spatial_effect spatial_effects[32];
			bool                grenades_enabled;
			unsigned char : 8;
			short                        vehicles_enterable_count;
			s_ai_vehicle_enterable_datum vehicles_enterable[32];
			short                        pending_mounted_weapons_count;
			unsigned short : 16;
			datum_index pending_mounted_weapon_indices[8];
		}; static_assert(sizeof(s_ai_globals_data) == 0x8DC);
	};
};
