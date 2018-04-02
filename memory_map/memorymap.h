/**
 * Insert GPL here.
 **/
#pragma once

#include "../ceinternal.h"
#include<list>
/**
 * At a likely 20k max total # of values, it's possible to do searches as fast as (or faster than)
 * a hashmap//other structure.
 * **/
struct defined_functionrange {
	const uintptr_t begin;
	const uintptr_t end;
	//I don't know how to guarantee an _exact_ size for all char *'s so instead, I'm using char **. :)
	char**functionName;

	bool operator<(defined_functionrange const &other) { return begin < other.end; }

	bool operator>(defined_functionrange const &other) { return begin > other.end; }

	bool operator==(defined_functionrange const &other) { return end == other.end && begin == other.begin; }

	bool contains(uintptr_t const &other) { return ( other <= begin && other >= end ); }

	void setName(char** newName) { functionName = newName; }
};

static std::list<defined_functionrange> knownfunctionlist;
