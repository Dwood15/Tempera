#pragma once

#include <precompile.h>

namespace Yelo::TagGroups {
	struct s_camera_track_control_point {
		real_vector3d   position;
		real_quaternion orientation;
		long:8 * sizeof(long) * 8;
	}; static_assert(sizeof(s_camera_track_control_point) == 0x3C); // max count: 16

	struct s_camera_track_definition {
		enum { k_group_tag = 'trak' };

		struct __flags {
			unsigned long unused_bit:1;
		}; static_assert(sizeof(__flags) == sizeof(unsigned long));

		__flags                                            flags;
		Yelo::TagBlock<const s_camera_track_control_point> control_points;
		long:8 * sizeof(long) * 8;
	}; static_assert(sizeof(s_camera_track_definition) == 0x30); // max count: 1
};
