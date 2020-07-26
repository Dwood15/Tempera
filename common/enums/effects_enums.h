/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo::Enums {
	enum {
		k_maximum_number_of_contrails_per_map       = 256,
		k_maximum_number_of_contrail_points_per_map = 1024,

		k_maximum_number_of_effects_per_map          = 256,
		k_maximum_number_of_effect_locations_per_map = 512,

		k_maximum_number_of_particle_systems_per_map          = 64,
		k_maximum_number_of_particle_system_particles_per_map = 512,

		k_maximum_number_of_particles_per_map = 1024,

			//UPGRADES
		k_maximum_number_of_particles_per_map_upgrade_amount = 512 + 212,

		k_maximum_number_of_effects_per_map_upgrade_amount          = 0,
		k_maximum_number_of_effect_locations_per_map_upgrade_amount = 0,

		k_maximum_number_of_particle_system_particles_per_map_upgrade_amount = 0,
	};
};
