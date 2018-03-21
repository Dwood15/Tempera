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

