#pragma once
/**
 * <GPLv3 License>
 */

#include "../ce_base_types.h"
//Yup, guess I'm turning into Kornman...
#define REAL_ASSRT(name, members) STAT_ASSRT(name, members * sizeof(real))
//Only bother porting what we actually need.
struct real_bounds {
	real lower;
	real upper;
};  REAL_ASSRT(real_point2d, 0x2);

struct real_plane2d {
	real i;
	real j;
	real d;
}; REAL_ASSRT (real_point2d, 0x3);

struct real_plane3d {
	inline real_plane2d ijd;
	real k;
}; REAL_ASSRT (real_plane2d, 0x4);

struct real_point2d {
	real x;
	real y;
}; REAL_ASSRT(real_point3d, 0x2);

struct real_point3d {
	inline real_point2d xy;
	real z;
}; REAL_ASSRT(real_point3d, 0x3);