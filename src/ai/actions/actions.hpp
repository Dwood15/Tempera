/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <cstdio>
#include "../../math/color_math.h"
#include "../../memory/datum_index.h"

namespace Yelo {
	namespace Enums {
		enum actor_action : short {
			_actor_action_none,
			_actor_action_sleep,
			_actor_action_alert,
			_actor_action_fight,
			_actor_action_flee,
			_actor_action_uncover,
			_actor_action_guard,
			_actor_action_search,
			_actor_action_wait,
			_actor_action_vehicle,
			_actor_action_charge,
			_actor_action_obey,
			_actor_action_converse,
			_actor_action_avoid,

			k_number_of_actor_actions
		};

		enum action_class : short {
			_action_class_passive = 1,
		};
	};

	namespace AI {
		typedef void (__cdecl *proc_action_function)(datum_index actor_index);

		typedef void (__cdecl *proc_action_function_2C)(datum_index actor_index, datum_index, datum_index);

		struct s_action_function_definition {
			Enums::actor_action action;
			unsigned short : 16;
			const char *               name;
			const real_argb_color debug_color;
			size_t                action_data_size;
			Enums::action_class   action_class;
			unsigned short : 16;
			proc_action_function    begin;
			proc_action_function    perform;
			proc_action_function    update;
			proc_action_function    control;
			proc_action_function    end;
			proc_action_function    func28;
			proc_action_function_2C func2C;
			proc_action_function    func30;
			proc_action_function    func34;
			proc_action_function    func38;
		};
	};
};
