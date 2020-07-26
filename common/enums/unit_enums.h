#pragma once

#include "macros_generic.h"

namespace Yelo::Enums {
	constexpr int k_maximum_weapons_per_unit = 4;

	enum unit_base_seat {
		_unit_base_seat_asleep,
		_unit_base_seat_alert,

		_unit_base_seat_stand,
		_unit_base_seat_crouch,
		_unit_base_seat_flee,
		_unit_base_seat_flaming,
		k_number_of_unit_base_seats,
	};

	enum unit_base_weapon {
		_unit_base_weapon_unarmed,

		k_number_of_unit_base_weapons,
	};
};

namespace Yelo::Flags {
	enum unit_definition_flags {
		_unit_circular_aiming_bit,
		_unit_destroyed_after_dying_bit,
		_unit_half_speed_interpolation_bit,
		_unit_fires_from_camera_bit,
		_unit_entrance_inside_bounding_sphere_bit,
		_unit_unused5_bit,                     // Halo2: 'doesn't show readied weapon'
		_unit_causes_passenger_dialogue_bit,
		_unit_resists_pings_bit,
		_unit_melee_attack_is_fatal_bit,
		_unit_dont_reface_during_pings_bit,
		_unit_has_no_aiming_bit,
		_unit_simple_creature_bit,
		_unit_impact_melee_attaches_to_unit_bit,
		_unit_impact_melee_dies_on_shields_bit,
		_unit_cannot_open_doors_automaticlly_bit,
		_unit_melee_attackers_cannot_attach_bit,
		_unit_not_instantly_killed_by_melee_bit,
		_unit_shield_sapping_bit,
		_unit_runs_around_flaming_bit,
		_unit_inconsequential_bit,
		_unit_special_cinematic_unit_bit,
		_unit_ignored_by_autoaiming_bit,
		_unit_shields_fry_infection_forms_bit,
		_unit_integrated_light_controls_weapon_bit,   // Halo2: unused
		_unit_integrated_light_lasts_forever_bit,   // Halo2: unused
		_unit_reserved25_bit,                  // Halo2: acts as gunner for parent
		_unit_reserved26_bit,                  // Halo2: controlled by parent gunner
		_unit_reserved27_bit,                  // Halo2: parent's primary weapon
		_unit_reserved28_bit,                  // Halo2: unit has boost
		_unit_unused29_bit,
		_unit_unused30_bit,
		_unit_has_boarding_seats_yelo_bit,         // YELO: set by postprocess proc, for runtime

		k_number_of_unit_definition_flags
	};

	static_assert(k_number_of_unit_definition_flags <= (sizeof(long_flags) * 8));

	enum unit_seat_definition_flags {
		_unit_seat_invisable_bit,
		_unit_seat_locked_bit,
		_unit_seat_driver_bit,
		_unit_seat_gunner_bit,
		_unit_seat_third_person_camera_bit,
		_unit_seat_allows_weapons_bit,
		_unit_seat_third_person_on_enter_bit,
		_unit_seat_first_person_camera_slaved_to_gun_bit,
		_unit_seat_allow_vehicle_communication_anims_bit,
		_unit_seat_not_valid_without_driver_bit,
		_unit_seat_allow_ai_noncombatants_bit,

		k_number_of_unit_seat_definition_flags,

		_unit_seat_reserved11_bit =            // Halo2
		k_number_of_unit_seat_definition_flags,
		_unit_seat_reserved12_bit,               // Halo2: ai firing disabled by max acceleration
		_unit_seat_reserved13_bit,      // Halo2
		_unit_seat_reserved14_bit,   // Halo2
		_unit_seat_reserved15_bit,      // Halo2
		_unit_seat_reserved16_bit,   // Halo2: invalid for player
		_unit_seat_reserved17_bit,   // Halo2: invalid for non-player
		_unit_seat_reserved18_bit,   // Halo2: gunner (player only)
		_unit_seat_reserved19_bit,   // Halo2: invisible under major damage
		_unit_seat_allows_melee_bit,   // YELO

		k_number_of_unit_seat_definition_flags_yelo,
	};
	static_assert(k_number_of_unit_seat_definition_flags_yelo <= (sizeof(long_flags) * 8));
};

enum unit_animation_keyframe : short {
	_unit_animation_keyframe_primary,
	_unit_animation_keyframe_secondary,
	_unit_animation_keyframe_final,

	_unit_animation_keyframe
};

enum unit_animation_state : signed char {
	_unit_animation_state_invalid = -1,

	_unit_animation_state_idle,
	_unit_animation_state_gesture,
	_unit_animation_state_turn_left,
	_unit_animation_state_turn_right,
	_unit_animation_state_move_front,
	_unit_animation_state_move_back,
	_unit_animation_state_move_left,
	_unit_animation_state_move_right,
	_unit_animation_state_stunned_front,
	_unit_animation_state_stunned_back,
	_unit_animation_state_stunned_left,
	_unit_animation_state_stunned_right,
	_unit_animation_state_slide_front,
	_unit_animation_state_slide_back,
	_unit_animation_state_slide_left,
	_unit_animation_state_slide_right,
	_unit_animation_state_ready,
	_unit_animation_state_put_away,
	_unit_animation_state_aim_still,
	_unit_animation_state_aim_move,
	_unit_animation_state_airborne,
	_unit_animation_state_land_soft,
	_unit_animation_state_land_hard,
	_unit_animation_state_unknown23,
	_unit_animation_state_airborne_dead,
	_unit_animation_state_landing_dead,
	_unit_animation_state_seat_enter,
	_unit_animation_state_seat_exit,
	_unit_animation_state_custom_animation,
	_unit_animation_state_impulse,
	_unit_animation_state_melee,
	_unit_animation_state_melee_airborne,
	_unit_animation_state_melee_continuous,
	_unit_animation_state_throw_grenade,
	_unit_animation_state_resurrect_front,
	_unit_animation_state_resurrect_back,
	_unit_animation_state_feeding,
	_unit_animation_state_surprise_front,   // TODO: open
	_unit_animation_state_surprise_back,   // TODO: close
	_unit_animation_state_leap_start,
	_unit_animation_state_leap_airborne,
	_unit_animation_state_leap_melee,
	_unit_animation_state_unknown42,      // unused AFAICT
	_unit_animation_state_berserk,

	_unit_animation_state,

	// custom unit animation states
		_unit_animation_state_yelo_seat_boarding = _unit_animation_state,
		_unit_animation_state_yelo_seat_ejecting,
		_unit_animation_state_yelo_unit_mounted,
		_unit_animation_state_yelo_unit_transforming,

	_unit_animation_state_yelo,
};

enum unit_replacement_animation_state : signed char {
	_unit_replacement_animation_state_none,
	_unit_replacement_animation_state_disarm,
	_unit_replacement_animation_state_weapon_drop,
	_unit_replacement_animation_state_weapon_ready,
	_unit_replacement_animation_state_weapon_put_away,
	_unit_replacement_animation_state_weapon_reload1,
	_unit_replacement_animation_state_weapon_reload2,
	_unit_replacement_animation_state_melee,
	_unit_replacement_animation_state_throw_grenade,

	k_number_of_unit_replacement_animation_states
};

enum unit_overlay_animation_state : signed char {
	_unit_overlay_animation_state_none,
	_unit_overlay_animation_state_fire_1,
	_unit_overlay_animation_state_fire_2,
	_unit_overlay_animation_state_charged_1,
	_unit_overlay_animation_state_charged_2,
	_unit_overlay_animation_state_chamber_1,
	_unit_overlay_animation_state_chamber_2,

	k_number_of_unit_overlay_animation_states
};

enum unit_camo_regrowth : signed short {
	_unit_camo_regrowth_off,
	_unit_camo_regrowth_on, // they fired their weapon, requiring active_camo_regrowth_rate to be applied
};

enum unit_throwing_grenade_state : signed char {
	_unit_throwing_grenade_state_none, // ie, no grenade
	_unit_throwing_grenade_state_begin,
	_unit_throwing_grenade_state_in_hand,
	_unit_throwing_grenade_state_released,
};

namespace Yelo::Flags {
	enum {
		_unit_animation_unk0_bit,
		_unit_animation_unk1_bit,
		_unit_animation_unk2_bit,
		_unit_animation_unk3_bit,
	};

	enum {
		_unit_unk0_bit,
		_unit_unk1_bit,
		_unit_unk2_bit,
		_unit_unk3_bit,
		_unit_powerup_on_bit,
		_unit_powerup_additional_bit,
		_unit_controllable_bit,
		_unit_berserking_bit,
		_unit_unk8_bit,
		_unit_unk9_bit,
		_unit_unk10_bit,
		_unit_unk11_bit,
		_unit_unk12_bit,
		_unit_unk13_bit,
		_unit_unk14_bit,
		_unit_unk15_bit,
		_unit_unk16_bit,
		_unit_unk17_bit,
		_unit_unk18_bit,
		_unit_unk19_bit, // integrated light related
		_unit_doesnt_drop_items_bit,
		_unit_unk21_bit,
		_unit_can_blink_bit,
		_unit_impervious_bit, // prevents unit from being knocked around or playing ping animations
		_unit_suspended_bit,
		_unit_blind_bit,
		_unit_unk26_bit, // when this is on, the integrated NV power increases. rate is 2x the speed it leaks when on
		_unit_possessed_bit,
		_unit_desires_flashlight_on_bit,
		_unit_desires_flashlight_off_bit,
		//			_unit_30_bit,
		//			_unit_31_bit,
	};

	enum unit_mounted_state_flags {
		_unit_mounted_state_flags_third_person_camera,

		_unit_mounted_state_flags
	};

	enum unit_mounted_state_keyframe_action_flags {
		_unit_mounted_state_keyframe_action_flags_eject_mounted_units,

		_unit_mounted_state_keyframe_action_flags
	};

	enum unit_seat_access_flags {
		_unit_seat_access_flags_requires_target_seat_occupied_bit,
		_unit_seat_access_flags_enemy_access_only_bit,
		_unit_seat_access_flags_restrict_by_unit_sight_bit,
		_unit_seat_access_flags_restrict_by_mounting_unit_sight_bit,
		_unit_seat_access_flags_restrict_by_unit_shield_bit,
		_unit_seat_access_flags_restrict_by_unit_health_bit,
		_unit_seat_access_flags_restrict_by_ai_state_bit,

		_unit_seat_access_flags
	};

	enum unit_seat_extensions_flags {
		_unit_seat_extensions_flags_triggers_mounted_state_bit,
		_unit_seat_extensions_flags_exit_on_unit_death_bit,
		_unit_seat_extensions_flags_exit_on_target_seat_empty_bit,
		_unit_seat_extensions_flags_prevent_death_when_unit_dies,
		_unit_seat_extensions_flags_ignored_by_seated_ai_bit,
		_unit_seat_extensions_flags_ignored_by_mounted_ai_bit,
		_unit_seat_extensions_flags_cant_enter_seats_when_occupied_bit,

		_unit_seat_extensions_flags
	};

	enum unit_seat_keyframe_action_flags {
		_unit_seat_keyframe_action_flags_control_powered_seat_bit,

		_unit_seat_keyframe_action_flags
	};

	enum unit_seat_boarding_delay_until_flags {
		_unit_seat_boarding_delay_until_flags_empty_target_seat_bit,
		_unit_seat_boarding_delay_until_flags_unit_shield_threshold_bit,
		_unit_seat_boarding_delay_until_flags_unit_health_threshold_bit,
		_unit_seat_boarding_delay_until_flags_region_destroyed_bit,

		_unit_seat_boarding_delay_until_flags
	};

	enum unit_seat_damage_flags {
		_unit_seat_damage_flags_use_weapon_melee_bit,
		_unit_seat_damage_flags_exit_after_grenade_plant_bit,

		_unit_seat_damage_flags
	};

	enum unit_seat_region_damage_flags {
		_unit_seat_region_damage_flags_disable_grenades_until_destroyed_bit,

		_unit_seat_region_damage_flags
	};
	
	enum unit_seat_mounted_keyframe_action_target : short
	{
		_unit_seat_mounted_keyframe_action_target_all_seats,
		_unit_seat_mounted_keyframe_action_target_mounted_trigger_seats,

		_unit_seat_mounted_keyframe_action_target
	};

	enum unit_seat_keyframe_action_self_seat_action : short
	{
		_unit_seat_keyframe_action_self_seat_action_none,
		_unit_seat_keyframe_action_self_seat_action_exit_current,
		_unit_seat_keyframe_action_self_seat_action_enter_target,

		_unit_seat_keyframe_action_seat_action_self
	};

	enum unit_seat_keyframe_action_target_seat_unit_action : short
	{
		_unit_seat_keyframe_action_target_seat_unit_action_none,
		_unit_seat_keyframe_action_target_seat_unit_action_exit_seat,
		_unit_seat_keyframe_action_target_seat_unit_action_eject_from_seat,

		_unit_seat_keyframe_action_seat_action_target
	};

	enum unit_seat_keyframe_action_unit_door_action : short
	{
		_unit_seat_keyframe_action_unit_door_action_neither,
		_unit_seat_keyframe_action_unit_door_action_open,
		_unit_seat_keyframe_action_unit_door_action_close,

		_unit_seat_keyframe_action_unit_door_action
	};

	enum unit_seat_keyframe_action_apply_damage_effect : short
	{
		_unit_seat_keyframe_action_apply_damage_effect_none,
		_unit_seat_keyframe_action_apply_damage_effect_mounted_unit,
		_unit_seat_keyframe_action_apply_damage_effect_mounted_unit_region,
		_unit_seat_keyframe_action_apply_damage_effect_seated_unit,

		_unit_seat_keyframe_action_apply_damage_effect
	};

	enum unit_seat_keyframe_action_apply_effect : short
	{
		_unit_seat_keyframe_action_apply_effect_none,
		_unit_seat_keyframe_action_apply_effect_mounted_unit,
		_unit_seat_keyframe_action_apply_effect_seated_unit,

		_unit_seat_keyframe_action_apply_effect
	};

	enum unit_seat_boarding_type : short
	{
		_unit_seat_boarding_type_immediate,
		_unit_seat_boarding_type_delayed,

		_unit_seat_boarding_type
	};

	enum unit_seat_boarding_vitality_threshold_source : short
	{
		_unit_seat_boarding_vitality_threshold_source_mounted_unit,
		_unit_seat_boarding_vitality_threshold_source_seated_unit,

		_unit_seat_boarding_vitality_threshold_source
	};

	enum unit_seat_damage_melee : short
	{
		_unit_seat_damage_melee_normal,
		_unit_seat_damage_melee_mounted_unit,
		_unit_seat_damage_melee_target_seat_unit,

		_unit_seat_damage_melee
	};

	enum unit_seat_damage_grenade : short
	{
		_unit_seat_damage_grenade_normal,
		_unit_seat_damage_grenade_disabled,
		_unit_seat_damage_grenade_plant_on_mounted_unit,
		_unit_seat_damage_grenade_plant_on_target_seat_unit,

		_unit_seat_damage_grenade
	};
};
