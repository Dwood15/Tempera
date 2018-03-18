#pragma once
#ifndef TEMPERA_PATCHING_HOOKING_H
#define TEMPERA_PATCHING_HOOKING_H
typedef unsigned int uintptr_t;
namespace spcore::memory {
	template<typename T>
	void patchValue(uintptr_t to_patch, T replace_with);
};

#endif //TEMPERA_PATCHING_HOOKING_H
