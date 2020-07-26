#pragma once

#include <macros_generic.h>
#include <enums/vehicle.h>
#include <enums/engine.h>
#include "../../memory/datum_index.h"

namespace Yelo::Objects {
#pragma pack(push, 1)
	struct s_vehicle_set {
		unsigned vehicle_set : 4; // e_vehicle_set
		unsigned warthogs : 3;
		unsigned ghosts : 3;
		unsigned scorpions : 3;
		unsigned rwarthogs : 3;
		unsigned banshees : 3;
		unsigned cg_turrets : 3;
		unsigned custom1 : 3;
		unsigned custom2 : 3;
		unsigned custom3 : 3;
		unsigned pad : 1;
	};
	STAT_ASSERT(s_vehicle_set, sizeof(int));

	//////////////////////////////////////////////////////////////////////////
	// This is still a WIP
	struct s_vehicle_team_remapper {
		int          team_index;
		int          scenario_datum_index; // vehicle block index
		unsigned short multiplayer_spawn_flags;
		short : 16;
		datum_index vehicle_definition_index;

		struct s_datum {
			int      scenario_datum_index;
			bool       spawn; // will this vehicle be spawned?
			byte_flags spawn_flags;
			short : 16;
		} datums[8];
	};
	STAT_ASSERT(s_vehicle_team_remapper, 0x50);
	//////////////////////////////////////////////////////////////////////////
	struct s_vehicle_remapper {
		s_vehicle_team_remapper team_mappings[Enums::k_number_of_multiplayer_teams];
	};
	STAT_ASSERT(s_vehicle_remapper, 0xA0);
	struct s_vehicle_remapper_globals {
		s_vehicle_remapper vehicle_mappings[Enums::k_number_of_vehicle_remapper_vehicles];

		bool processed;
		bool is_teamplay;
	};
	STAT_ASSERT(s_vehicle_remapper_globals, 0x3C2);
#pragma pack(pop)
};
