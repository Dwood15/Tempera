#pragma once


#include <precompile.h>

namespace Yelo {
	// The epsilon value I've seen Halo1 use
	// TODO: See if we need to throw this into the base module
	const real K_REAL_EPSILON = 0.000099999997f; // 0x38D1B717

	const long K_TICKS_PER_SECOND = 30;
	const real  K_SECONDS_PER_TICK = 0.033333335f; // 0x3D088889
};

namespace Yelo::blam {
	void *__cdecl system_malloc(size_t size) { return GlobalAlloc(GMEM_FIXED, size); }

	void __cdecl system_free(void *pointer) { GlobalFree(pointer); }

	void *__cdecl system_realloc(void *pointer, size_t size) {
		//assert(size >= 0);  we use size_t, which is unsigned and thus this will always be true
		if (pointer == nullptr) {
			return system_malloc(size);

		} else if (size == 0) {
			system_free(pointer);
			return nullptr;
		}

		return GlobalReAlloc(pointer, size, GMEM_MOVEABLE);
	}
};
