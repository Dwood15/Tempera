#pragma once

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

namespace Flags {
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
};
