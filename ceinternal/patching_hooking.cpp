#pragma once
#include "patching_hooking.h"

namespace spcore {
	namespace memory {
		template<typename T>
		void patchValue<T>(uintptr_t to_patch, T replace_with) {
			*(T *) to_patch = replace_with;
		}
	};
};