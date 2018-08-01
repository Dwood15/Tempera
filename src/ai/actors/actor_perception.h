#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum actor_perception_type : short
		{
			_actor_perception_type_none,
			_actor_perception_type_partial,
			_actor_perception_type_full,
			_actor_perception_type_unmistakable,

			k_number_of_actor_perception_types,
		};

		enum actor_target_type : short
		{
			k_number_of_actor_target_types = 12
		};

		enum actor_danger_zone : short
		{
			_actor_danger_zone_none,
			_actor_danger_zone_suicide,
			_actor_danger_zone_projectile,
			_actor_danger_zone_vehicle,
		};
	};
};
