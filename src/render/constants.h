/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo::Enums {
	enum {
		k_maximum_rendered_objects = 256,

		k_maximum_rendered_clusters = 0x80,

		k_maximum_rendered_triangles = 0x4000,

		k_maximum_rendered_distant_lights = 2,
		k_maximum_rendered_point_lights   = 2
	};

	enum {
		k_maximum_rendered_objects_upgrade_amount = 256,
	};
};

