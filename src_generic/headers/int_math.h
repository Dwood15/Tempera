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
}; STAT_ASSRT(sbsp_stri, 0x6);

struct rectangle2d {
	short top;    //0X0
	short left;    //0X2
	short bottom; //0x4
	short right;    //0x6
}; STAT_ASSRT(rectangle2d, 0x8);
