#pragma once
namespace Yelo::Enums {
	enum e_vehicle_remapper_vehicle {
		_vehicle_remapper_vehicle_warthog,
		_vehicle_remapper_vehicle_ghost,
		_vehicle_remapper_vehicle_scorpion,
		_vehicle_remapper_vehicle_rwarthog,
		_vehicle_remapper_vehicle_banshee,
		_vehicle_remapper_vehicle_cg_turret,

		k_number_of_vehicle_remapper_vehicles,

		_vehicle_remapper_vehicle_custom1 = k_number_of_vehicle_remapper_vehicles,
		_vehicle_remapper_vehicle_custom2,
		_vehicle_remapper_vehicle_custom3,

		k_number_of_vehicle_remapper_vehicles_yelo,
	};

	enum e_vehicle_set {
		_vehicle_set_default,
		_vehicle_set_none,
		_vehicle_set_warthogs,
		_vehicle_set_ghosts,
		_vehicle_set_scorpions,
		_vehicle_set_rwarthogs,
		_vehicle_set_banshees,
		_vehicle_set_cg_turrets,
		_vehicle_set_custom,

		k_number_of_vehicle_sets,

		_vehicle_set_custom1 = k_number_of_vehicle_sets,
		_vehicle_set_custom2,
		_vehicle_set_custom3,

		k_number_of_vehicle_sets_yelo,
	};
};
