#pragma once
#ifndef ce_base_types
#define ce_base_types
#include "shitty_macros.h"

#pragma region  colors
struct argb_color;

// 0 - 255, represents a color
struct rgb_color {
	byte red;
	byte green;
	byte blue;
}; static_assert(sizeof(rgb_color) == 0x3, STATIC_ASSERT_FAIL);

// 0 - 255, represents a color
struct argb_color {
	byte alpha;
	rgb_color rgb;
}; static_assert(sizeof(argb_color) == 0x4, STATIC_ASSERT_FAIL);

// represents a color, range: 0 - 1
struct real_rgb_color {
	float red;
	float green;
	float blue;
}; static_assert(sizeof(real_rgb_color) == 0xC, STATIC_ASSERT_FAIL);

// represents a color, range: 0 - 1
struct real_argb_color {
	float alpha;
	real_rgb_color rgb;
}; static_assert(sizeof(real_argb_color) == 0x10, STATIC_ASSERT_FAIL);
#pragma endregion

#pragma region floating point-based structures
//reals in halo === float.
struct real_euler_angles2d {
	float yaw;
	float pitch;
}; static_assert(sizeof(real_euler_angles2d) == 0x8, STATIC_ASSERT_FAIL);

struct real_vector2d {
	float x;
	float y;
}; static_assert(sizeof(real_vector2d) == 0x8, STATIC_ASSERT_FAIL);

struct real_vector3d {
	real x;
	real y;
	real z;
}; static_assert(sizeof(real_vector3d) == 0xC, STATIC_ASSERT_FAIL);

struct real_point3d {
	real_vector2d xy;
	float z;
}; static_assert(sizeof(real_point3d) == 0xC, STATIC_ASSERT_FAIL);
#pragma endregion

#pragma region whole number-based structures
struct rectangle2d {
	short top;    //0X0
	short left;    //0X2
	short bottom; //0x4
	short right;    //0x6
}; static_assert(sizeof(rectangle2d) == 0x8, STATIC_ASSERT_FAIL);
#pragma endregion

/// <summary>	Handle to data allocated by the engine's data-array construct. </summary>
struct datum_index {
	typedef short t_index;
	typedef short t_salt;

	union {
		unsigned long handle;

		struct {
			t_index index;
			t_salt salt;
		};
	};
}; static_assert(sizeof(datum_index) == 0x4, STATIC_ASSERT_FAIL);
#endif