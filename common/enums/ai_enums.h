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

	enum actor_variant_transform_keyframe_rider_handling : short {
		_actor_variant_transform_keyframe_rider_handling_none,
		_actor_variant_transform_keyframe_rider_handling_kill,
		_actor_variant_transform_keyframe_rider_handling_eject,

		_actor_variant_transform_keyframe_rider_handling
	};

	enum actor_variant_transform_keyframe_effect_target : short {
		_actor_variant_transform_keyframe_effect_target_self,
		_actor_variant_transform_keyframe_effect_target_riders,

		_actor_variant_transform_keyframe_effect_target
	};

	enum actor_variant_transform_team_handling : short {
		_actor_variant_transform_team_handling_inherit_from_old_actor,
		_actor_variant_transform_team_handling_inherit_from_attacker,
		_actor_variant_transform_team_handling_override,

		_actor_variant_transform_team
	};

	enum actor_variant_transform_in_actor_state_handling : short {
		_actor_variant_transform_in_actor_state_handling_inherit,
		_actor_variant_transform_in_actor_state_handling_override,
		_actor_variant_transform_in_actor_state_handling_actor_default,

		_actor_variant_transform_in_actor_state_handling
	};

	enum actor_variant_transform_in_actor_action_handling : short {
		_actor_variant_transform_in_actor_action_handling_default,
		_actor_variant_transform_in_actor_action_handling_inherit,
		_actor_variant_transform_in_actor_action_handling_override,

		_actor_variant_transform_in_actor_action_handling
	};

	enum actor_variant_transform_in_encounter_squad_handling : short {
		_actor_variant_transform_in_encounter_squad_handling_inherit_from_old_actor,
		_actor_variant_transform_in_encounter_squad_handling_inherit_from_attacker,
		_actor_variant_transform_in_encounter_squad_handling_free_actor,

		_actor_variant_transform_in_encounter_squad_handling
	};

	enum actor_variant_transform_in_vitality_handling : short {
		_actor_variant_transform_in_vitality_handling_neither,
		_actor_variant_transform_in_vitality_handling_both,
		_actor_variant_transform_in_vitality_handling_shield_only,
		_actor_variant_transform_in_vitality_handling_health_only,

		_actor_variant_transform_in_vitality_handling
	};

	enum actor_variant_transform_out_threshold_type : short {
		_actor_variant_transform_out_threshold_type_both,
		_actor_variant_transform_out_threshold_type_shield_amount_only,
		_actor_variant_transform_out_threshold_type_health_amount_only,

		_actor_variant_transform_out_threshold_type
	};

	enum actor_variant_transform_out_damage_type : short {
		_actor_variant_transform_out_damage_type_both,
		_actor_variant_transform_out_damage_type_weapon_only,
		_actor_variant_transform_out_damage_type_melee_only,

		_actor_variant_transform_out_damage_type
	};

	constexpr int k_maximum_number_of_mounted_weapon_units = 8;

	enum ai_sound_volume {
		k_number_of_ai_sound_volumes = 5,
	};

	enum ai_unit_effect {
		k_number_of_ai_unit_effects = 4,
	};

	enum ai_spatial_effect {
		_ai_spatial_effect_environmental_noise,
		_ai_spatial_effect_weapon_impact,
		_ai_spatial_effect_weapon_detonation,

		k_number_of_ai_spatial_effects
	};

	enum ai_communication_type : short {
		_ai_communication_type_death,
		_ai_communication_type_killing_spree,
		_ai_communication_type_hurt,
		_ai_communication_type_damage,
		_ai_communication_type_sighted_enemy,
		_ai_communication_type_found_enemy,
		_ai_communication_type_unexpected_enemy,
		_ai_communication_type_found_dead_friend,
		_ai_communication_type_allegiance_changed,
		_ai_communication_type_grenade_throwing,
		_ai_communication_type_grenade_startle,
		_ai_communication_type_grenade_sighted,
		_ai_communication_type_grenade_danger,
		_ai_communication_type_lost_contact,
		_ai_communication_type_blocked,
		_ai_communication_type_alert_noncombat,
		_ai_communication_type_search_start,
		_ai_communication_type_search_query,
		_ai_communication_type_search_report,
		_ai_communication_type_search_abandon,
		_ai_communication_type_search_group_abandon,
		_ai_communication_type_uncover_start,
		_ai_communication_type_advance,
		_ai_communication_type_retreat,
		_ai_communication_type_cover,
		_ai_communication_type_sighted_friend_player,
		_ai_communication_type_shooting,
		_ai_communication_type_shooting_vehicle,
		_ai_communication_type_shooting_berserk,
		_ai_communication_type_shooting_group,
		_ai_communication_type_shooting_traitor,
		_ai_communication_type_flee,
		_ai_communication_type_flee_leader_died,
		_ai_communication_type_flee_idle,
		_ai_communication_type_attempted_flee,
		_ai_communication_type_hiding_finished,
		_ai_communication_type_vehicle_entry,
		_ai_communication_type_vehicle_exit,
		_ai_communication_type_vehicle_woohoo,
		_ai_communication_type_vehicle_scared,
		_ai_communication_type_vehicle_falling,
		_ai_communication_type_surprise,
		_ai_communication_type_berserk,
		_ai_communication_type_melee,
		_ai_communication_type_dive,
		_ai_communication_type_uncover_exclamation,
		_ai_communication_type_falling,
		_ai_communication_type_leap,
		_ai_communication_type_postcombat_alone,
		_ai_communication_type_postcombat_unscathed,
		_ai_communication_type_postcombat_wounded,
		_ai_communication_type_postcombat_massacre,
		_ai_communication_type_postcombat_triumph,
		_ai_communication_type_postcombat_check_enemy,
		_ai_communication_type_postcombat_check_friend,
		_ai_communication_type_postcombat_shoot_corpse,
		_ai_communication_type_postcombat_celebrate,

		_ai_communication_type,
	};

	enum ai_communication_priority {
		_ai_communication_priority_none,
		_ai_communication_priority_filler,
		_ai_communication_priority_chatter,
		_ai_communication_priority_talk,
		_ai_communication_priority_communicate,
		_ai_communication_priority_shout,
		_ai_communication_priority_yell,
		_ai_communication_priority_exclaim,

		_ai_communication_priority,
	};

	enum //ai_communication_team
	{
		_ai_communication_team_unteamed = NONE,
		_ai_communication_team_human    = 0,
		_ai_communication_covenant,

		k_number_of_ai_communication_teams
	};

	enum {
		_ai_communication_hostility_type_none,
		_ai_communication_hostility_type_self,
		_ai_communication_hostility_type_friend,
		_ai_communication_hostility_type_enemy,
		_ai_communication_hostility_type_traitor,

		k_number_of_ai_communication_hostility_types
	};
	enum {
		_ai_communication_status_type_never,
		_ai_communication_status_type_dead,
		_ai_communication_status_type_lost,
		_ai_communication_status_type_not_visual,
		_ai_communication_status_type_no_danger,
		_ai_communication_status_type_visual,

		k_number_of_ai_communication_status_types,
	};
};


namespace Yelo::Flags {
	enum actor_variant_transform_collection_transform_flags {
		_actor_variant_transform_collection_transform_flags_scripted_only,

		_actor_variant_transform_collection_transform_flags
	};

	enum actor_variant_transform_in_target_flags {
		_actor_variant_transform_in_target_flags_try_to_use_existing_unit,
		_actor_variant_transform_in_target_flags_drop_weapon,
		_actor_variant_transform_in_target_flags_inherit_seated_units,
		_actor_variant_transform_in_target_flags_delete_attached_actors,

		_actor_variant_transform_in_target_flags
	};

	enum actor_variant_transform_out_attachment_flags {
		_actor_variant_transform_out_attachment_flags_destroy_attachments_on_death,

		_actor_variant_transform_out_attachment_flags
	};

	enum actor_variant_transform_out_flags {
		_actor_variant_transform_out_flags_invicible_during_transform_out_bit,

		_actor_variant_transform_out_flags
	};

	enum actor_variant_transform_out_criteria_flags {
		_actor_variant_transform_out_criteria_flags_transform_on_damage_only_bit,
		_actor_variant_transform_out_criteria_flags_ignore_friendly_fire_bit,
		_actor_variant_transform_out_criteria_flags_transform_on_actor_action_bit,
		_actor_variant_transform_out_criteria_flags_transform_on_actor_state_bit,
		_actor_variant_transform_out_criteria_flags_transform_on_shield_range_bit,
		_actor_variant_transform_out_criteria_flags_transform_on_health_range_bit,

		_actor_variant_transform_out_criteria_flags
	};
};
