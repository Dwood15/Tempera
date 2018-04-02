#pragma once
#include "../ce_base_types.h"
//TODO: Investigate why this is the way it is.
struct argb_color;

// 0 - 255, represents a color
struct rgb_color {
	byte red;
	byte green;
	byte blue;
};
STAT_ASSRT(rgb_color, 0x3);

// 0 - 255, represents a color
struct argb_color {
	byte      alpha;
	rgb_color rgb;
};
STAT_ASSRT(argb_color, 0x4);

// represents a color, range: 0 - 1
struct real_rgb_color {
	float red;
	float green;
	float blue;
};
STAT_ASSRT(real_rgb_color, 0xC);

// represents a color, range: 0 - 1
struct real_argb_color {
	float          alpha;
	real_rgb_color rgb;
};
STAT_ASSRT(real_argb_color, 0x10);
