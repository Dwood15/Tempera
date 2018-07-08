#pragma once

#include "../../../tags/group/tag_groups_base_yelo.hpp"

namespace Yelo::TagGroups {
	struct s_unit_camera_track {
		tag_reference track;
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
};

