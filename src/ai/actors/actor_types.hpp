#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum actor_type : short
		{
			_actor_type_elite,
			_actor_type_jackal,
			_actor_type_grunt,
			_actor_type_hunter,
			_actor_type_engineer,
			_actor_type_assassin,		// uses elite actor type definition
			_actor_type_player,			// uses marine actor type definition
			_actor_type_marine,
			_actor_type_crew,
			_actor_type_combat_form,
			_actor_type_infection_form,
			_actor_type_carrier_form,
			_actor_type_monitor,		// uses sentinel actor type definition
			_actor_type_sentinel,
			_actor_type_none,			// uses grunt actor type definition
			_actor_type_mounted_weapon,

			k_number_of_actor_types,
		};
	};

	namespace blam
	{
		extern const char *  g_actor_type_names[Enums::k_number_of_actor_types];
	};
};
