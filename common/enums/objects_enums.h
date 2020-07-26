#pragma once

namespace Yelo::Enums {
	enum unit_speech_priority : short {
		_unit_speech_none,
		_unit_speech_idle,
		_unit_speech_pain,
		_unit_speech_talk,
		_unit_speech_communicate,
		_unit_speech_shout,
		_unit_speech_script,
		_unit_speech_involuntary,
		_unit_speech_exclaim,
		_unit_speech_scream,
		_unit_speech_death,

		k_number_of_unit_speech_priorities, // NUMBER_OF_UNIT_SPEECH_PRIORITIES
	};

	enum unit_scream_type : short {
		_unit_scream_type_fear,
		_unit_scream_type_enemy_grenade, // _dialogue_vocalization_hurt_enemy_grenade
		_unit_scream_type_pain,
		_unit_scream_type_maimed_limb,
		_unit_scream_type_mained_head,
		_unit_scream_type_resurrection,

		k_number_of_unit_scream_types, // NUMBER_OF_UNIT_SCREAM_TYPES
	};

	enum unit_camo_regrowth : short {
		_unit_camo_regrowth_off,
		_unit_camo_regrowth_on, // they fired their weapon, requiring active_camo_regrowth_rate to be applied
	};

	enum powered_seat {
		_powered_seat_driver,
		_powered_seat_gunner,

		k_number_of_powered_seats
	};

	enum group_tags : unsigned int {
		sbsp_group_tag = 'sbsp'
	};
};

namespace Yelo::Flags {

	enum object_definition_flags : unsigned short
	{
		_object_definition_does_not_cast_shadow_bit,
		_object_definition_transparent_self_occlusion_bit,
		_object_definition_brighter_than_it_should_bit,
		_object_definition_not_pathfinding_obstacle_bit,

		k_number_of_object_definition_flags,

		_object_definition_cast_shadow_by_default_yelo_bit = k_number_of_object_definition_flags,

		k_number_of_object_definition_flags_yelo,
	};

	enum object_damage_flags {
		_object_damage_flag_body_depleted      = 1 << 0,
		_object_damage_flag_destroyed_a_region = 1 << 1,
		_object_damage_flag_destroyed_object   = 1 << 2,
		_object_damage_flag_shield_depleted    = 1 << 3,
		_object_damage_flag_is_enemy_team      = 1 << 4,
		_object_damage_flag_unknown5           = 1 << 5,
		_object_damage_flag_unknown6           = 1 << 6,
		_object_damage_flag_unknown7           = 1 << 7,

		_object_damage_flags
	};

	enum damage_data_flags : short {
		// Incomplete
			_damage_data_flags_affect_target_only_bit,

		_damage_data_flags
	};

	enum {
		// --- 0x10

		_object_hidden_bit = 0,
		_object_is_on_the_ground_bit,
		_object_unk2_bit,
		_object_is_in_water_bit,
		_object_unk4_bit,
		_object_at_reset_bit, // when set, physics aren't updated
		_object_unk6_bit,
		_object_unk7_bit,
		_object_unk8_bit,
		_object_unk9_bit,
		_object_unk10_bit, // set when the object has a looping sound attachment
		_object_connected_to_map_bit,
		_object_not_placed_automatically_bit,
		_object_unk13_bit, // set in device_machine's 'new' function, always
		_object_unk14_bit, // set in device_machine's 'new' function, if _machine_is_elevator_bit is true
		_object_unk15_bit, // set in device_machine's 'new' function, if _machine_is_elevator_bit is true
		_object_garbage_bit,
		_object_unk17_bit, // unit and weapon 'place' functions set this, but I don't see any code which tests for it
		_object_does_not_cast_shadow_bit,
		_object_deactivation_is_deletion_bit, // instead of deactivating (and persisting), the object is deleted
		_object_unk20_bit, // prohibits the object from ever being activated again
		_object_outside_map_bit,
		_object_beautify_bit,
		_object_limping_bit, // used with _biped_limping_bit; node matrices won't be updated
		_object_collideable_bit,
		_object_has_collision_model_bit,
		_object_unk26_bit, // HaloPC, message delta related. see object_type_should_force_baseline_update
		_object_unk27_bit, // HaloPC, message delta related. see *_process_update_delta
		_object_yelo_is_transforming_out_bit,
		_object_yelo_is_transforming_in_bit,
		//_object_30_bit,
		//_object_31_bit,

		// --- 0x106

		// These are set when the damage thresholds are reached and the game applies the
		// respective s_damage_resistance's damage_effect
			_object_body_damage_effect_applied_bit = 0,
			_object_shield_damage_effect_applied_bit,
		// test this bit if you want to see if the unit is_dead
			_object_body_depleted_bit, // if this is set, _hud_draw_element_1_bit will be set when the unit interface is rendered
			_object_shield_depleted_bit, // if this is set, _hud_draw_element_0_bit will be set when the unit interface is rendered
			_object_damage_unk4_bit, // shield related
			_object_killed_bit,
			_object_killed_silent_bit,
			_object_melee_attack_inhibited_bit,
			_object_damage_unk8_bit, // unused?
			_object_damage_unk9_bit, // unused?
			_object_damage_unk10_bit, // unused?
			_object_cannot_take_damage_bit,
			_object_shield_recharging_bit,
			_object_killed_no_statistics_bit,
	};
}

constexpr int MAX_ATTACHMENTS_PER_OBJECT = 8;
