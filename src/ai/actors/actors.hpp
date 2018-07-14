#pragma once

#include "../../cseries/base.h"
#include "../../memory/datum_index.h"
#include "../../hs/types.hpp"

//TODO:
#include  <engine_functions.h>

namespace Yelo {
	namespace Enums {
		enum actor_action : short;

		enum {
			k_maximum_number_of_units_per_swarm = 16,

			k_maximum_number_of_active_swarms = 32,
		};

		enum actor_default_state : short {
			_actor_default_state_none,
			_actor_default_state_sleeping,
			_actor_default_state_alert,
			_actor_default_state_moving_repeat_same_position,
			_actor_default_state_moving_loop,
			_actor_default_state_moving_loop_back_and_forth,
			_actor_default_state_moving_loop_randomly,
			_actor_default_state_moving_randomly,
			_actor_default_state_guarding,
			_actor_default_state_guarding_at_guard_position,
			_actor_default_state_searching,
			_actor_default_state_fleeing,

			k_number_of_actor_default_states,
		};

		enum actor_fire_target : short {
			_actor_fire_target_prop         = 1,
			_actor_fire_target_manual_point = 2,
		};

		enum actor_acknowledgement : short {
			_actor_acknowledgement_never,
			_actor_acknowledgement_combat,
			_actor_acknowledgement_instant,
			_actor_acknowledgement_searching,
			_actor_acknowledgement_definite,
		};

		enum actor_knowledge_type : short {
			_actor_knowledge_type_noncombat0,
			_actor_knowledge_type_guard1,
			_actor_knowledge_type_guard2,
			_actor_knowledge_type_noncombat3,

			k_number_of_actor_knowledge_types,
		};
	};

	namespace AI {
	};

	namespace blam {
		static auto actor_del_ptr = K_ACTOR_DELETE;

		// Deletes and detaches the specified actor from all AI (encounters, etc.)
		void __cdecl actor_delete(datum_index actor_index, bool is_dead = false);

		static auto actor_up_ptr = K_ACTOR_UPDATE;

		void __cdecl actor_update(const datum_index actor_index);

		static auto actor_c_unit = K_ACTOR_CUSTOMIZE_UNIT;

		void __cdecl actor_customize_unit(const datum_index actor_variant, const datum_index unit_index);

		static auto actor_s_active = K_ACTOR_SET_ACTIVE;

		void __cdecl actor_set_active(const datum_index actor_index, const bool active);

		static auto actor_s_dormant = K_ACTOR_SET_DORMANT;

		void __cdecl actor_set_dormant(const datum_index actor_index, const bool dormant);

		static auto actor_d_props = K_ACTOR_DELETE_PROPS;

		void __cdecl actor_delete_props(const datum_index actor_index);

		static auto actor_f_freeze = K_ACTOR_FREEZE;

		void __cdecl actor_freeze(const datum_index actor_index);

		// static auto actor_b_braindead = K_ACTOR_SET_BRAINDEAD;
		void __cdecl actor_braindead(const datum_index actor_index, const bool braindead);

		static auto actor_c_f_unit = K_ACTOR_CREATE_FOR_UNIT;
		datum_index
		__cdecl actor_create_for_unit(const bool is_swarm, const datum_index unit_index, const datum_index actor_variant, const datum_index encounter_index, const long squad_index, const long arg7,
												const long arg6, const bool magic_sight_after_timer, const Enums::actor_default_state initial_state, const Enums::actor_default_state return_state,
												const long command_list_index, const long sequence_id);

		static auto actor_a_change = K_ACTOR_ACTION_CHANGE;
		void __cdecl actor_action_change(const datum_index actor_index, const Enums::actor_action new_action_type, const byte *new_action_data);
	};
};
