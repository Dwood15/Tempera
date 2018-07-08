#pragma once
#include "../ce_base_types.h"

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

static_assert(sizeof(sbsp_stri) == 0x6);

struct rectangle2d {
	short top;    //0X0
	short left;    //0X2
	short bottom; //0x4
	short right;    //0x6
};
struct point2d {
	short x;
	short y;

	inline operator short *() { return reinterpret_cast<short *>(this); };
};
static_assert(sizeof(rectangle2d) == 0x8);

// bounding in short integer values
struct short_bounds {
	short lower;
	short upper;

	inline operator short *() { return reinterpret_cast<short *>(this); };
};


#define pad_point2d unsigned short : 16; unsigned short : 16

#define pad_rectangle2d unsigned short : 16; unsigned short : 16; unsigned short : 16; unsigned short : 16


