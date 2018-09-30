#pragma once

#include "game_variants.h"

namespace Yelo::Enums {
	constexpr int k_network_server_password_length = 8;
	constexpr int k_network_game_join_token_size   = 16;
	constexpr int k_network_game_challenge_length  = 7;
	constexpr int k_player_name_length             = 11;

	enum game_engine : unsigned int {
		_game_engine_none,
		_game_engine_ctf,
		_game_engine_slayer,
		_game_engine_oddball,
		_game_engine_king,
		_game_engine_race,
		_game_engine_stub,

		k_number_of_game_engines
	};

	enum player_powerup {
		_player_powerup_active_camo,
		_player_powerup_full_spectrum_vision,

		k_number_of_player_powerups,
	};

	enum multiplayer_team {
		_multiplayer_team_red,
		_multiplayer_team_blue,

		k_number_of_multiplayer_teams,
	};

		enum game_engine_custom : long
		{
			_game_engine_custom1 = k_number_of_game_engines,
			_game_engine_custom2,
			_game_engine_custom3,
			_game_engine_custom4,
			_game_engine_custom5,
			_game_engine_custom,
		};

		enum game_trait_custom : long
		{
			_game_custom_trait1 = _game_trait+1,
			_game_custom_trait2,
			_game_custom_trait3,
			_game_custom_trait4,
			_game_custom_trait5,
			_game_custom_trait,
		};

		enum game_engine_mode : long
		{
			_game_engine_mode_active,
			_game_engine_mode_postgame_delay,
			_game_engine_mode_postgame_rasterize_delay,
			_game_engine_mode_3,
		};

};

namespace Yelo::Flags {
	enum game_engine_flags : unsigned int {
		_game_engine_player_spawn_in_team_designator_locations_only_bit,
		// Engine's score is based on the kills a player has
		_game_engine_uses_slayer_scoring_bit,
	};
};
