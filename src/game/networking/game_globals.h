#pragma once

namespace Yelo::Networking {
	struct network_update_globals {
		struct _local_player {
			long update_rate;                  // 15
			long vehicle_update_rate;            // 15
		} local_player;

		struct _remote_player {
			long action_baseline_update_rate;      // 180
			long position_update_rate;            // 30
			long position_baseline_update_rate;   // 90
			long vehicle_update_rate;            // 15
			long vehicle_baseline_update_rate;      // 90, not used
		} remote_player;

		bool use_super_remote_players_action_update;// true
		bool use_new_vehicle_update_scheme;         // true
	};

	// Network settings dealing with the rate of updating certain data
	network_update_globals *UpdateSettings();
};
