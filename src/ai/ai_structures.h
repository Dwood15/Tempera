#pragma once
/*
*/

#include "macros_generic.h"
#include "../math/real_math.h"
#include "../memory/datum_index.h"

namespace Yelo {
	namespace AI {
		struct s_ai_spatial_effect {
			short type;
			unsigned short unkPadA; // unknown field
			real_vector3d location;
			uint        tick;
		}; static_assert(sizeof(s_ai_spatial_effect) == 0x14);

		struct s_ai_vehicle_enterable_datum {
			datum_index vehicle_index;
			real        distance;
			short       teams_bitvector;
			short       actor_types_bitvector;
			short       actors_count;
			unsigned short unkPad1;
			datum_index actor_indices[6];
		}; static_assert(sizeof(s_ai_vehicle_enterable_datum) == 0x28);

		struct s_ai_globals_data {
			bool ai_active;
			bool initialized;
			unsigned char pad1; // unknown field
			unsigned char pad2; // unknown field
			unsigned short pad3; // unknown field
			unsigned short pad4; // unknown field
			datum_index first_encounterless_actor_index;
			unsigned long pad5; // unknown field
			bool dialogue_triggers_enabled;
			unsigned char pad6;
			unsigned short pad7;

			char pad8[8];
			char pad9[8];
			char pad10[8];
			unsigned short pad11; // unknown field
			unsigned short pad12; // unknown field
			char pad13[256];
			unsigned short pad14; // unknown field
			unsigned short pad15; // unknown field
			s_ai_spatial_effect spatial_effects[32];
			bool                grenades_enabled;
			unsigned char pad16;
			short                        vehicles_enterable_count;
			s_ai_vehicle_enterable_datum vehicles_enterable[32];
			short                        pending_mounted_weapons_count;
			unsigned short pad17;
			datum_index pending_mounted_weapon_indices[8];
		}; static_assert(sizeof(s_ai_globals_data) == 0x8DC);
	};
};
