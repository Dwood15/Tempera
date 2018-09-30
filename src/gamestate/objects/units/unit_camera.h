#pragma once

#include "../../../tags/group/tagreference.h"

namespace Yelo::TagGroups {
	struct s_unit_camera_track {
		Yelo::tag_reference track;
		unsigned long : 32;
		unsigned long : 32;
		unsigned long : 32;
	}; static_assert(sizeof(s_unit_camera_track) == 0x1C);

	struct s_unit_camera {
		tag_string camera_marker_name;
		tag_string camera_submerged_marker_name;
		real pitch_autolevel;
		real_bounds pitch_range;
		TagBlock<s_unit_camera_track> camera_tracks;
	}; static_assert(sizeof(s_unit_camera) == 0x58);

	struct s_unit_camera_info
	{
		datum_index unit_index;
		short seat_index;
		unsigned short : 16;
		s_unit_camera* unit_camera_definition;
		real_point3d position;
	}; static_assert( sizeof(s_unit_camera_info) == 0x18 );
};

