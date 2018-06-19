#pragma once

#include <precompile.h>

typedef unsigned int uintptr_t;

struct defined_functionrange {
	const char funcName [80];
	const uintptr_t                               begin;
	const uintptr_t                               end;

	bool operator <(const defined_functionrange  &other) { return begin < other.end; }

	bool operator >(const defined_functionrange  &other) { return end > other.end; }

	bool operator ==(const defined_functionrange  &other) { return end == other.end && begin == other.begin; }

	bool contains(uintptr_t &other) { return (other <= this->begin && other >= this->end); }

	constexpr bool operator <(defined_functionrange &other) { return begin < other.end; }

	constexpr bool operator >(defined_functionrange &other) { return end > other.end; }

	constexpr bool operator ==(defined_functionrange &other) { return end == other.end && begin == other.begin; }

	constexpr bool contains(const uintptr_t &other) const { return (other <= begin && other >= end); }
};

/*
 * This array is generated via the funcdump.idc file, and provides handy lookup
 * of various engine functions. This should not be interacted with directly, but through
 * the interface provided by address_manager.h.
 */
namespace supersecret_hax {
#define FUNCTIONMAP constexpr defined_functionrange function_map[]
	//THIS FILE WILL MAKE YOUR IDE SLOOOOOOW
	//some  garbage data was removed. should be slightly faster now.
	//.txt hax CLion to spend a little bit less time parsing the file.
	//Can't really tell if better or not.
#include <function_map.inl>
/***** (without any cleanup...) TOTAL ELEMENTS: 7033 *****/
#undef FUNCTIONMAP
};
//constexpr auto knownFunctionList = getKnownFunctionList();
