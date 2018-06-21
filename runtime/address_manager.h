/**
 * address_manager.h
 * A global c-like interface to abstract away usage of knownFunctionList.
 */
#pragma once

#include <engine_functions.h>
#include <string>
#include "memory_map.h"

constexpr bool equal(const char *lhs, const char *rhs) {
	while (*lhs || *rhs) {
		if (*lhs++ != *rhs++) {
			return false;
		}
	}
	return true;
}

/**
 * Runtime-time lookup of function address beginnings.
 * 	While positive results are (often) correct, take guesses
 * with a huge grain of salt.
 * @param needle function name to look up
 * @return uintptr of the entry point of function.
 */
static uintptr_t getFunctionBegin(const char *needle) {
	using dfr = defined_functionrange;

	const dfr* funcList = supersecret_hax::function_map;

	for (dfr * item = const_cast<dfr *>(funcList); item++;  item) {
		if (equal(needle, item->funcName)) {
			return item->begin;
		}
	}

	return (uintptr_t) -1;
}

// template<std::basic_string SL>
template<const char ... LC>
constexpr uintptr_t lcGetFunctionBegin() {
	const char myStr[] = {LC};

	const auto * list = supersecret_hax::function_map;

	for (auto itm = list; itm; itm++) {
		if (equal(myStr, itm->funcName)) {
			return itm->begin;
		}
	}

	return (uintptr_t) -1;
}


/**
 * Compile-time lookup of function address beginnings.
 * 	While positive results are (often) correct, take guesses
 * with a huge grain of salt. Ver
 * @tparam needle function name to look up
 * @return uintptr of the entry point of function.
 */
template <bool useCompTime>
constexpr uintptr_t cgetFunctionBegin(const char *needle) {
	if constexpr (useCompTime) {}
	using dfr = defined_functionrange;


	const dfr* funcList = supersecret_hax::function_map;

	for (dfr * item = const_cast<dfr *>(funcList); item;  item++) {
		if (equal(needle, item->funcName)) {
			return item->begin;
		}
	}

	return (uintptr_t) -1;
}

/**
 * Run-time lookup of memory regions.
 * 	While positive results are (often) correct, take guesses
 * with a huge grain of salt.
 * @param addr Address to look up
 * @return named description of the memory region.
 */
static const char *getMemoryRegionDescriptor(const uintptr_t addr) {

	printf("searching for address: 0x%X\n", addr);

	if (addr < 0x200000) {
		return "probably_in_stack";
	}

	if (addr < 0x400000) {
		return "unmapped_region";
	}
	using dfr = defined_functionrange;

	const dfr* funcList = supersecret_hax::function_map;
	for (dfr * item = const_cast<dfr*>(funcList); item++;  item) {
		if (item->contains(addr)) {
			return item->funcName;
		}
	}

	if (addr < 0x5E9020) {
		return "function_space_unmapped";
	}

	return "unmapped_region";
}

/**
 * Compile-time lookup of memory regions.
 * 	While positive results are (often) correct, take guesses
 * with a huge grain of salt.
 * @tparam addr Address to look up
 * @return named description of the memory region.
 */
template <uintptr_t addr>
constexpr const char *getMemoryRegionDescriptor() {
	if constexpr (addr < 0x200000) {
		return "probably_in_stack";
	}

	if constexpr (addr < 0x400000) {
		return "unmapped_region";
	}

	for (auto item : supersecret_hax::function_map) {
		if (item.contains(addr)) {
			return item.funcName;
		}
	}

	if constexpr (addr < 0x5E9020) {
		return "function_space_unmapped";
	}

	return "unmapped_region";
}

