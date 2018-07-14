/*
	See GPLv3 specific license information
*/
#pragma once

#include "../memory/datum_index.h"
#include "../memory/data_base.h"
#include "ai_scenario_definitions.h"
#include "../memory/array.h"

namespace Yelo {
	namespace Enums {
		enum {
			k_maximum_squads_per_encounter   = 64,
			k_maximum_platoons_per_encounter = 32,

			k_maximum_encounters_per_map = 128,

			k_maximum_squads_per_map   = 1024,
			k_maximum_platoons_per_map = 256,

			k_maximum_examined_pursuit_positions_per_map            = 256,
			k_number_of_actor_indices_per_examined_pursuit_position = 6,
		};

		enum post_combat_behavior_type {
			k_number_of_post_combat_behavior_types = 4,
		};
	};

	namespace AI {
		struct s_encounter_actor_iterator {
			datum_index encounter_index;
			datum_index next_actor_index;
			datum_index prev_actor_index;

			void SetEndHack() {
				encounter_index = next_actor_index = prev_actor_index = datum_index::null();
			}

			bool IsEndHack() const {
				return next_actor_index.IsNull();
			}
		};

		static_assert(sizeof(s_encounter_actor_iterator) == 0xC);

		struct s_encounter_datum : TStructImpl(108) {
		// 0x2 short team_index
		// 0x4 short squad_base
		// 0x6 short squad_count
		// 0x8 short platoon_base
		// 0xA short platoon_count

		// 0xD bool active
		// 0xE game_ticks_t, ticks until deactivation?
		// 0x10 long activated_time
		// 0x14 datum_index, actor index
		// 0x18 short original_count
		// 0x1A short actor_count
		// 0x1C short unique_leader_count
		// 0x20 short link_encounter_indices_count
		// 0x22 short link_encounter_indices[3]
		// 0x28 bool
		// 0x29  unsigned char : 8
		// 0x2A short actors_alive_count
		// 0x2C short swarm_unit_count?
		// 0x2E short actors_in_combat_count?
		// 0x30 short actors_fighting_count?
		// 0x32 unsigned short : 16
		// 0x34 real
		// 0x38 datum_index pursuit_index
		// 0x3C bool respawn_enabled
		// 0x3D  unsigned char : 8
		// 0x3E short
		// 0x40 bool blind
		// 0x41 bool deaf
		// 0x42 bool
		// 0x43 bool
		// 0x44 bool enemy_alive
		// 0x45 bool enemy_visible
		// 0x46 bool
		// 0x47 bool
		// 0x48 bool
		// 0x4A game_ticks_t, ticks related to 0x47 and 0x48
		// 0x4C short
		// 0x4E unsigned short : 16
		// 0x50 game_time_t, time related to enemy_visible
		// 0x54 game_time_t, time related to enemy_alive
		// 0x58 game_time_t
		// 0x60 bool playfight
		// 0x61  unsigned char : 8
		// 0x62 short follow_type
		//	0 players
		//	1 object
		//	2 ai_index
		// 0x64 datum_index follow_type_index
		// 0x68 UNUSED32?
	};
	typedef Memory::DataArray<s_encounter_datum, Enums::k_maximum_encounters_per_map> encounter_data_t;

	struct s_squad_datum {
		constexpr auto maxLocation = Yelo::Enums::k_maximum_actor_starting_locations;
		long_flags     required_locations[BIT_VECTOR_SIZE_IN_DWORDS(maxLocation)];
		long_flags     unused_locations[BIT_VECTOR_SIZE_IN_DWORDS(maxLocation)];
		unsigned long : 32;                  // Haven't checked what is here yet
		short respawn_total;      // 0xC
		unsigned short : 16;                  // I believe this is padding
		bool automatic_migration;   // 0x10
		UNKNOWN_TYPE(bool);         // 0x11
		short delay_ticks;         // 0x12
		UNKNOWN_TYPE(bool);         // 0x14
		unsigned char : 8;
		short original_count;      // 0x16
		UNKNOWN_TYPE(short);      // 0x18, unit_count?
		UNKNOWN_TYPE(short);      // 0x1A, swarm_unit_count?
		UNKNOWN_TYPE(real);         // 0x1C
	}; static_assert(sizeof(s_squad_datum) == 0x20);
	typedef s_squad_datum squads_data_t[Enums::k_maximum_squads_per_map];
	static_assert(sizeof(squads_data_t) == 0x8000);


	struct s_platoon_datum {
		bool start_in_defending_state;   // 0x0
		unsigned char : 8; unsigned short : 16;                     // I believe this is padding
		short original_count;         // 0x4
		UNKNOWN_TYPE(short);         // 0x6, unit_count?
		UNKNOWN_TYPE(short);         // 0x8, swarm_unit_count?
		unsigned short : 16;
		UNKNOWN_TYPE(real);            // 0xC
	}; static_assert(sizeof(s_platoon_datum) == 0x10);
	typedef s_platoon_datum platoons_data_t[Enums::k_maximum_platoons_per_map];
	static_assert(sizeof(platoons_data_t) == 0x1000);


	struct s_ai_pursuit_datum : Memory::s_datum_base {
		short firing_position_index;   // 0x2
		long examined_game_time;      // 0x4
		UNKNOWN_TYPE(short);         // 0x8
		short       next_actor_index_index;   // 0xA
		datum_index actor_indices[Enums::k_number_of_actor_indices_per_examined_pursuit_position];
		datum_index next_pursuit_index;   // 0x24
	}; static_assert(sizeof(s_ai_pursuit_datum) == 0x28);
	typedef Memory::DataArray<s_ai_pursuit_datum, Enums::k_maximum_examined_pursuit_positions_per_map> ai_pursuit_data_t;
};
};
