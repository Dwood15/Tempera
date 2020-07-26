#pragma once
#include <macros_generic.h>
#include <enums/generic_enums.h>
#include "../../tags/group/tagreference.h"
#include "../objects/units/unit_camera.h"
#include "../../tags/tags.h"
#include "../../tags/group/markup.h"
#include "../../game/objects/object_definitions.h"
#include "extended_definitions.h"

namespace Yelo::TagGroups {
	struct unit_seat_acceleration {
		real_vector3d seat_acceleration_scale;
		unsigned long:32;;
		unsigned long:32;;
		unsigned long:32;;
	};

	struct unit_hud_reference {
		tag_reference unit_hud_interface;
		int32 padint328[8]; // 32
	};

	struct dialogue_variant_definition {
		int16 varient_number;
		short : 16;
		unsigned long:32;;
		tag_reference dialogue;
	};

	struct powered_seat_definition {
		unsigned long:32;;
		real driver_powerup_time;
		real driver_powerdown_time;
		int32 padint3214[14]; // 56
	};

	struct unit_initial_weapon {
		tag_reference weapon;
		int32 padint324[4]; // 20
	};

	struct unit_seat {
		long_flags flags;
		tag_string label;
		tag_string marker_name;
		tag_string padtag_string1[1]; // 32

		unit_seat_acceleration acceleration;

		real yaw_rate;
		real pitch_rate;

		s_unit_camera unit_camera;
		TagBlock<unit_hud_reference> unit_hud_interface;

		int32 padint321[1]; // 4
		int16 hud_text_message_index;
		short : 16;

		real yaw_min;
		real yaw_max;

		tag_reference built_in_gunner;

		TagBlock<unit_seat_extensions> seat_extensions;
		int32 padint322[2]; // 8

	private:
		bool Postprocess(Enums::tag_postprocess_mode mode,
							  datum_index tag_index);

	public:
		static bool Postprocess(void *element, Enums::tag_postprocess_mode mode, datum_index tag_index);
	};

	STAT_ASSERT(unit_seat, 284 );

	//////////////////////////////////////////////////////////////////////////
	struct _unit_definition {
		long_flags flags;
		short default_team;
		short constant_sound_volume;

		real rider_damage_fraction;

		tag_reference integrated_light_toggle;

		short function_exports[k_number_of_incoming_object_functions]; // Enums::unit_function_mode

		real camera_fov;
		real camera_stiffness;
		s_unit_camera unit_camera;

		unit_seat_acceleration acceleration;

		real soft_ping_threshold;
		real soft_ping_interrupt_time;

		real hard_ping_threshold;
		real hard_ping_interrupt_time;

		real hard_death_threshold;
		real feign_death_threshold;
		real feign_death_time;

		real distance_of_evade_anim;
		real distance_of_dive_anim;
		unsigned long:32;;

		real stunned_movement_threshold;
		real feign_death_chance;
		real feign_repeat_chance;

		tag_reference spawned_actor;
		short_bounds spawned_actor_count;
		real spawned_velocity;

		real aiming_velocity_max;
		real aiming_acceleration_max;
		real casual_aiming_modifier;

		real looking_velocity_max;
		real looking_acceleration_max;
		unsigned long:32;;
		unsigned long:32;;

		real ai_vehicle_radius;
		real ai_danger_radius;

		tag_reference melee_damage;

		short blip_type;
		short : 16;

		TagBlock<unit_extensions> extensions;

		TagBlock<unit_hud_reference> new_hud_interfaces;
		TagBlock<dialogue_variant_definition> dialogue_variants;

		real grenade_velocity;
		short grenade_type;
		int16 grenade_count;

		struct { // tag value * 30f
			int16 soft;
			int16 hard;
		}       runtime_ping_interrupt_time;

		TagBlock<powered_seat_definition> powered_seats;
		TagBlock<unit_initial_weapon> weapons;
		TagBlock<unit_seat> seats;

		bool Postprocess(Enums::tag_postprocess_mode mode, datum_index tag_index);

	};

	STAT_ASSERT(_unit_definition, 0x174 );

	struct s_unit_definition : s_object_definition {
		enum { k_group_tag = 'unit' };

		_unit_definition unit;

		bool Postprocess(Enums::tag_postprocess_mode mode, datum_index tag_index) {
			return unit.Postprocess(mode, tag_index);
		}
	};
};
