#pragma once

#include <macros_generic.h>
#include <enums/engine.h>
#include <enums/player_enums.h>
#include "../memory/datum_index.h"
#include "../math/real_math.h"
#include "objects/vehicle_remapper.h"

namespace Yelo::GameEngine {
	struct s_universal_variant {
		bool teams;
		char  : 8;
		short : 16; // these three pad bytes are encoded in game settings update messages...
		Flags::game_variant_flags      flags;
		Enums::game_variant_goal_radar goal_rader;
		bool                           odd_man_out;
		char  : 8;                              // these three pad bytes are encoded in game settings update messages...
		short : 16;
		int respawn_time_growth;
		int respawn_time;
		int suicide_penalty;
		int lives;

		real                           health; // I thought this was damage_multiplier...
		int                            score_to_win;
		Enums::game_variant_weapon_set weapon_set;

		Objects::s_vehicle_set vehicle_set[Enums::k_number_of_multiplayer_teams];
		int                    vehicles_respawn;

		byte friendly_fire; // need an enum for this
		char  : 8;
		short : 16;
		int  friendly_fire_penalty;
		bool team_autobalance;
		char  : 8;
		short : 16;

		int time_limit;
	};
	STAT_ASSERT(s_universal_variant, 0x48);
#pragma pack(push, 1)
	union s_game_engine_variant {
		// The message delta definition for this struct encodes this as a byte[], so everything is copied across the wire
		enum { k_max_variant_size = 0x38 };

		PAD(0, k_max_variant_size);

		struct s_ctf {
			bool assault;
			PAD8; // unused...

			struct {
				bool  reset;
				bool  at_home_to_score;
				int32 flag_type; // 1 = single
			}    flag;
		} ctf;
		STAT_ASSERT(s_ctf, 0x8);

		struct s_slayer {
			bool death_handicap; // True: killed player's speed increases in magnitudes of one
			bool kill_handicap; // True: killer's speed decreases in magnitudes of two
			bool kill_in_order;
		} slayer;
		STAT_ASSERT(s_slayer, 0x3);

		struct s_oddball {
			bool random_start;
			char : 8;
			short                        : 16;
			Enums::oddball_carrier_speed ball_speed;

			Enums::game_trait        nonball_trait;
			Enums::game_trait        ball_trait;
			Enums::oddball_ball_type type;
			int32                    ball_count;
		} oddball;
		STAT_ASSERT(s_oddball, 0x18);

		struct s_king {
			bool moving_hill;
		} king;
		STAT_ASSERT(s_king, 0x1);

		struct s_race {
			Enums::race_type race_type;
			long_enum        team_scoring;
		} race;
		STAT_ASSERT(s_race, 0x8);

	}; STAT_ASSERT(s_game_engine_variant, s_game_engine_variant::k_max_variant_size);
	struct s_game_variant {
		wchar_t             name[24];                     // 0x0
		long_enum           game_engine_index;            // 0x30 Enums::game_engine
		s_universal_variant universal_variant;      // 0x34
		PAD64
		PAD64
		PAD64
		PAD64;               // 0x7C, unused...
		s_game_engine_variant game_engine_variant;   // 0x9C
		long_flags            flags;                     // 0xD4
		//uint32 crc;								// 0xD8, not actually part of the structure, only "used" when presisting to/from files
	}; STAT_ASSERT(s_game_variant, 0xD8/*0xDC*/ );

	struct s_ctf_globals {
		real_point3d *team_flag_positions[Enums::k_number_of_multiplayer_teams];
		datum_index  team_flag_object_indexes[Enums::k_number_of_multiplayer_teams]; // weapon objects
		int          team_score[Enums::k_number_of_multiplayer_teams];
		int          score_limit;
		bool         team_flag_is_taken[Enums::k_number_of_multiplayer_teams];
		short : 16;
		uint team_flag_is_taken_sound_timer[Enums::k_number_of_multiplayer_teams];
		uint flag_swap_timer;
		uint flag_failure_sound_timer;
		byte team_with_flag;
		byte  : 8;
		short : 16;
	};
	STAT_ASSERT(s_ctf_globals, 0x34);

	struct s_slayer_globals {
	};

	struct s_king_globals {
	};

	struct s_race_globals {
	};

	struct s_oddball_globals {
		int         score_to_win;
		int         team_score[Yelo::Enums::k_multiplayer_maximum_players];
		int         individual_score[Yelo::Enums::k_multiplayer_maximum_players];
		int         undocumented1[Yelo::Enums::k_multiplayer_maximum_players];
		datum_index current_ball_owners[Yelo::Enums::k_multiplayer_maximum_players]; // player_index
		int         current_ball_held_times[Yelo::Enums::k_multiplayer_maximum_players]; // game-ticks
	};
#pragma pack(pop)

};
