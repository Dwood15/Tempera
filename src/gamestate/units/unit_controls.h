#pragma once
#include <cstddef>
#include "macros_generic.h"
#include "../../math/real_math.h"

struct s_persistent_control {
	long          ticks_remaining;                              // 0x210
	unsigned long flags;                                 // 0x214
};

struct control_state {
	/** Unit is pressing the crouch button. */
	ushort crouch_button : 1;

	/** Unit is pressing the jump button. */
	ushort jump_button : 1;

	ushort unkA : 2;

	/** Unit is pressing the flashlight button. */
	ushort flashlight_button : 1;

	ushort unkB : 1;

	/** Unit is pressing the action button. */
	ushort action_button : 1;

	/** Unit is pressing the melee button. */
	ushort melee_button : 1;

	ushort unkC :  2;

	/** Unit is pressing the reload button. */
	ushort reload_button : 1;

	/** Unit is pressing the primary fire button. */
	ushort primary_fire_button : 1;

	/** Unit is pressing the primary fire button. */
	ushort secondary_fire_button : 1;

	/** Unit is pressing the grenade button. */
	ushort grenade_button : 1;

	/** Unit is holding the exchange weapon button or is holding down the action button. */
	ushort exchange_weapon_button : 1;

	ushort unkD : 1;
};
STAT_ASSERT(control_state, sizeof(ushort));

union unit_control_flags {
	ushort        control_flags_a;
	control_state control_flags;
};
STAT_ASSERT(unit_control_flags, sizeof(short));

struct s_unit_control_data {
	::std::byte               animation_state;
	::std::byte               aiming_speed;
	unit_control_flags control_flags;
	short              weapon_index;
	short              grenade_index;
	short              zoom_index;
	unsigned short:16;
	real_vector3d       throttle;
	real                primary_trigger;
	real_vector3d       facing_vector;
	real_vector3d       aiming_vector;
	real_euler_angles3d looking_vector;
};

STAT_ASSERT(s_unit_control_data, 0x40);

struct s_player_action {
	unit_control_flags control_flagsA;
	unit_control_flags control_flagsB;
	float              desired_facing_yaw;
	float              desired_facing_pitch;
	float              throttle_forwardback;
	float              throttle_leftright;
	float              primary_trigger;
	short            desired_weapon_index;
	short            desired_grenade_index;
	short            desired_zoom_index;
	short            field_1E;
};


