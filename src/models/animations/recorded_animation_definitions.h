#pragma once

#include <macros_generic.h>
#include "../../tags/group/tagdata.h"

namespace Yelo::TagGroups {
	struct recorded_animation_definition {
		tag_string name;
		byte       version; // a Enums::e_recorded_animation_version, with +1  (to support NONE)
		sbyte      raw_animation_data;
		sbyte      unit_control_data_version;
		unsigned char : 8;
		short length_of_animation;
		unsigned short : 16;
		unsigned long  : 32;
		Yelo::tag_data event_stream;

		inline byte_enum ToRecordedAnimationVersion() const { return static_cast<byte_enum>( version) - 1; }
	};

	STAT_ASSERT(recorded_animation_definition, 0x40);
};

// namespace Scripting
// {
//unit_control_v1
//0  byte, animation_state
//1  byte, aiming_speed
//2  word, control_flags
//4  word, weapon_index
//-1 word
//C  real_vector2d, throttle
//1C real_vector3d, facing_vector
//28 real_vector3d, aiming_vector
//34 real_vector3d, looking_vector
//unit_control_v2
//18 long, primary_trigger
//unit_control_v3
//6  word, grenade_index
//unit_control_v4
//8  word, zoom_index
// };
