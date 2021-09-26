#pragma once

#include <macros_generic.h>

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpadded"
#endif


#include <d3dx9math.h>
#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wpadded"
#endif

//#pragma clang diagnostic pop

//TODO: Investigate why this is the way it is.
struct argb_color;

// 0 - 255, represents a color
struct rgb_color {
	byte red;
	byte green;
	byte blue;
};
STAT_ASSERT(rgb_color, 0x3);

// 0 - 255, represents a color
struct argb_color {
	byte      alpha;
	rgb_color rgb;
};
STAT_ASSERT(argb_color, 0x4);

// represents a color, range: 0 - 1
struct real_rgb_color {
	float red;
	float green;
	float blue;
};
STAT_ASSERT(real_rgb_color, 0xC);

// represents a color, range: 0 - 1
struct real_argb_color {
	float          alpha;
	real_rgb_color rgb;
};
STAT_ASSERT(real_argb_color, 0x10);

//#define vect float
struct vect2 : public D3DXVECTOR2 {
	using D3DXVECTOR2::D3DXVECTOR2;
};
STAT_ASSERT(vect2, 0x8);

//-------------------------
struct HaloColor {
	float a;
	float r;
	float g;
	float b;

	//HaloColor constexpr(float a, float r, float g, float b);
	HaloColor(float a, float r, float g, float b);
};

const auto hGreen = HaloColor(1,0,1,0);
const auto hBlue  = HaloColor(1,0,0,1);
const auto hRed   = HaloColor(1,1,0,0);

#include "real_math.h"

struct vect3 : public D3DXVECTOR3 {
	using D3DXVECTOR3::D3DXVECTOR3;
	// vect3() : D3DXVECTOR3() {}
	//
	// vect3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}
	inline operator float *() { return reinterpret_cast<float *>(this); }
	inline operator real_point3d *() { return reinterpret_cast<real_point3d *>(this); }
};

STAT_ASSERT(vect3, 0xC);
STAT_ASSERT(vect3, sizeof(D3DXVECTOR3));

class CMath {
public:
	float Get2dDistance(float x, float y);

	float GetAngleDistance(vect3 local, vect3 toTarget);

	vect2 GetAngleToTarget(vect3 local, vect3 target);
};
