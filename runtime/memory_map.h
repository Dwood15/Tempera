#pragma once

#include <macros_generic.h>

typedef unsigned int uintptr_t;

struct defined_functionrange {
	const char funcName [140];
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

struct LPCoreAddressList {
	uint core_0;
	uint core_1;
	uint core_2;
	uint core_3;
	uint core_4;
	uint core_5;
	uint core_6;
	uint core_7;
	uint CAMERA;
	uint MAP_HEADER_ADDRESS;
	uint TAG_INDEX_HEADER_ADDRESS;
};


