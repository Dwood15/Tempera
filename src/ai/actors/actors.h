#pragma once

#include "macros_generic.h"
#include "../../memory/datum_index.h"
#include "../../hs/types/types.h"
#include "../actions/actions.hpp"

//TODO:
#include <enums/hs_enums.h>

namespace Yelo {
	namespace blam {
		// static auto actor_del_ptr = K_ACTOR_DELETE;
		//
		// // Deletes and detaches the specified actor from all AI (encounters, etc.)
		// void actor_delete(datum_index actor_index, bool is_dead = false);
		//
		// static auto actor_up_ptr = K_ACTOR_UPDATE;
		//
		// void actor_update(const datum_index actor_index);
		//
		// static auto actor_c_unit = K_ACTOR_CUSTOMIZE_UNIT;
		//
		// void actor_customize_unit(const datum_index actor_variant, const datum_index unit_index);
		//
		// static auto actor_s_active = K_ACTOR_SET_ACTIVE;
		//
		// void actor_set_active(const datum_index actor_index, const bool active);
		//
		// static auto actor_s_dormant = K_ACTOR_SET_DORMANT;
		//
		// void actor_set_dormant(const datum_index actor_index, const bool dormant);
		//
		// static auto actor_d_props = K_ACTOR_DELETE_PROPS;
		//
		// void actor_delete_props(const datum_index actor_index);
		//
		// static auto actor_f_freeze = K_ACTOR_FREEZE;
		//
		// void actor_freeze(const datum_index actor_index);
		//
		// // static auto actor_b_braindead = K_ACTOR_SET_BRAINDEAD;
		 void actor_braindead(const datum_index actor_index, const bool braindead);

		// static auto actor_c_f_unit = K_ACTOR_CREATE_FOR_UNIT;
		// datum_index
		// actor_create_for_unit(const bool is_swarm, const datum_index unit_index, const datum_index actor_variant, const datum_index encounter_index, const long squad_index, const long arg7,
		// 										const long arg6, const bool magic_sight_after_timer, const Enums::actor_default_state initial_state, const Enums::actor_default_state return_state,
		// 										const long command_list_index, const long sequence_id);
		//
		// static auto actor_a_change = K_ACTOR_ACTION_CHANGE;
		// void actor_action_change(const datum_index actor_index, const Enums::actor_action new_action_type, const byte *new_action_data);
	};
};
