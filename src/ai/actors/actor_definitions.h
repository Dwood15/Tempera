#pragma once

#include "../../math/real_math.h"
#include "../../math/int_math.h"
#include "../../math/colors.h"
#include "macros_generic.h"
#include "../../tags/group/tagblock.h"
#include "../../tags/group/tagreference.h"

namespace Yelo::TagGroups {
	struct s_actor_definition {
		enum { k_group_tag = 'actr' };

		struct __flags {
			unsigned long can_see_in_darkness_bit:1;
			unsigned long sneak_uncovering_target_bit:1;
			unsigned long sneak_uncovering_pursuit_position_bit:1;
			unsigned long unused_bit:1;
			unsigned long shoot_at_target_s_last_location_bit:1;
			unsigned long try_to_stay_still_when_crouched_bit:1;
			unsigned long crouch_when_not_in_combat_bit:1;
			unsigned long crouch_when_guarding_bit:1;
			unsigned long unused_1_bit:1;
			unsigned long must_crouch_to_shoot_bit:1;
			unsigned long panic_when_surprised_bit:1;
			unsigned long always_charge_at_enemies_bit:1;
			unsigned long gets_in_vehicles_with_player_bit:1;
			unsigned long start_firing_before_aligned_bit:1;
			unsigned long standing_must_move_forward_bit:1;
			unsigned long crouching_must_move_forward_bit:1;
			unsigned long defensive_crouch_while_charging_bit:1;
			unsigned long use_stalking_behavior_bit:1;
			unsigned long stalking_freeze_if_exposed_bit:1;
			unsigned long always_berserk_in_attacking_mode_bit:1;
			unsigned long berserking_uses_panicked_movement_bit:1;
			unsigned long flying_bit:1;
			unsigned long panicked_by_unopposable_enemy_bit:1;
			unsigned long crouch_when_hiding_from_unopposable_bit:1;
			unsigned long always_charge_in_attacking_mode_bit:1;
			unsigned long dive_off_ledges_bit:1;
			unsigned long swarm_bit:1;
			unsigned long suicidal_melee_attack_bit:1;
			unsigned long cannot_move_while_crouching_bit:1;
			unsigned long fixed_crouch_facing_bit:1;
			unsigned long crouch_when_in_line_of_fire_bit:1;
			unsigned long avoid_friends_line_of_fire_bit:1;
		};

		STAT_ASSERT(__flags, sizeof(unsigned long));

		struct __more_flags {
			unsigned long avoid_all_enemy_attack_vectors_bit:1;
			unsigned long must_stand_to_fire_bit:1;
			unsigned long must_stop_to_fire_bit:1;
			unsigned long disallow_vehicle_combat_bit:1;
			unsigned long pathfinding_ignores_danger_bit:1;
			unsigned long panic_in_groups_bit:1;
			unsigned long no_corpse_shooting_bit:1;
		};
		STAT_ASSERT(__more_flags, sizeof(unsigned long));

		__flags      flags;
		__more_flags more_flags;
		long         pad00[3];
		short        type;
		unsigned short : 16;

		////////////////////////////////////////////////////////////////
		// perception
		real  max_vision_distance;
		angle central_vision_angle;
		angle max_vision_angle;
		unsigned long : 32;
		angle peripheral_vision_angle;
		real  peripheral_distance;
		unsigned long : 32;
		real_vector3d standing_gun_offset;
		real_vector3d crouching_gun_offset;
		real          hearing_distance;
		real          notice_projectile_chance;
		real          notice_vehicle_chance;
		long          pad0[2];
		real          combat_perception_time;
		real          guard_perception_time;
		real          non_combat_perception_time;
		long          pad1[3];
		long          pad2[2];

		////////////////////////////////////////////////////////////////
		// movement
		real  dive_into_cover_chance;
		real  emerge_from_cover_chance;
		real  dive_from_grenade_chance;
		real  pathfinding_radius;
		real  glass_ignorance_chance;
		real  stationary_movement_dist;
		real  free_flying_sidestep;
		angle begin_moving_angle;
		unsigned long : 32;

		////////////////////////////////////////////////////////////////
		// looking
		real_euler_angles2d maximum_aiming_deviation;
		real_euler_angles2d maximum_looking_deviation;
		angle               noncombat_look_delta_l;
		angle               noncombat_look_delta_r;
		angle               combat_look_delta_l;
		angle               combat_look_delta_r;
		real_euler_angles2d idle_aiming_range;
		real_euler_angles2d idle_looking_range;
		real_bounds         event_look_time_modifier;
		real_bounds         noncombat_idle_facing;
		real_bounds         noncombat_idle_aiming;
		real_bounds         noncombat_idle_looking;
		real_bounds         guard_idle_facing;
		real_bounds         guard_idle_aiming;
		real_bounds         guard_idle_looking;
		real_bounds         combat_idle_facing;
		real_bounds         combat_idle_aiming;
		real_bounds         combat_idle_looking;
		long                pad3[2];
		long                pad4[4];
		Yelo::tag_reference do_not_use;
		long                pad5[67];
		Yelo::tag_reference do_not_use_1;

		////////////////////////////////////////////////////////////////
		// unopposable
		short unreachable_danger_trigger;
		short vehicle_danger_trigger;
		short player_danger_trigger;
		unsigned short : 16;
		real_bounds danger_trigger_time;
		short       friends_killed_trigger;
		short       friends_retreating_trigger;
		long        pad6[3];
		real_bounds retreat_time;
		long        pad7[2];

		////////////////////////////////////////////////////////////////
		// panic
		real_bounds cowering_time;
		real        friend_killed_panic_chance;
		short       leader_type;
		unsigned short : 16;
		real leader_killed_panic_chance;
		real panic_damage_threshold;
		real surprise_distance;
		long pad8[7];

		////////////////////////////////////////////////////////////////
		// defensive
		// danger values: being aimed at: 0.7
		// an enemy shooting in our general direction: 1.2
		// an enemy shooting directly at us: 1.6
		// an enemy damaging us: 2.0
		real_bounds hide_behind_cover_time;
		real        hide_target_not_visible_time;
		real        hide_shield_fraction;
		real        attack_shield_fraction;
		real        pursue_shield_fraction;
		long        pad9[4];
		short       defensive_crouch_type;
		unsigned short : 16;
		real  attacking_crouch_threshold;
		real  defending_crouch_threshold;
		real  min_stand_time;
		real  min_crouch_time;
		real  defending_hide_time_modifier;
		real  attacking_evasion_threshold;
		real  defending_evasion_threshold;
		real  evasion_seek_cover_chance;
		real  evasion_delay_time;
		real  max_seek_cover_distance;
		real  cover_damage_threshold;
		real  stalking_discovery_time;
		real  stalking_max_distance;
		angle stationary_facing_angle;
		real  change_facing_stand_time;
		unsigned long : 32;

		////////////////////////////////////////////////////////////////
		// pursuit
		real_bounds uncover_delay_time;
		real_bounds target_search_time;
		real_bounds pursuit_position_time;
		short       num_positions;
		short       num_positions_1;
		long        padA[8];

		////////////////////////////////////////////////////////////////
		// berserk
		real        melee_attack_delay;
		real        melee_fudge_factor;
		real        melee_charge_time;
		real_bounds melee_leap_range;
		real        melee_leap_velocity;
		real        melee_leap_chance;
		real        melee_leap_ballistic;
		real        berserk_damage_amount;
		real        berserk_damage_threshold;
		real        berserk_proximity;
		real        suicide_sensing_dist;
		real        berserk_grenade_chance;
		long        padB[3];

		////////////////////////////////////////////////////////////////
		// firing positions
		real_bounds guard_position_time;
		real_bounds combat_position_time;
		real        old_position_avoid_dist;
		real        friend_avoid_dist;
		long        padC[10];

		////////////////////////////////////////////////////////////////
		// communication
		real_bounds         noncombat_idle_speech_time;
		real_bounds         combat_idle_speech_time;
		long                padD[12];
		long                padE[32];
		Yelo::tag_reference do_not_use_2;
		long                padF[12];
	}; STAT_ASSERT(s_actor_definition, 0x4F8); // max count: 1

	struct s_actor_variant_change_colors {
		real_rgb_color color_lower_bound;
		real_rgb_color color_upper_bound;
		long           pad[2];
	};
	STAT_ASSERT(s_actor_variant_change_colors, 0x20); // max count: 4

	struct s_actor_variant_definition {
		enum { k_group_tag = 'actv' };

		struct __flags {
			unsigned long can_shoot_while_flying_bit:1;
			unsigned long interpolate_color_in_hsv_bit:1;
			unsigned long has_unlimited_grenades_bit:1;
			unsigned long moveswitch_stay_w_friends_bit:1;
			unsigned long active_camouflage_bit:1;
			unsigned long super_active_camouflage_bit:1;
			unsigned long cannot_use_ranged_weapons_bit:1;
			unsigned long prefer_passenger_seat_bit:1;
		};
		STAT_ASSERT(__flags, sizeof(unsigned long))

		__flags             flags;
		Yelo::tag_reference actor_definition;
		Yelo::tag_reference unit;
		Yelo::tag_reference major_variant;
		long                pad000[6];

		////////////////////////////////////////////////////////////////
		// movement switching
		// note: only the flood combat forms will ever try to switch movement types voluntarily during combat
		short movement_type;
		unsigned short : 16;
		real        initial_crouch_chance;
		real_bounds crouch_time;
		real_bounds run_time;

		////////////////////////////////////////////////////////////////
		// ranged combat
		Yelo::tag_reference weapon;
		real                maximum_firing_distance;
		real                rate_of_fire;
		angle               projectile_error;
		real_bounds         first_burst_delay_time;
		real                new_target_firing_pattern_time;
		real                surprise_delay_time;
		real                surprise_fire_wildly_time;
		real                death_fire_wildly_chance;
		real                death_fire_wildly_time;
		real_bounds         desired_combat_range;
		real_vector3d       custom_stand_gun_offset;
		real_vector3d       custom_crouch_gun_offset;
		real                target_tracking;
		real                target_leading;
		real                weapon_damage_modifier;
		real                damage_per_second;

		////////////////////////////////////////////////////////////////
		// burst geometry
		// at the start of every burst we pick a random point near the target to fire at, on either the left or the right side.
		// the burst origin angle controls whether this error is exactly horizontal or might have some vertical component.
		//
		// over the course of the burst we move our projectiles back in the opposite direction towards the target. this return motion is also controlled by an angle that specifies how close to the horizontal it is.
		//
		// for example if the burst origin angle and the burst return angle were both zero, and the return length was the same as the burst length, every burst would start the same amount away from the target (on either the left or right) and move back to exactly over the target at the end of the burst.
		real        burst_origin_radius;
		angle       burst_origin_angle;
		real_bounds burst_return_length;
		angle       burst_return_angle;
		real_bounds burst_duration;
		real_bounds burst_separation;
		angle       burst_angular_velocity;
		unsigned long : 32;
		real  special_damage_modifier;
		angle special_projectile_error;

		////////////////////////////////////////////////////////////////
		// firing patterns
		// a firing pattern lets you modify the properties of an actor's burst geometry. actors choose which firing pattern to use based on their current state:
		//      'new-target' when the target just appeared
		//      'moving' when the actor is moving
		//      'berserk' if the actor is berserk
		// if none of these apply, no firing pattern is used.
		//
		// the default values in the burst geometry are multiplied by any non-zero modifiers in the firing pattern.
		real new_target_burst_duration;
		real new_target_burst_separation;
		real new_target_rate_of_fire;
		real new_target_projectile_error;
		long pad01[2];
		real moving_burst_duration;
		real moving_burst_separation;
		real moving_rate_of_fire;
		real moving_projectile_error;
		long pad02[2];
		real berserk_burst_duration;
		real berserk_burst_separation;
		real berserk_rate_of_fire;
		real berserk_projectile_error;
		long pad03[2];

		////////////////////////////////////////////////////////////////
		// special-case firing properties
		real  super_ballistic_range;
		real  bombardment_range;
		real  modified_vision_range;
		short special_fire_mode;
		short special_fire_situation;
		real  special_fire_chance;
		real  special_fire_delay;

		////////////////////////////////////////////////////////////////
		// berserking and melee
		real        melee_range;
		real        melee_abort_range;
		real_bounds berserk_firing_ranges;
		real        berserk_melee_range;
		real        berserk_melee_abort_range;
		long        pad04[2];

		////////////////////////////////////////////////////////////////
		// grenades
		short grenade_type;
		short trajectory_type;
		short grenade_stimulus;
		short minimum_enemy_count;
		real  enemy_radius;
		unsigned long : 32;
		real          grenade_velocity;
		real_bounds   grenade_ranges;
		real          collateral_damage_radius;
		real_fraction grenade_chance;
		real          grenade_check_time;
		real          encounter_grenade_timeout;
		long          pad05[5];

		////////////////////////////////////////////////////////////////
		// items
		Yelo::tag_reference equipment;
		short_bounds        grenade_count;
		real                dont_drop_grenades_chance;
		real_bounds         drop_weapon_loaded;
		short_bounds        drop_weapon_ammo;
		long                pad06[3];
		long                pad07[4];

		////////////////////////////////////////////////////////////////
		// unit
		real  body_vitality;
		real  shield_vitality;
		real  shield_sapping_radius;
		short forced_shader_permutation;
		unsigned short : 16;
		long                                          pad08[4];
		tag_block                             pad09;
		Yelo::TagBlock<s_actor_variant_change_colors> change_colors;
	};

	STAT_ASSERT(s_actor_variant_definition, 0x238); // max count: 1
};
