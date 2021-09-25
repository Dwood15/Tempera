#pragma once
#include <macros_generic.h>

struct sbsp_stri {
	union {
		//May be useful for loops
		short indices[3];
		//The way they should be reference.
		struct {
			short a;
			short b;
			short c;
		};
	};
};

STAT_ASSERT(sbsp_stri, 0x6);

struct rectangle2d {
	short top;    //0X0
	short left;    //0X2
	short bottom; //0x4
	short right;    //0x6
};
STAT_ASSERT(rectangle2d, 0x8);

struct point2d {
	short x;
	short y;

	inline operator short *() { return reinterpret_cast<short *>(this); }
};
STAT_ASSERT(point2d, 0x4);

// bounding in short integer 
struct short_bounds {
	short lower;
	short upper;

	inline operator short *() { return reinterpret_cast<short *>(this); }
};
STAT_ASSERT(short_bounds, 0x4);

#define pad_point2d unsigned short : 16; unsigned short : 16

#define pad_rectangle2d unsigned short : 16; unsigned short : 16; unsigned short : 16; unsigned short : 16

#include "../render/render.h"