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

constexpr const defined_functionrange * getKnownFunctionList();
//constexpr auto knownFunctionList = getKnownFunctionList();
