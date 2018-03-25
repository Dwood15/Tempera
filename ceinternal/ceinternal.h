#pragma once
#define WIN32_LEAN_AND_MEAN
#include "exceptions/exception_handler.h"
#include "function_rewrite.h"
#include "haloforge/headers/addlog.h"
#include "haloforge/headers/forgeMain.h"

namespace spcore {
	extern FILE *debug_out;

	const void initCEI(HMODULE reason);

	const void unload();
};
