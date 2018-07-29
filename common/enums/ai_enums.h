#pragma once

namespace Yelo::Enums {
	enum actor_type : unsigned short {
		_actor_type_elite,
		_actor_type_jackal,
		_actor_type_grunt,
		_actor_type_hunter,
		_actor_type_engineer,
		_actor_type_assassin,      // uses elite actor type definition
		_actor_type_player,         // uses marine actor type definition
		_actor_type_marine,
		_actor_type_crew,
		_actor_type_combat_form,
		_actor_type_infection_form,
		_actor_type_carrier_form,
		_actor_type_monitor,      // uses sentinel actor type definition
		_actor_type_sentinel,
		_actor_type_none,         // uses grunt actor type definition
		_actor_type_mounted_weapon,

		k_number_of_actor_types,
	};

	enum {
		_ai_index_type_encounter,
		_ai_index_type_platoon,
		_ai_index_type_squad,

		k_number_of_ai_index_types,
	};

	constexpr int k_maximum_actor_starting_locations = 32;

	enum {
		_ai_atom_pause,
		_ai_atom_go_to,
		_ai_atom_go_to_and_face,
		_ai_atom_move_in_direction,
		_ai_atom_look,
		_ai_atom_animation_mode,
		_ai_atom_crouch,
		_ai_atom_shoot,
		_ai_atom_grenade,
		_ai_atom_vehicle,
		_ai_atom_running_jump,
		_ai_atom_targeted_jump,
		_ai_atom_script,
		_ai_atom_animate,
		_ai_atom_recording,
		_ai_atom_action,
		_ai_atom_vocalize,
		_ai_atom_targeting,
		_ai_atom_initiative,
		_ai_atom_wait,
		_ai_atom_loop,
		_ai_atom_die,
		_ai_atom_move_immediate,
		_ai_atom_look_random,
		_ai_atom_look_player,
		_ai_atom_look_object,
		_ai_atom_set_radius,
		_ai_atom_teleport,

		k_number_of_ai_atoms,
	};

	enum danger_trigger : short {
		_danger_trigger_never,
		_danger_trigger_visible,
		_danger_trigger_shooting,
		_danger_trigger_shooting_near_us,
		_danger_trigger_damaging_us,
		_danger_trigger_unused,
		_danger_trigger_unused_1,
		_danger_trigger_unused_2,
		_danger_trigger_unused_3,
		_danger_trigger_unused_4,

		_danger_trigger,
	};
	enum defensive_crouch_type : short {
		_defensive_crouch_type_never,
		_defensive_crouch_type_danger,
		_defensive_crouch_type_low_shields,
		_defensive_crouch_type_hide_behind_shield,
		_defensive_crouch_type_any_target,
		_defensive_crouch_type_flood_shamble,

		_defensive_crouch_type,
	};
	enum actor_movement_type : short {
		_actor_movement_type_always_run,
		_actor_movement_type_always_crouch,
		_actor_movement_type_switch_types,

		_actor_movement_type,
	};
	enum actor_special_fire_mode : short {
		_actor_special_fire_mode_none,
		_actor_special_fire_mode_overcharge,
		_actor_special_fire_mode_secondary_trigger,

		_actor_special_fire_mode,
	};
	enum actor_special_fire_situation : short {
		_actor_special_fire_situation_never,
		_actor_special_fire_situation_enemy_visible,
		_actor_special_fire_situation_enemy_out_of_sight,
		_actor_special_fire_situation_strafing,

		_actor_special_fire_situation,
	};
	enum grenade_type : short {
		_grenade_type_human_fragmentation,
		_grenade_type_covenant_plasma,
		_grenade_type_custom_2,
		_grenade_type_custom_3,

		_grenade_type,
	};
	enum trajectory_type : short {
		_trajectory_type_toss,
		_trajectory_type_lob,
		_trajectory_type_bounce,

		_trajectory_type,
	};
	enum grenade_stimulus : short {
		_grenade_stimulus_never,
		_grenade_stimulus_visible_target,
		_grenade_stimulus_seek_cover,

		_grenade_stimulus,
	};

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
