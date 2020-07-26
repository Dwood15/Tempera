#pragma once

#include <macros_generic.h>
#include <enums/unit_enums.h>
#include "../../tags/group/tagreference.h"
#include "../objects/units/unit_camera.h"

namespace Yelo::TagGroups {
	using namespace Yelo::Enums;
	using namespace Yelo::Flags;

	struct unit_mounted_state_keyframe_action {
		unit_animation_keyframe                  keyframe;
		ushort                                   flags;
		unit_seat_mounted_keyframe_action_target target;
		short : 16;
		tag_reference damage_effect;
		tag_reference effect;
		tag_string    effect_marker;
		tag_block     pad2tag_block[2];
	}; STAT_ASSERT(unit_mounted_state_keyframe_action, 0x60);

	struct unit_mounted_state {
		ushort flags;
		short : 16;
		s_unit_camera                                unit_camera;
		TagBlock<unit_mounted_state_keyframe_action> keyframe_actions;;
		tag_block pad2tag_block[2];
	}; STAT_ASSERT(unit_mounted_state, 0x80);

	struct unit_extensions {
		TagBlock<unit_mounted_state> mounted_state;;
		tag_block pad4tag_block[4];
	}; STAT_ASSERT(unit_extensions, 0x3C);


	struct unit_seat_keyframe_action {
		unit_animation_keyframe                           keyframe;
		ushort                                            flags;
		unit_seat_keyframe_action_self_seat_action        self_seat_action;
		unit_seat_keyframe_action_target_seat_unit_action target_seat_unit_action;
		unit_seat_keyframe_action_unit_door_action        unit_door_action;
		short : 16;

		unit_seat_keyframe_action_apply_damage_effect apply_damage_to;
		int16                                         region_index;
		tag_string                                    region_name;
		tag_reference                                 damage_effect;

		unit_seat_keyframe_action_apply_effect apply_effect_to;
		short : 16;
		tag_reference effect;
		tag_string    effect_marker;
		tag_block     pad3tag_block[3];
	}; STAT_ASSERT(unit_seat_keyframe_action, 0x98);

	struct unit_seat_boarding {
		unit_seat_boarding_type                      boarding_type;
		ushort                                       delay_until;
		unit_seat_boarding_vitality_threshold_source unit_vitality_source;
		short : 16;
		real_fraction unit_shield_threshold;
		real_fraction unit_health_threshold;
		short : 16;
		int16                               region_index;
		tag_string                          region_name;
		tag_block                           pad2tag_block[2];
		TagBlock<unit_seat_keyframe_action> keyframe_actions;;
		tag_block pad2tag_blockB[2];
	}; STAT_ASSERT(unit_seat_boarding, 0x70);

	struct unit_seat_damage {
		ushort                   flags;
		unit_seat_damage_melee   melee;
		tag_reference            melee_damage_effect;
		unit_seat_damage_grenade grenade;
		ushort                   disabled_grenade_types;
		real                     grenade_detonation_time_scale;
		tag_string               grenade_marker;
		ushort                   region_flags;
		int16                    region_index;
		tag_string               region_name;
		tag_reference            region_damage_effect;
		tag_block                pad2tag_block[2];
	}; STAT_ASSERT(unit_seat_damage, 0x88);

	struct unit_seat_access {
		ushort flags;
		short : 16;
		angle         unit_sight_angle;
		tag_string    unit_sight_marker;
		angle         mounting_unit_sight_angle;
		tag_string    mounting_unit_sight_marker;
		real_fraction unit_shield_threshold;
		real_fraction unit_health_threshold;
		ushort        permitted_ai_states;
		short : 16;
		tag_block pad3tag_block[3];
	}; STAT_ASSERT(unit_seat_access, 0x7C);

	struct unit_seat_extensions {
		ushort          flags;
		int16           target_seat_index;
		TagBlock<int16> seat_targeting_seats;;
		TagBlock<unit_seat_access> seat_access;;
		TagBlock<unit_seat_boarding> seat_boarding;;
		TagBlock<unit_seat_damage> seat_damage;;
		tag_block pad4tag_block[4];
	}; STAT_ASSERT(unit_seat_extensions, 0x64);
};
