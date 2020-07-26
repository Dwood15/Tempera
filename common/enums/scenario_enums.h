#pragma once
namespace Yelo::Enums {
	enum production_build_stage {
		_production_build_stage_ship,
		_production_build_stage_alpha,
		_production_build_stage_beta,
		_production_build_stage_delta,
		_production_build_stage_epsilon,
		_production_build_stage_release,
	};

	enum project_yelo_info {
		k_group_tag = 'yelo',
		k_version   = 2,
	};

	constexpr int k_maximum_object_names_per_scenario = 512;

	constexpr int k_maximum_skies_per_scenario          = 32;
	constexpr int k_maximum_structure_bsps_per_scenario = 16;

		// How much OS-upgrades [k_maximum_structure_bsps_per_scenario]
		constexpr int k_maximum_structure_bsps_per_scenario_upgrade = k_maximum_structure_bsps_per_scenario * 2;

	enum scenario_netgame_type {
		_scenario_netgame_type_none,
		_scenario_netgame_type_ctf,
		_scenario_netgame_type_slayer,
		_scenario_netgame_type_oddball,
		_scenario_netgame_type_koth,
		_scenario_netgame_type_race,
		_scenario_netgame_type_terminator,
		_scenario_netgame_type_stub,
		_scenario_netgame_type_ignored1,
		_scenario_netgame_type_ignored2,
		_scenario_netgame_type_ignored3,
		_scenario_netgame_type_ignored4,
		_scenario_netgame_type_all_games,
		_scenario_netgame_type_all_except_ctf,
		_scenario_netgame_type_all_except_ctf_y_race,

		_scenario_netgame_type
	};

	enum scenario_netgame_flag_type {
		_scenario_netgame_flag_ctf_flag,
		_scenario_netgame_flag_ctf_vehicle,
		_scenario_netgame_flag_oddball_ball_spawn,
		_scenario_netgame_flag_race_track,
		_scenario_netgame_flag_race_vehicle,
		_scenario_netgame_flag_vegas_bank,
		_scenario_netgame_flag_teleporter_source,
		_scenario_netgame_flag_teleporter_target,
		_scenario_netgame_flag_hill,

		_scenario_netgame_flag_type,
	};
};

namespace Yelo::Flags {
	enum project_yellow_flags : unsigned short {
		_project_yellow_dont_fix_ui_game_globals_bit,
		_project_yellow_game_updates_ignore_player_pvs_hack_bit,

		k_number_of_project_yellow_flags,

		_project_yellow_invalid_version_bit    = 13,      // runtime. a yelo tag was found but it was an invalid version
		_project_yellow_null_definition_bit    = 14,      // runtime. 'null_yelo' definition
		_project_yellow_cache_is_protected_bit = 15,   // runtime. cache was protected with HEK+

		_project_yellow__first_runtime_bit =
		_project_yellow_invalid_version_bit,
	};
	static_assert(k_number_of_project_yellow_flags <= _project_yellow__first_runtime_bit);

	enum project_yellow_gameplay_flags : unsigned long {
		// don't allow MTV settings for this map, even if the user enables it
			_project_yellow_gameplay_prohibit_multiteam_vehicles_bit,

		k_number_of_project_yellow_gameplay_flags
	};

};
