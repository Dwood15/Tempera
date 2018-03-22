#pragma once
#ifndef ceinternal
#define ceinternal
#include "function_rewrite.h"
#include "haloforge/headers/addlog.h"
#include "haloforge/headers/forgeMain.h"

#define MOTION_SENSOR_LOCATION 0x6B44C8

namespace spcore {
	extern FILE *debug_out;

	void constexpr initCEI(HMODULE reason);

	void constexpr unload();
};
#endif