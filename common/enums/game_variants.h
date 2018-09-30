#pragma once
namespace Yelo::Enums {
	enum game_variant_goal_radar : unsigned long {
		_game_variant_goal_radar_motion_tracker,
		_game_variant_goal_radar_navpoints,
		_game_variant_goal_radar_none,

		_game_variant_goal_radar,
	};

	enum game_variant_weapon_set : unsigned long {
		_game_variant_weapon_set_normal,
		_game_variant_weapon_set_pistols,
		_game_variant_weapon_set_rifles,
		_game_variant_weapon_set_plasma,
		_game_variant_weapon_set_sniper,
		_game_variant_weapon_set_sans_snipers,
		_game_variant_weapon_set_rockets,
		_game_variant_weapon_set_shotguns,
		_game_variant_weapon_set_short_range,
		_game_variant_weapon_set_human,
		_game_variant_weapon_set_covenant,
		_game_variant_weapon_set_classic,
		_game_variant_weapon_set_heavies,

		_game_variant_weapon_set,
	};

	enum game_trait : unsigned long {
		_game_trait_none,
		_game_trait_invisible,
		_game_trait_extra_damage,
		_game_trait_damage_resistant,
		_game_trait,
	};

	enum oddball_carrier_speed : unsigned long {
		_oddball_carrier_speed_normal,
		_oddball_carrier_speed_faster,
		_oddball_carrier_speed_slower,

		_oddball_carrier_speed,
	};

	enum oddball_ball_type : unsigned long {
		_oddball_ball_type_normal,
		_oddball_ball_type_reverse_tag,
		_oddball_ball_type_juggernaut,

		_oddball_ball_type,
	};

	enum race_type : unsigned long {
		_race_type_normal,
	};
};

namespace Yelo::Flags {
	enum game_variant_flags : unsigned long {
		_game_variant_show_players_on_radar_bit,
		_game_variant_show_homies_bit,
		_game_variant_infinite_grenades_bit,
		_game_variant_players_have_shields_bit,
		_game_variant_invisible_players_bit,
		_game_variant_use_custom_start_equipment_bit,
	};
};
