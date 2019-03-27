// #include "objects.h"
#include <enums/generic_enums.h>
#include "../math/real_math.h"
#include "../memory/memory_pool.h"
#include "objects.h"
#include "../cseries/MacrosCpp.h"

namespace Yelo::Objects {
	size_t PredictMemoryPoolUsage(const object_type type, const int32 node_count, const bool include_yelo_upgrades) {
		size_t total_headers_size     = sizeof(Memory::s_memory_pool_block);
		size_t total_node_memory_size = (sizeof(real_orientation3d) * 2) + sizeof(real_matrix4x3);
		total_node_memory_size *= node_count;

		size_t object_type_size = 0;
		switch (type) {
			case _object_type_biped:
				object_type_size = k_object_size_biped;
				break;
			case _object_type_vehicle:
				object_type_size = k_object_size_vehicle;
				break;

			case _object_type_weapon:
				object_type_size = k_object_size_weapon;
				break;
			case _object_type_equipment:
				object_type_size = k_object_size_equipment;
				break;
			case _object_type_garbage:
				object_type_size = k_object_size_garbage;
				break;

			case _object_type_projectile:
				object_type_size = k_object_size_projectile;
				break;
			case _object_type_scenery:
				object_type_size = k_object_size_scenery;
				break;

			case _object_type_machine:
				object_type_size = k_object_size_machine;
				break;
			case _object_type_control:
				object_type_size = k_object_size_control;
				break;
			case _object_type_light_fixture:
				object_type_size = k_object_size_light_fixture;
				break;

			case _object_type_placeholder:
				object_type_size = k_object_size_placeholder;
				break;
			case _object_type_sound_scenery:
				object_type_size = k_object_size_sound_scenery;
				break;

			default:
				return 0;
		}

		return object_type_size + total_node_memory_size + total_headers_size;
	}
};
