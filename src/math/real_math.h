#pragma once
/**
 * <GPLv3 License>
 */

#include <macros_generic.h>
#include "../ce_base_types.h"
//Yup, guess I'm turning into Kornman...
#define REAL_ASSRT(name, members) STAT_ASSRT(name, members * sizeof(real))
//Only bother porting what we actually need.
struct real_bounds {
	real lower;
	real upper;
};
STAT_ASSERT(real_bounds, 0x8);
REAL_ASSRT(real_bounds, 0x2);

struct real_plane2d {
	real i;
	real j;
	real d;
}; REAL_ASSRT (real_plane2d, 0x3);

struct real_plane3d {
	real_plane2d ijd;
	real k;
}; REAL_ASSRT (real_plane3d, 0x4);

struct real_point2d {
	real x;
	real y;
}; REAL_ASSRT(real_point2d, 0x2);

struct real_point3d : public real_point2d {
	real z;
}; REAL_ASSRT(real_point3d, 0x3);

//reals in halo === float.
struct real_euler_angles2d {
	float yaw;
	float pitch;
}; STAT_ASSRT(real_euler_angles2d, 0x8);

struct real_vector2d {
	float x;
	float y;
}; STAT_ASSRT(real_vector2d, 0x8);

struct real_vector3d {
	real x;
	real y;
	real z;
}; STAT_ASSRT(real_vector3d, 0xC);

struct real_euler_angles3d {
	// Up, Down offset degrees
	real yaw;

	// Side to Side offset degrees
	real pitch;

	// Left to Right offset degrees
	real roll;

	inline operator float *() { return reinterpret_cast<float *>(this); }

	inline operator real_euler_angles2d *() { return reinterpret_cast<real_euler_angles2d *>(this); }
};

struct real_rectangle2d {
	real x0, x1; // top, bottom
	real y0, y1; // left, right
};

// a 3d real vector with ?
struct real_quaternion {
	// X-Component
	real i;

	// Y-Component
	real j;

	// Z-Component
	real k;

	// ?
	real w;
};

typedef float angle;
typedef float real_fraction;
// bounding in real angle values
struct angle_bounds
{
	angle lower;
	angle upper;
};

// bounding in real values
struct real_fraction_bounds
{
	real lower;
	real upper;
};
