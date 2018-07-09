#pragma once

namespace Yelo::Enums {

	enum {
		k_maximum_object_names_per_scenario = 512,
	};

	enum scenario_type : short {
		_scenario_type_campaign,
		_scenario_type_multiplayer,
		_scenario_type_main_menu,
	};

	enum {
		k_maximum_skies_per_scenario          = 32,
		k_maximum_structure_bsps_per_scenario = 16,

		// How much OS-upgrades [k_maximum_structure_bsps_per_scenario]
			k_maximum_structure_bsps_per_scenario_upgrade = k_maximum_structure_bsps_per_scenario * 2,
	};

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
