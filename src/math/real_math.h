#pragma once
/**
 * Code provided under GPLv3 or later GPL License
 */

#include <macros_generic.h>


//Yup, guess I'm turning into Kornman...
#define REAL_ASSRT(name, members) STAT_ASSERT(name, members * sizeof(real))
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
	real         k;
}; REAL_ASSRT (real_plane3d, 0x4);

struct real_point2d {
	real x;
	real y;
}; REAL_ASSRT(real_point2d, 0x2);
STAT_ASSERT(real_point2d, 0x8);

struct real_point3d : public real_point2d {
	real z;
}; REAL_ASSRT(real_point3d, 0x3);

//reals in halo === float.
struct real_euler_angles2d {
	float yaw;
	float pitch;
}; STAT_ASSERT(real_euler_angles2d, 0x8);

struct real_vector2d {
	float x;
	float y;
}; STAT_ASSERT(real_vector2d, 0x8);

struct real_vector3d {
	union {
		real  x;
		float i;
	};
	union {
		real  y;
		float j;
	};

	union {
		real  z;
		float k;
	};

	inline operator real_point3d &() { return reinterpret_cast<real_point3d &>(*this); }
};

STAT_ASSERT(real_vector3d, 0xC);

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
REAL_ASSRT(real_rectangle2d, 0x4)

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
REAL_ASSRT(real_quaternion, 0x4)

typedef float angle;
typedef float real_fraction;
// bounding in real angle values
struct angle_bounds {
	angle lower;
	angle upper;
};
REAL_ASSRT(angle_bounds, 0x2)

// bounding in real values
struct real_fraction_bounds {
	real lower;
	real upper;
};
REAL_ASSRT(real_fraction_bounds, 0x2);


struct real_orientation3d {
	real_quaternion rotation;
	real_point3d    translation;
	real            scale;
};

struct real_matrix3x3 {
	real_vector3d Forward;
	real_vector3d Left;
	real_vector3d Up;
};

struct real_matrix4x3 {
	real          Scale;
	real_vector3d Forward;
	real_vector3d Left;
	real_vector3d Up;
	real_point3d  Position;

	// void ConvertTo4x4(D3DMATRIX &d3dmat) const {
	// 	d3dmat._11 = Forward.i;
	// 	d3dmat._12 = Forward.j;
	// 	d3dmat._13 = Forward.k;
	// 	d3dmat._14 = 0.0f;
	// 	d3dmat._21 = Left.i;
	// 	d3dmat._22 = Left.j;
	// 	d3dmat._23 = Left.k;
	// 	d3dmat._24 = 0.0f;
	// 	d3dmat._31 = Up.i;
	// 	d3dmat._32 = Up.j;
	// 	d3dmat._33 = Up.k;
	// 	d3dmat._34 = 0.0f;
	// 	d3dmat._41 = Position.x;
	// 	d3dmat._42 = Position.y;
	// 	d3dmat._43 = Position.z;
	// 	d3dmat._44 = Scale;
	// }
};
//STAT_ASSERT(real_matrix4x3, 0x4 + sizeof(real_vector3d) * 3 + sizeof(real_point3d));

struct real_matrix3x4 {
	float m[4][3];
};

#include <enums/math.h>
namespace Yelo::blam {
	real periodic_function_evaluate(Enums::periodic_function function_type, real input);

	real transition_function_evaluate(Enums::transition_function function_type, real input);
};
