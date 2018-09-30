#pragma once
#include <macros_generic.h>
#include <random>
#include "random.h"
#include "../math/real_math.h"
#include "../math/int_math.h"

namespace Yelo::Random {
	static std::default_random_engine g_generator;

	void InitializeSeed(const int32 seed) {
		g_generator.seed(seed);
	}

	real GetReal(const real lower, const real upper) {
		std::uniform_real_distribution <real> distribution(lower, upper);

		return distribution(g_generator);
	}

	real GetReal(const real_bounds bounds) {
		return GetReal(bounds.lower, bounds.upper);
	}

	real GetReal(const real_fraction_bounds bounds) {
		return GetReal(bounds.lower, bounds.upper);
	}

	real GetReal(const angle_bounds bounds) {
		return GetReal(bounds.lower, bounds.upper);
	}

	uint GetUInt32(const uint32 lower, const uint32 upper) {
		std::uniform_int_distribution <uint32> distribution(lower, upper);

		return distribution(g_generator);
	}

	int GetInt32(const int lower, const int32 upper) {
		std::uniform_int_distribution <int32> distribution(lower, upper);

		return distribution(g_generator);
	}

	ushort GetUInt16(const ushort lower, const ushort upper) {
		std::uniform_int_distribution <ushort> distribution(lower, upper);

		return distribution(g_generator);
	}

	int16 GetInt16(const int16 lower, const int16 upper) {
		std::uniform_int_distribution <int16> distribution(lower, upper);

		return distribution(g_generator);
	}

	int16 GetInt16(const short_bounds bounds) {
		return GetInt16(bounds.lower, bounds.upper);
	}
};
